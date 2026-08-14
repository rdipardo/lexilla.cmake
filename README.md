Building lexilla with CMake
===========================

## External dependencies

- [scintilla][]: cross-platform source code editing component: <https://scintilla.org>

- [lexilla][]: (newly independent) library of language lexers for use with
  scintilla: <https://scintilla.org/Lexilla.html>

## Testing locally

**Note.** All compilers must support C++20 features, e.g.
         `std::basic_string<CharT,Traits,Allocator>::ends_with`

Clone the source tree and submodules:

    git clone --recursive https://bitbucket.org/rdipardo/lexilla-dev.git

Or, inside your local working tree:

    # if necessary, clean out the submodule path
    # rm -rf ./lexilla
    git submodule sync
    git submodule update --init --remote

Pass the name of a CMake task to the appropriate build script for your environment; for example:

### On Linux or macOS
_Requires g++ 9.x or clang on LLVM 10.x_

To build and run the lexer test driver (the default task):

    $ ./build.sh

To build all tasks:

    $ ./build.sh all

To build only the static library:

    $ ./build.sh lexilla-static

#### MinGW
_Requires the [MinGW-w64 toolchain][]_

To build and run the test program, start a console with `g++` and `mingw32-make`
in your `PATH`, then run

    > build-mingw.bat CheckLexilla

#### NMake
_Requires, at minimum, the [Visual Studio Build Tools][], edition 2019 or later_

From the [Developer Command Prompt][], run

    > build-nmake.bat


License
=======
Except where stated otherwise, all CMake files, build scripts and configurations in this repository
are in the public domain under the terms of the [Creative Commons Zero v1.0 Universal](./LICENSE.txt)
public license.

Scintilla's [License.txt] file describes the conditions under which Scintilla, Lexilla and SciTE may
be distributed.

[lexilla]: https://github.com/ScintillaOrg/lexilla
[License.txt]: https://www.scintilla.org/License.txt
[scintilla]: https://sourceforge.net/p/scintilla/code/ci/default/tree
[MinGW-w64 toolchain]: https://www.mingw-w64.org
[Visual Studio Build Tools]: https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=vs-2019
[Developer Command Prompt]: https://docs.microsoft.com/en-us/dotnet/framework/tools/developer-command-prompt-for-vs
