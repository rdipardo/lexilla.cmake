@echo off
SETLOCAL
SET "BOOTSTRAP_MSVC=C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsDevCmd.bat"
xcopy /SY cmake\* lexilla
cd lexilla

@rem MinGW build
cmake -Bbuild-mingw -H. -G"MinGW Makefiles" -DCMAKE_C_COMPILER=gcc.exe -DCMAKE_CXX_COMPILER=g++.exe
cmake --build ./build-mingw -- TestLexers

@rem MSVC build
CALL "%BOOTSTRAP_MSVC%" &
cmake -Bbuild-msvc -H. -G"NMake Makefiles" -DCMAKE_C_COMPILER=cl.exe -DCMAKE_CXX_COMPILER=cl.exe -DLEXILLA_STATIC=1
cmake --build ./build-msvc -- TestLexers
ENDLOCAL
