#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$ROOT_DIR/build-vcpkg"
VCPKG_ROOT="${VCPKG_ROOT:-$HOME/vcpkg}"
TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"

if ! command -v cmake >/dev/null 2>&1; then
  echo "CMake was not found. Install CMake and try again." >&2
  exit 1
fi

if [ ! -f "$TOOLCHAIN_FILE" ]; then
  echo "vcpkg toolchain was not found: $TOOLCHAIN_FILE" >&2
  echo "Install vcpkg or set VCPKG_ROOT to the folder where vcpkg is installed." >&2
  exit 1
fi

cmake -S "$ROOT_DIR/src" -B "$BUILD_DIR" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
  -DVCPKG_MANIFEST_DIR="$ROOT_DIR"

cmake --build "$BUILD_DIR"
