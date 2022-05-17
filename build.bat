@echo off
setlocal
SET "PLATFORM=x86"
if not "%1"==[] (SET "PLATFORM=%1")
if not exist scintilla (hg clone http://hg.code.sf.net/p/scintilla/code scintilla)
cd scintilla
hg pull
hg up -C
cd ..

if not exist scite (hg clone http://hg.code.sf.net/p/scintilla/scite)

cd scite\win32
@rem  Make sure to trim any trailing backslash, e.g.:
@rem  echo %WindowsSDKVersion%
@rem  => 10.0.18362.0\
msbuild /nologo /v:m /p:Platform=%PLATFORM%;WindowsTargetPlatformVersion=%WindowsSDKVersion:~0,-1%

cd ..\..
if ("%VSCODE_BUILD%"==[] AND NOT "%PLATFORM%"==[]) (
if "%PLATFORM%"=="x86" (SET PLATFORM=)
scite\win32\%PLATFORM%\Debug\SciTE.exe "%2"
)
endlocal
