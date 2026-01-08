#!/bin/bash
set -e

cmake -S . -B build -G Ninja
cmake --build build

exe="$(find build -maxdepth 2 -type f -executable -print -quit)"
echo "Running: $exe"
"$exe"
