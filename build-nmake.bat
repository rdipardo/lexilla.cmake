@echo off
@rem build and test with NMake
xcopy /SY cmakelists\* lexilla
@rem export undecorated functions when DLL target is 32-bit
echo D | xcopy /SY win32\* lexilla\win32
cd lexilla
cmake -Bbuild-msvc -H. -G"NMake Makefiles" -DDEBUG=1
cmake --build ./build-msvc -- TestLexers
