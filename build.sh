#!/usr/bin/env bash
[ -n "$1" ] && TASK=$1 || TASK=TestLexers
cp -vr cmakelists/* lexilla
cd lexilla
cmake -Bbuild -H. -DDEBUG=1
cmake --build ./build -- $TASK
