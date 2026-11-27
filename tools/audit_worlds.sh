#!/usr/bin/env bash
# Runs ClassCoverage and TraceDiff on every world of a host project, headless, one launch per world.
#   tools/audit_worlds.sh [-e src|stock] [-p <host project>] [-o <out dir>] [map ...]
# -e picks the host (src: ~/Data/mrs_flight_forge 5.7 src, the patched engine; stock: ~/Data/mrs_flight_forge 5.7).
# Per-world settings (exclusions, game mode, rig pose, collision stand-ins) are in audit_worlds.tsv beside this script.
# FF_PORT (18600) is the server port; FF_MAP_TIMEOUT (900 s) the per-launch budget; FF_RETRY=0 scores the first launch only.
set -uo pipefail

ENGINE=src PROJECT= OUT=
while getopts "e:p:o:" Opt; do
  case $Opt in
    e) ENGINE=$OPTARG ;;
    p) PROJECT=$OPTARG ;;
    o) OUT=$OPTARG ;;
    *) exit 2 ;;
  esac
done
shift $((OPTIND - 1))

case $ENGINE in
  src)   PROJECT=${PROJECT:-$HOME/Data/mrs_flight_forge 5.7 src} ;;
  stock) PROJECT=${PROJECT:-$HOME/Data/mrs_flight_forge 5.7} ;;
  *)     echo "-e src|stock" >&2; exit 2 ;;
esac
TABLE=$(cd "$(dirname "$0")" && pwd)/audit_worlds.tsv
PORT=${FF_PORT:-18600}
BUDGET=${FF_MAP_TIMEOUT:-900}
RETRY=${FF_RETRY:-1}
MIN_FREE_GB=15

# the deployed simulator and its ssh tunnel
if [ "$PORT" -eq 8551 ] || [ "$PORT" -eq 8600 ] || { [ "$PORT" -ge 4000 ] && [ "$PORT" -le 4010 ]; }; then
  echo "port $PORT belongs to the live simulator" >&2; exit 2
fi
if ss -ltn "sport = :$PORT" | grep -q LISTEN; then
  echo "port $PORT is already in use; a leftover editor would take half the requests" >&2; exit 2
fi

cd "$PROJECT" || exit 2
OUT=${OUT:-$PROJECT/Saved/FlightForge/audit_$ENGINE}
mkdir -p "$OUT"
SUMMARY=$OUT/summary.txt
: > "$SUMMARY"

