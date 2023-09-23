@echo off
SETLOCAL
SET TASK=TestLexers
IF NOT "%1"=="" (SET TASK=%1)
xcopy /SY cmakelists\* lexilla
xcopy /DIY .gitignore lexilla
@rem export undecorated functions when DLL target is 32-bit
echo D | xcopy /SY win32\* lexilla\win32
cd lexilla
cmake -Bbuild-msvc -H. -G"NMake Makefiles" -DDEBUG=1
cmake --build ./build-msvc -- %TASK%
ENDLOCAL
