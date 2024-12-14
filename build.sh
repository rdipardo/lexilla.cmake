#!/usr/bin/env bash
set -e
cp -vr cmakelists/* lexilla
cp .gitignore lexilla
cmake -B.cmake -H. -DDEBUG=1
mv scite/scite/* scite/ || true
rm -rf scite/lexilla scite/scintilla
cmake --build .cmake -- scintilla
pushd scite/gtk
make -q || DEBUG=1 GTK3=1 make -j8 
popd

[ ! -d scite/.vscode ] && cp -vr .vscode scite/
[ ! -f scite/compile_flags.txt ] && \
  cp -v compile_flags_scite.txt scite/compile_flags.txt

exit 0
