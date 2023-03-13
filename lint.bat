@echo off
SETLOCAL
SET "FILE_NAME=*.cxx"
SET "FILTER=cxx"
IF NOT "%1"=="" (
  SET "FILE_NAME=%~n1.cxx"
  SET "FILTER=%~n1.cxx"
)

echo.
echo "Running cppcheck . . . ."

@echo on
cppcheck --enable=warning,performance,style,portability --suppressions-list=lexilla/cppcheck.suppress --max-configs=100 ^
	-I lexilla/include -I lexilla/access -I lexilla/lexlib -I scintilla/include "-DSTDMETHODIMP_(type) type STDMETHODCALLTYPE" ^
	--template=gcc --quiet --platform=native "lexilla\lexers\%FILE_NAME%" | findstr /C:"%FILTER%"
@echo off
echo.

where clang 2>NUL: 1>NUL:
IF %ERRORLEVEL%==0 (
  @echo "Linting with Clang-Tidy . . . ."
  @echo.
  clang-tidy "lexilla/lexers/%FILE_NAME%" --config="" -- ^
    -I lexilla/include -I lexilla/access -I lexilla/lexlib -I scintilla/include -Wall -Wextra -pedantic --std=c++17 ^
    -fsyntax-only -DNDEBUG -D_CRT_SECURE_NO_DEPRECATE=1
)
ENDLOCAL
