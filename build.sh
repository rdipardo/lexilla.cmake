#!/usr/bin/env bash
cp -vr cmakelists/* lexilla
cd lexilla
cmake -Bbuild -H. -DDEBUG=1
cmake --build ./build -- TestLexers
