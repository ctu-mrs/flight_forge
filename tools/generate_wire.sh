#!/usr/bin/env bash
# Regenerates the wire bindings and the vendored FlatBuffers runtime from schema/; FF_WIRE_CHECK=1 only reports drift.
set -euo pipefail

FLATBUFFERS_VERSION=25.12.19
FLATBUFFERS_SHA256=f81c3162b1046fe8b84b9a0dbdd383e24fdbcf88583b9cb6028f90d04d90696a

ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)
CONNECTOR=${FF_CONNECTOR_DIR:-$ROOT/mrs_flight_forge_connector}
CACHE=${XDG_CACHE_HOME:-$HOME/.cache}/flightforge-flatc/$FLATBUFFERS_VERSION
FLATC=$CACHE/bin/flatc
SOURCE=$CACHE/flatbuffers-$FLATBUFFERS_VERSION

RUNTIME_HEADERS=(allocator.h array.h base.h buffer.h buffer_ref.h default_allocator.h detached_buffer.h flatbuffer_builder.h
                 flatbuffers.h stl_emulation.h string.h struct.h table.h vector.h vector_downward.h verifier.h)

fetch_flatc() {
  [[ -x $FLATC && -d $SOURCE/include/flatbuffers ]] && return

  mkdir -p "$CACHE"
  local Archive=$CACHE/flatbuffers-$FLATBUFFERS_VERSION.tar.gz
  if [[ ! -f $Archive ]]; then
    curl -fsSL -o "$Archive.part" "https://github.com/google/flatbuffers/archive/refs/tags/v$FLATBUFFERS_VERSION.tar.gz"
    mv "$Archive.part" "$Archive"
  fi
  echo "$FLATBUFFERS_SHA256  $Archive" | sha256sum -c --quiet -

  rm -rf "$SOURCE"
  tar -xzf "$Archive" -C "$CACHE"
  echo "building flatc $FLATBUFFERS_VERSION into $CACHE, log in $CACHE/build.log" >&2
  if ! { cmake -S "$SOURCE" -B "$SOURCE/build" -DCMAKE_BUILD_TYPE=Release -DFLATBUFFERS_BUILD_TESTS=OFF \
               -DFLATBUFFERS_BUILD_FLATLIB=OFF -DFLATBUFFERS_BUILD_FLATHASH=OFF &&
         cmake --build "$SOURCE/build" --target flatc -j"$(nproc)"; } >"$CACHE/build.log" 2>&1; then
    tail -20 "$CACHE/build.log" >&2
    exit 1
  fi
  mkdir -p "$(dirname "$FLATC")"
  cp "$SOURCE/build/flatc" "$FLATC"
}

generate() {
  local Stage=$1
  local Schemas=(common.fbs robot.fbs sensors.fbs sim.fbs flightforge.fbs)

  mkdir -p "$Stage/cpp" "$Stage/python" "$Stage/runtime"
  # relative paths keep absolute checkout locations out of the generated files
  (
    cd "$ROOT/schema"
    "$FLATC" --cpp --gen-object-api --scoped-enums --gen-compare --cpp-std c++17 -o "$Stage/cpp" "${Schemas[@]}"
    "$FLATC" --binary --schema --bfbs-comments --bfbs-filenames . -o "$Stage/cpp" flightforge.fbs
    "$FLATC" --python --gen-object-api -o "$Stage/python" "${Schemas[@]}"
  )

  for Header in "${RUNTIME_HEADERS[@]}"; do
    cp "$SOURCE/include/flatbuffers/$Header" "$Stage/runtime/"
  done
  cp "$SOURCE/LICENSE" "$Stage/runtime/LICENSE"
  echo "$FLATBUFFERS_VERSION" > "$Stage/runtime/VERSION"
}

DRIFT=0
sync_dir() {
  local From=$1 To=$2
  if [[ ${FF_WIRE_CHECK:-0} == 1 ]]; then
    if ! diff -r -x __pycache__ "$From" "$To" >/dev/null 2>&1; then
      echo "stale: ${To#"$ROOT"/}" >&2
      DRIFT=1
    fi
    return
  fi
  rm -rf "$To"
  mkdir -p "$(dirname "$To")"
  cp -r "$From" "$To"
}

if ! grep -q "\"flatbuffers==$FLATBUFFERS_VERSION\"" "$CONNECTOR/python/pyproject.toml"; then
  echo "python/pyproject.toml must pin flatbuffers==$FLATBUFFERS_VERSION" >&2
  exit 1
fi

fetch_flatc

STAGE=$(mktemp -d)
trap 'rm -rf "$STAGE"' EXIT
generate "$STAGE"

sync_dir "$STAGE/cpp" "$ROOT/FlightForgePlugin/Source/MessageSerialization/Public/Generated"
sync_dir "$STAGE/runtime" "$ROOT/FlightForgePlugin/Source/ThirdParty/include/flatbuffers"
sync_dir "$STAGE/cpp" "$CONNECTOR/include/flight_forge_connector/wire"
sync_dir "$STAGE/runtime" "$CONNECTOR/include/flatbuffers"
sync_dir "$STAGE/python/flightforge/wire" "$CONNECTOR/python/flightforge/wire"

exit $DRIFT
