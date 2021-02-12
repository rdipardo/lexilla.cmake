@echo off
@rem build and test with NMake
xcopy /SY cmakelists\* lexilla
cd lexilla
cmake -Bbuild-msvc -H. -G"NMake Makefiles"
cmake --build ./build-msvc -- TestLexers
