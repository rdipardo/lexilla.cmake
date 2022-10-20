@echo off
setlocal
SET "PLATFORM=x86"
if "%1" NEQ "" (SET "PLATFORM=%1")
if not exist scintilla (hg clone http://hg.code.sf.net/p/scintilla/code scintilla)
pushd scintilla
hg pull
hg up -C
popd

if not exist scite (hg clone http://hg.code.sf.net/p/scintilla/scite)

pushd scite\win32
@rem  Make sure to trim any trailing backslash, e.g.:
@rem  echo %WindowsSDKVersion%
@rem  => 10.0.18362.0\
msbuild /nologo /v:m /p:Platform=%PLATFORM%;WindowsTargetPlatformVersion=%WindowsSDKVersion:~0,-1%
popd

if "%VSCODE_BUILD%"=="" goto :Launch

exit /b 0

:Launch
if "%PLATFORM%"=="x86" (SET PLATFORM=)
scite\win32\%PLATFORM%\Debug\SciTE.exe "%2"
endlocal
