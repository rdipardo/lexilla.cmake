########
lexilla
########

Demonstrates a lexer development workflow targeting scintilla 5.x


Source origin
===============

This project combines code from the (actively changing) `lexilla`_ and
(stable) `scintilla`_ code bases.


Testing locally
===============

**Note.** All compilers must support C++20 features, e.g.
``std::basic_string<CharT,Traits,Allocator>::ends_with``


Unix make
---------

Requires:

- g++ 9.x or clang with LLVM 10.x

.. code-block:: shell

    $ cd lexilla/scripts
    $ ./RunTest.sh


MSVC
----

Requires:

- `Visual Studio 2019 Build Tools`_

From the `Developer Command Prompt`_:


nmake
^^^^^

.. code-block:: doscon

    > cd lexilla\scripts
    > RunTest


MsBuild
^^^^^^^

.. code-block:: doscon

    > cd lexilla
    > msbuild /nologo /v:m src\Lexilla.vcxproj
    > msbuild /nologo /v:m /p:Configuration=Release test\TestLexers.vcxproj
    > test\Release\TestLexers.exe


License
=======
The `License.txt <scintilla/License.txt>`_ file describes the conditions
under which the scintilla and lexilla source code may be distributed.

.. _`lexilla`: https://github.com/ScintillaOrg/lexillatest5
.. _`scintilla`: https://sourceforge.net/p/scintilla/code/ci/default/tree
.. _`Visual Studio 2019 Build Tools`: https://docs.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=vs-2019
.. _`Developer Command Prompt`: https://docs.microsoft.com/en-us/dotnet/framework/tools/developer-command-prompt-for-vs
