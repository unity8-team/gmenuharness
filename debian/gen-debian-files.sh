#!/bin/bash
set -e

DIR="$1"
DISTRO=$(lsb_release -c -s)
SOURCE_SYMBOLS_FILE="${DIR}/libgmenuharness0.1.${DISTRO}.symbols"
TARGET_SYMBOLS_FILE="${DIR}/libgmenuharness0.1.symbols"

if [ -f "$SOURCE_SYMBOLS_FILE" ]; then
  cp "$SOURCE_SYMBOLS_FILE" "$TARGET_SYMBOLS_FILE"
fi
