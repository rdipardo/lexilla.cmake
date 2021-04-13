@echo off
@rem build and test with NMake
xcopy /SY cmakelists\* lexilla
cd lexilla
@rem build statically to work around CMake directory resolution issues 
@rem and the resulting 1 exit code
cmake -Bbuild-msvc -H. -G"NMake Makefiles" -DLEXILLA_STATIC=1
cmake --build ./build-msvc -- TestLexers
