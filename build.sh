#!/usr/bin/env bash
cp -vr cmakelists/* lexilla
cd lexilla
cmake -Bbuild -H.
cmake --build ./build -- TestLexers
