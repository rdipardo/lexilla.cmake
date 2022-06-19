@echo off
:: ----------------------------------------------------------------------
:: Demonstrates loading an external lexer using the Lexilla protocol
::
:: See:
::  * https://www.scintilla.org/LexillaDoc.html
::  * https://github.com/ScintillaOrg/lexilla/blob/master/examples/SimpleLexer/SimpleLexer.cxx
::  * https://community.notepad-plus-plus.org/topic/22787/remove-the-external-lexer-support-due-to-scintilla-5/6
::
:: This script:
::  - builds a simple lexer module ('SimpleLexer.dll')
::  - builds SciTE
::  - configures SciTE to load the module
::  - opens and highlights a test file in SciTE
:: ----------------------------------------------------------------------
SETLOCAL

cd ..\lexilla
xcopy /SY ..\cmakelists\* .
echo D | xcopy /SY ..\win32\* .\win32
cmake -Bbuild-msvc -H. -G"NMake Makefiles" -DDEBUG=1
cmake --build ./build-msvc -- SimpleLexer

set "SCI_PROPS=%USERPROFILE%\SciTEUser.properties"
set "EXAMPLE_FILE=%~dp0test.bsc"
if EXIST %SCI_PROPS% (move %SCI_PROPS% %SCI_PROPS%.bak)
:: IMPORTANT
:: The module path must not have ANY trailing spaces; otherwise it won't be found!
echo lexilla.path=.;%~dp0..\lexilla\build-msvc\examples\SimpleLexer\SimpleLexer.dll> %SCI_PROPS%
echo lexer.*.bsc=simple>> %SCI_PROPS%
echo font.base=$(font.monospace)>> %SCI_PROPS%
echo style.simple.1=$(font.base),fore:#FF0000,bold>> %SCI_PROPS%
echo Expect every other character to be red!> %EXAMPLE_FILE%

cd ..
.\build.bat %VSCMD_ARG_TGT_ARCH% %EXAMPLE_FILE%

ENDLOCAL
