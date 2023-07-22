#!/usr/bin/env bash
set -e
cp -vr cmakelists/* lexilla
[ ! -d ./scite ] && hg clone http://hg.code.sf.net/p/scintilla/scite
[ ! -d scite/.vscode ] && cp -vr .vscode scite/
[ ! -f scite/compile_flags.txt ] && \
  cp -v compile_flags_scite.txt scite/compile_flags.txt

cmake -Bscite/.cmake -H. -DDEBUG=1
cmake --build scite/.cmake -- scintilla
cd scite/gtk
DEBUG=1 GTK3=1 make -j8
