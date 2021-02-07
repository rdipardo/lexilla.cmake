@echo off
@Rem Test lexers
@Rem build lexilla.dll and TestLexers.exe then run TestLexers.exe
SETLOCAL

SET "PRE_BUILD_CMD=C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"

IF [%LEXER_SRC_FILE%]==[] SET "LEXER_SRC_FILE=Lex*.cxx"
IF [%CI%] NEQ [] CALL "%PRE_BUILD_CMD%" &

echo.
echo "Analyzing . . . ."
echo.
@echo on
cl /analyze /I..\..\scintilla\include /I..\lexlib /I..\include /std:c++17 /nologo /Zi /TP /MP /Wall /wd4820 ^
 /EHsc /O1 /MT /DNDEBUG /GL /c "..\lexers\%LEXER_SRC_FILE%" | findstr %LEXER_SRC_FILE%
@echo off
echo.
echo "Testing with NMake . . . ."
echo.

cd ..\src
nmake /nologo -f lexilla.mak clean
nmake /nologo -f lexilla.mak
cd ..\test
nmake /nologo -f testlexers.mak clean
nmake /nologo -f testlexers.mak
nmake /nologo -f testlexers.mak test
cd ..\scripts

ENDLOCAL