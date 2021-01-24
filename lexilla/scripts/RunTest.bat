@echo off
@Rem Test lexers
@Rem build lexilla.dll and TestLexers.exe then run TestLexers.exe
cd ..\src
nmake /nologo -f lexilla.mak clean
nmake /nologo -f lexilla.mak
cd ..\test
nmake /nologo -f testlexers.mak clean
nmake /nologo -f testlexers.mak
nmake /nologo -f testlexers.mak test
cd ..\scripts
