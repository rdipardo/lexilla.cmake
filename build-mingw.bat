@echo off
@rem build and test with MinGW-w64
xcopy /SY cmakelists\* lexilla
cd lexilla
cmake -Bbuild-mingw -H. -G"MinGW Makefiles"
cmake --build ./build-mingw -- TestLexers