if [ $# -eq 0 ]; then
  # packed sublevels, level-composition sources and the menu/template maps are not worlds
  mapfile -t MAPS < <(find -L Content -name '*.umap' \
    | grep -v '/Meshes/\|/Maps/Source/\|Ind_War_\|Preview\|/UI/\|/ThirdPerson/' \
    | sed 's|^Content/|/Game/|; s|\.umap$||' | sort)
else
  MAPS=("$@")
fi

setting() {
  awk -F'\t' -v m="$1" -v k="$2" '$1 == m && $2 == k { print $3; exit }' "$TABLE"
}

free_gb() {
  df --output=avail -BG / | tail -1 | tr -dc 0-9
}

# one launch //{

# run_map <log> <map url> [extra args]: echoes how the launch ended
run_map() {
  local Log=$1 Url=$2
  shift 2
  local Start=$SECONDS Reason=timeout
  UE_EDITOR_BIN=UnrealEditor-Cmd ./run-editor.sh "$Url" -game -RenderOffScreen -unattended -nopause -nosplash -stdout -AllowStdOutLogVerbosity \
    -FlightForgePort="$PORT" -ExecCmds="Automation RunTests FlightForge.Segmentation" "$@" > "$Log" 2>&1 &
  local Pid=$!  # run-editor.sh execs the editor, so this is the editor itself once it has started

  while [ $((SECONDS - Start)) -lt "$BUDGET" ]; do
    if grep -aq "Automation Test Queue Empty" "$Log"; then Reason=done; break; fi
    if grep -aqE "Signal 11|Fatal error|Failed to enter|Unhandled Exception|Assertion failed" "$Log"; then Reason=crash; break; fi
    if ! kill -0 "$Pid" 2>/dev/null; then
      wait "$Pid"; local Code=$?
      [ "$Code" -eq 3 ] && Reason="gpu watchdog (exit 3)" || Reason="exited $Code"
      Pid=; break
    fi
    if [ "$(free_gb)" -lt "$MIN_FREE_GB" ]; then Reason=diskfull; break; fi
    sleep 2
  done

  if [ -n "$Pid" ]; then
    local Exe
    Exe=$(basename "$(readlink "/proc/$Pid/exe" 2>/dev/null)")
    if [ "$Exe" = UnrealEditor-Cmd ]; then
      kill "$Pid" 2>/dev/null
      for _ in $(seq 1 10); do kill -0 "$Pid" 2>/dev/null || break; sleep 1; done
      kill -9 "$Pid" 2>/dev/null
    else
      echo "pid $Pid is $Exe, not the editor; not killed" >&2
    fi
    wait "$Pid" 2>/dev/null
  fi
  echo "$Reason after $((SECONDS - Start)) s"
}

# //}

# scoring //{

result_of() {
  grep -a -o "Test Completed. Result={[A-Za-z]*} Name={$2}" "$1" | tail -1 | grep -o "{[A-Za-z]*}" | head -1 | tr -d '{}'
}

trace_line() {
  grep -a -o "SegDiff: [0-9]* comparable px: agree [0-9]* ([0-9.]*%.*nanite [0-9]* ([0-9]* correct)" "$1" | tail -1 | sed 's/SegDiff: //; s/, sky.*nanite/; nanite/'
}

# //}

echo "$ENGINE engine, $PROJECT, ${#MAPS[@]} maps, port $PORT" | tee -a "$SUMMARY"
PASSED=0 FAILED=0 EXCLUDED=0

for MAP in "${MAPS[@]}"; do
  if [ "$(free_gb)" -lt "$MIN_FREE_GB" ]; then
    echo "stopped: less than $MIN_FREE_GB GB free on /" | tee -a "$SUMMARY"; break
  fi

  EXCLUDE=$(setting "$MAP" exclude)
  if [ -n "$EXCLUDE" ]; then
    EXCLUDED=$((EXCLUDED + 1))
    echo "$MAP | excluded: $EXCLUDE" | tee -a "$SUMMARY"
    continue
  fi

  URL=$MAP
  GAME=$(setting "$MAP" game)
  [ -n "$GAME" ] && URL="$MAP?game=$GAME"
  ARGS=()
  POSE=$(setting "$MAP" pose)
  [ -n "$POSE" ] && ARGS+=("-FlightForgeAuditPose=$POSE")
  STANDIN=$(setting "$MAP" standin)
  [ -n "$STANDIN" ] && ARGS+=("-FlightForgeAuditStandIn=$STANDIN")

  NAME=$(basename "$MAP")
  NOTE=
  LOG=$OUT/$NAME.log
  RUN=$(run_map "$LOG" "$URL" "${ARGS[@]}")
  TRACE=$(result_of "$LOG" TraceDiff)
  # a first launch can score shaders that are still compiling into a young DDC
  if [ "$TRACE" != Success ] && [ "$RETRY" -ne 0 ]; then
    NOTE=" (first launch: ${TRACE:-no result}, $RUN, $(trace_line "$LOG"))"
    LOG=$OUT/$NAME.warm.log
    RUN=$(run_map "$LOG" "$URL" "${ARGS[@]}")
    TRACE=$(result_of "$LOG" TraceDiff)
  fi

  AUDIT=$(grep -a -o "\[AuditClasses\] [^:]*: .*bounds area" "$LOG" | tail -1 | sed 's/\[AuditClasses\] [^:]*: //')
  COVERAGE=$(result_of "$LOG" ClassCoverage)
  [ "$TRACE" = Success ] && PASSED=$((PASSED + 1)) || FAILED=$((FAILED + 1))
  echo "$MAP | $RUN | tracediff: ${TRACE:-no result} $(trace_line "$LOG")$NOTE | classes: ${COVERAGE:-no result} ${AUDIT}" | tee -a "$SUMMARY"
done

echo "TraceDiff: $PASSED passed, $FAILED failed, $EXCLUDED excluded" | tee -a "$SUMMARY"
[ "$FAILED" -eq 0 ]
