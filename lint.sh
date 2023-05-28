#!/usr/bin/env bash
FILE_NAME='*.cxx'
FILTER='*.cxx'

if [ ! -z "$1" ]; then
  FILE_NAME="${1%.cxx}.cxx"
  FILTER="${1%.cxx}.cxx"
fi

echo "Running cppcheck . . . ."
set -x
cppcheck --enable=warning,performance,style,portability --suppressions-list=lexilla/cppcheck.suppress --max-configs=100 \
	-I lexilla/include -I lexilla/access -I lexilla/lexlib -I scintilla/include "-DSTDMETHODIMP_(type) type STDMETHODCALLTYPE" \
	--template=gcc --quiet --platform=native "lexilla/lexers/${FILE_NAME}" | grep -i "${FILTER}"

set +x
if [ "$(command -v clang)" ]; then
  echo
  echo "Linting with Clang-Tidy . . . ."
  set -x
  clang-tidy "lexilla/lexers/${FILE_NAME}" -- \
    -I lexilla/include -I lexilla/access -I lexilla/lexlib -I scintilla/include -Wall -Wextra -pedantic --std=c++17 \
    -fsyntax-only -DNDEBUG -D_CRT_SECURE_NO_DEPRECATE=1
  set +x
fi
