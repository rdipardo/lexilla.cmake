@echo off
SETLOCAL
SET "BOOTSTRAP_MSVC=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
SET "FILE_NAME=*.cxx"
SET "FILTER=cxx"
SET LINTER_ARG=

@rem optionally filter results to a single TU
IF NOT "%1"=="" (
  SET "FILE_NAME=%~n1.cxx"
  SET "FILTER=%~n1.cxx"
  SET "LINTER_ARG=%~n1.cxx"
)

CALL .\lint.bat %LINTER_ARG%

DIR /B "%BOOTSTRAP_MSVC%" 2>NUL
IF NOT %ERRORLEVEL% == 0 (GOTO END)

CALL "%BOOTSTRAP_MSVC%" &
echo.
echo "Analyzing . . . ."
echo.
cd lexilla
@echo on
cl /analyze /I..\scintilla\include /Ilexlib /Iinclude /std:c++17 /nologo /Zi /TP /MP ^
  /Wall /wd4820 /wd4365 /wd4514 /wd4625 /wd4626 /wd4710 /wd4996 /wd5204 /EHsc /O1 /MT /DNDEBUG /GL ^
  /c "lexers\%FILE_NAME%" | findstr /C:"%FILTER%"
@echo off
cd ..

:END
ENDLOCAL
