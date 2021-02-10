#!/usr/bin/env bash
cp -vr cmake/* lexilla
cd lexilla
cmake -Bbuild -H.
cmake --build ./build -- TestLexers
