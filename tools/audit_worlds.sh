#!/usr/bin/env bash
# Runs the ClassCoverage (class audit) and TraceDiff tests on every world of a host project, headless,
# one engine launch per world, and prints one summary line each. Usage:
#   tools/audit_worlds.sh <project dir with run-editor.sh> [map ...]
# With no maps every .umap under Content is run. Per-world CSVs land in Saved/FlightForge/.
# FF_GAMEMODE_PORT / FF_DRONE_PORT move the servers off a port already in use (a deployed sim, an ssh -L tunnel).
# FF_MAP_TIMEOUT is the per-map budget; raise it for a world whose Nanite/DDC build has not been done yet.
set -uo pipefail

PROJECT=${1:?project dir}
shift
cd "$PROJECT"

if [ $# -eq 0 ]; then
  # packed sublevels, level-composition sources and the menu/template maps are not worlds
  mapfile -t MAPS < <(find Content -name '*.umap' \
    | grep -v '/Meshes/\|/Maps/Source/\|Ind_War_\|Preview\|/UI/\|/ThirdPerson/' \
    | sed 's|^Content/|/Game/|; s|\.umap$||' | sort)
else
  MAPS=("$@")
fi

BUDGET=${FF_MAP_TIMEOUT:-900}

LOG_DIR=Saved/FlightForge/audit
mkdir -p "$LOG_DIR"
SUMMARY=$LOG_DIR/summary.txt
: > "$SUMMARY"

for MAP in "${MAPS[@]}"; do
  NAME=$(basename "$MAP")
  LOG=$LOG_DIR/$NAME.log
  UE_EDITOR_BIN=UnrealEditor-Cmd timeout "$BUDGET" ./run-editor.sh "$MAP" -game -RenderOffScreen -unattended -nopause -nosplash -stdout -AllowStdOutLogVerbosity \
    -FlightForgeGameModePort=${FF_GAMEMODE_PORT:-8551} -FlightForgeDroneStartPort=${FF_DRONE_PORT:-4000} \
    -ExecCmds="Automation RunTests FlightForge.Segmentation" > "$LOG" 2>&1 &
  PID=$!
  # the tests do not quit the engine; leave once the queue has drained (or the run dies)
  for _ in $(seq 1 "$BUDGET"); do
    if grep -aq "Automation Test Queue Empty\|Signal 11\|Fatal error\|Failed to enter" "$LOG"; then break; fi
    kill -0 $PID 2>/dev/null || break
    sleep 1
  done
  EDITOR=$(pgrep -f "UnrealEditor-Cmd.*$NAME" | head -1)
  [ -n "$EDITOR" ] && kill "$EDITOR" 2>/dev/null
  wait $PID 2>/dev/null

  AUDIT=$(grep -a -o "\[AuditClasses\] [^:]*: .*bounds area" "$LOG" | tail -1 | sed 's/\[AuditClasses\] //')
  DIFF=$(grep -a -o "SegDiff: [0-9]* comparable px: agree [0-9]* ([0-9.]*%" "$LOG" | tail -1 | sed 's/SegDiff: //')
  COVERAGE=$(grep -a -o "Test Completed. Result={[A-Za-z]*} Name={ClassCoverage}" "$LOG" | tail -1 | grep -o "{[A-Za-z]*}" | head -1)
  VERDICT=$(grep -a -o "Test Completed. Result={[A-Za-z]*} Name={TraceDiff}" "$LOG" | tail -1 | grep -o "{[A-Za-z]*}" | head -1)
  LINE="$NAME | audit: ${AUDIT:-no result} ${COVERAGE:-} | tracediff: ${DIFF:-no result} ${VERDICT:-}"
  echo "$LINE" | tee -a "$SUMMARY"
done
