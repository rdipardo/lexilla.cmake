Building SciTE with lexilla
==============================

## External dependencies

- [SciTE][]: free source code editor for Win32, macOS and GNU/Linux <https://scintilla.org/SciTE.html>

- [scintilla][]: cross-platform source code editing component: <https://scintilla.org>

- [lexilla][]: (newly independent) library of language lexers for use with
  scintilla: <https://scintilla.org/Lexilla.html>

## Testing locally

**Note.** All compilers must support C++17 features

Clone this repository and submodules:

    git clone --recursive --branch scite-gtk3 https://github.com/rdipardo/lexilla.cmake.git

Or, inside your local working tree:

    git checkout scite-gtk3
    # if necessary, clean out the submodule path
    # rm -rf ./lexilla
    git submodule sync
    git submodule update --init --remote

CMake will fetch the SciTE source code when ``build.sh`` or ``build.bat`` is run.

Your working tree should now look like this:

    ├── analyze.bat
    ├── build.bat
    ├── build-mingw.bat
    ├── build-nmake.bat
    ├── build.sh
    ├── cmakelists
    ├── CMakeLists.txt
    ├── compile_flags_scite.txt
    ├── compile_flags.txt
    ├── demo
    ├── lexilla
    ├── lint.bat
    ├── lint.sh
    ├── README.md
    ├── scintilla
    ├── scite
    └── win32

### GNU make
_Requires any 3.x version of the [GTK+ development libraries][]_

    $ sudo apt-get update
    $ sudo apt-get install libgtk-3-dev

To build all libraries and the SciTE text editor:

    $ ./build.sh

### MSBuild
_Requires Visual Studio >= 2017 or, at minimum, the [Visual Studio Build Tools][]_

    > build.bat

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
[SciTE]: https://sourceforge.net/p/scintilla/scite/ci/default/tree
[GTK+ development libraries]: https://packages.debian.org/stable/libgtk-3-dev
[Visual Studio Build Tools]: https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=vs-2019
