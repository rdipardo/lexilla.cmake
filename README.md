Building lexilla with CMake
===========================

## External dependencies

- [scintilla][]: cross-platform source code editing component: <https://scintilla.org>

- [lexilla][]: (newly independent) library of language lexers for use with
  scintilla: <https://scintilla.org/Lexilla.html>

## Testing locally

**Note.** All compilers must support C++20 features, e.g.
         `std::basic_string<CharT,Traits,Allocator>::ends_with`

Make sure [mercurial](https://www.mercurial-scm.org) is installed.

Clone the source tree and submodules:

    git clone --recursive https://bitbucket.org/rdipardo/lexilla-dev.git

Or, inside your local working tree:

    # if necessary, clean out the submodule path
    # rm -rf ./lexilla
    git submodule sync
    git submodule update --init --remote

Edit the appropriate build script for your build system; for example:

```sh
# build.sh

cmake -Bbuild-gnu -H. -G"Unix Makefiles"

# build all tasks
cmake --build ./build-gnu

# build only the static library
cmake --build ./build-gnu -- lexilla-static
```

```bat
:: build-mingw.bat

:: configure the test program to link statically
cmake -Bbuild-mingw -H. -G"MinGW Makefiles" -DLEXILLA_STATIC=1

:: build and run the test program
cmake --build ./build-mingw -- TestLexers
```

### On Linux or macOS
_Requires g++ 9.x or clang on LLVM 10.x_

    $ ./build.sh

### On Windows
_Requires the [MinGW-w64 toolchain][] or the [Visual Studio 2019 Build Tools][]_

#### MinGW

Run `build-mingw.bat` from a console with `g++` and `mingw32-make` in your `PATH`.

#### NMake

Run `build-nmake.bat` from the [Developer Command Prompt][].


License
=======
The [License.txt][] file describes the conditions under which this software may
be distributed.

[lexilla]: https://github.com/ScintillaOrg/lexilla
[License.txt]: https://github.com/ScintillaOrg/lexilla/blob/master/License.txt
[scintilla]: https://sourceforge.net/p/scintilla/code/ci/default/tree
[MinGW-w64 toolchain]: https://chocolatey.org/packages/mingw
[Visual Studio 2019 Build Tools]: https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=vs-2019
[Developer Command Prompt]: https://docs.microsoft.com/en-us/dotnet/framework/tools/developer-command-prompt-for-vs
