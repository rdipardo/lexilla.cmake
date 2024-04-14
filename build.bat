@echo off
setlocal
SET "PLATFORM=x86"
if "%1" NEQ "" (SET "PLATFORM=%1")

cmake -B.cmake-win -H.
cmake --build .cmake-win
pushd scite\win32
@rem  Make sure to trim any trailing backslash, e.g.:
@rem  echo %WindowsSDKVersion%
@rem  => 10.0.18362.0\
msbuild /nologo /v:m /p:PlatformToolSet=v143;Platform=%PLATFORM%;WindowsTargetPlatformVersion=%WindowsSDKVersion:~0,-1%
popd

if "%VSCODE_BUILD%"=="" goto :Launch

exit /b 0

:Launch
if "%PLATFORM%"=="x86" (SET PLATFORM=)
scite\win32\%PLATFORM%\Debug\SciTE.exe "%2"
endlocal
