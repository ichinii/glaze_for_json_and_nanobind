#!/usr/bin/env bash

set -e

git submodule update --init --recursive

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
cmake --build build

# ---------------------------------------------------------------------------
# json test
# ---------------------------------------------------------------------------

echo
echo
echo "Running json test..."
./build/test_json

# ---------------------------------------------------------------------------
# nanobind test
# ---------------------------------------------------------------------------

echo
echo
echo "Running nanobind test..."
(cp test.py build && cd build && python test.py)

# ---------------------------------------------------------------------------
# finished
# ---------------------------------------------------------------------------

echo
echo
echo "Finished all tests successfully."
