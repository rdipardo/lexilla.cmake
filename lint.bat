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
	--template=gcc --quiet "lexilla\lexers\%FILE_NAME%" | findstr /C:"%FILTER%"
@echo off
echo.
ENDLOCAL
