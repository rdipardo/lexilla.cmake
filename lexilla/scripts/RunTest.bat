@echo off
@Rem Test lexers
@Rem build lexilla.dll and TestLexers.exe then run TestLexers.exe
SET "PRE_BUILD_CMD=date"

IF NOT [%CI%] == [] (
    SET "PRE_BUILD_CMD=C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"
)

echo "Testing with NMake . . . ."
call "%PRE_BUILD_CMD%" &

cd ..\src
nmake /nologo -f lexilla.mak clean
nmake /nologo -f lexilla.mak
cd ..\test
nmake /nologo -f testlexers.mak clean
nmake /nologo -f testlexers.mak
nmake /nologo -f testlexers.mak test
cd ..\scripts
