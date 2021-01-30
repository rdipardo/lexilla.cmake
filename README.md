Lexer Development Demo for `lexilla`
====================================

[![Build Status][travis-ci]][Build Status]

## Purpose

Demonstrate a lexer development workflow targeting scintilla 5.x

## Source origin

This project combines code from the (actively changing) [lexilla][] and
(stable) [scintilla][] code bases.

## Testing locally

**Note.** All compilers must support C++20 features, e.g.
`std::basic_string<CharT,Traits,Allocator>::ends_with`

### Unix `make`
_Requires g++ 9.x or clang on LLVM 10.x_

    $ cd lexilla/scripts
    $ ./RunTest.sh

### MSVC
_Requires Visual Studio 2019 or, at minimum, the
[Visual Studio 2019 Build Tools][]_

From the [Developer Command Prompt][]:

#### NMake

    > cd lexilla\scripts
    > RunTest

#### MSBuild

    > cd lexilla\src
    > nmake /nologo -f lexilla.mak
    > cd ..
    > msbuild /nologo /v:m /p:Configuration=Release /t:Rebuild test\TestLexers.vcxproj
    > test\Release\TestLexers.exe


License
=======
The [scintilla/License.txt](scintilla/License.txt) and
[lexilla/License.txt](lexilla/License.txt) files describe the conditions under
which this software may be distributed.

[lexilla]: https://github.com/ScintillaOrg/lexilla
[scintilla]: https://sourceforge.net/p/scintilla/code/ci/default/tree
[Visual Studio 2019 Build Tools]: https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=vs-2019
[Developer Command Prompt]: https://docs.microsoft.com/en-us/dotnet/framework/tools/developer-command-prompt-for-vs
[travis-ci]: https://travis-ci.com/rdipardo/lexilla-dev.svg?branch=master
[Build Status]: https://travis-ci.com/bitbucket/rdipardo/lexilla-dev
