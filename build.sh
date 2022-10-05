#!/usr/bin/env bash
set -e
cp -vr cmakelists/* lexilla
cd lexilla
cmake -Bbuild -H. -DDEBUG=1
cmake --build ./build -- lexilla

cd ../scintilla/gtk
GTK3=1 make -j8

cd ../..
if [ ! -d ./scite ]; then
    hg clone http://hg.code.sf.net/p/scintilla/scite
    cp -v compile_flags_scite.txt scite/compile_flags.txt
    cp -vr .vscode scite/
fi
cd scite/gtk
GTK3=1 make -j8
