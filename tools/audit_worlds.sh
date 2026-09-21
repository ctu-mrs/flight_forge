#!/usr/bin/env bash
# Runs the segmentation audit and the TraceDiff test on every world of a host project, headless,
# one engine launch per world, and prints one summary line each. Usage:
#   tools/audit_worlds.sh <project dir with run-editor.sh> [map ...]
# With no maps every .umap under Content/Worlds is run. Per-world CSVs land in Saved/FlightForge/.
set -uo pipefail

PROJECT=${1:?project dir}
shift
cd "$PROJECT"

if [ $# -eq 0 ]; then
  # asset-preview levels (Ind_War_Box_01 and the like) are not worlds
  mapfile -t MAPS < <(find Content/Worlds -name '*.umap' | grep -v '/Meshes/\|Ind_War_\|Preview' | sed 's|^Content/|/Game/|; s|\.umap$||' | sort)
else
  MAPS=("$@")
fi

LOG_DIR=Saved/FlightForge/audit
mkdir -p "$LOG_DIR"
SUMMARY=$LOG_DIR/summary.txt
: > "$SUMMARY"

for MAP in "${MAPS[@]}"; do
  NAME=$(basename "$MAP")
  LOG=$LOG_DIR/$NAME.log
  # a -game launch runs a single -ExecCmds command; the TraceDiff test runs the class audit itself
  UE_EDITOR_BIN=UnrealEditor-Cmd timeout 900 ./run-editor.sh "$MAP" -game -RenderOffScreen -unattended -nopause -nosplash -stdout -AllowStdOutLogVerbosity \
    -ExecCmds="Automation RunTests FlightForge.Segmentation" > "$LOG" 2>&1 &
  PID=$!
  # the test does not quit the engine; leave once its verdict is logged (or the run dies)
  for _ in $(seq 1 900); do
    if grep -aq "Test Completed.*TraceDiff\|Signal 11\|Fatal error\|Failed to enter" "$LOG"; then break; fi
    kill -0 $PID 2>/dev/null || break
    sleep 1
  done
  EDITOR=$(pgrep -f "UnrealEditor-Cmd.*$NAME" | head -1)
  [ -n "$EDITOR" ] && kill "$EDITOR" 2>/dev/null
  wait $PID 2>/dev/null

  AUDIT=$(grep -a -o "\[AuditClasses\] [^:]*: .*bounds area" "$LOG" | tail -1 | sed 's/\[AuditClasses\] //')
  DIFF=$(grep -a -o "SegDiff: [0-9]* comparable px: agree [0-9]* ([0-9.]*%" "$LOG" | tail -1 | sed 's/SegDiff: //')
  VERDICT=$(grep -a -o "Test Completed. Result={[A-Za-z]*} Name={TraceDiff}" "$LOG" | tail -1 | grep -o "{[A-Za-z]*}" | head -1)
  LINE="$NAME | audit: ${AUDIT:-no result} | tracediff: ${DIFF:-no result} ${VERDICT:-}"
  echo "$LINE" | tee -a "$SUMMARY"
done
