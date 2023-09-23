@echo off
SETLOCAL
SET TASK=TestLexers
IF NOT "%1"=="" (SET TASK=%1)
xcopy /SY cmakelists\* lexilla
xcopy /DIY .gitignore lexilla
cd lexilla
cmake -Bbuild-mingw -H. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G"MinGW Makefiles" -DDEBUG=1
cmake --build ./build-mingw -- %TASK%
ENDLOCAL
