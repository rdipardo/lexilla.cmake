@echo off
SETLOCAL
SET TASK=TestLexers
IF NOT "%1"=="" (SET TASK=%1)
xcopy /SY cmakelists\* lexilla
cd lexilla
cmake -Bbuild-mingw -H. -G"MinGW Makefiles" -DDEBUG=1
cmake --build ./build-mingw -- %TASK%
ENDLOCAL
