### lexilla/CMakeLists.txt
cmake_minimum_required (VERSION 3.13)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR};${CMAKE_MODULE_PATH}")
include(ProjectVersion)
include (ExternalProject)

project (lexilla)

# clones repo when there's no `scintilla` directory on disk;
# otherwise runs `hg pull && hg update`
externalproject_add (
    scintilla
    PREFIX "scintilla"
    HG_REPOSITORY "http://hg.code.sf.net/p/scintilla/code"
    SOURCE_DIR "../../scintilla"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND "" # required if project has no CMakeLists.txt
    TEST_COMMAND ""
    INSTALL_COMMAND "" # might be used to copy headers to system directories
)

# HACK: prevent cmake from automagically populating our flags, even if they've
# been cached already
# https://stackoverflow.com/a/24767451
# https://cmake.org/cmake/help/latest/command/set.html#set-cache-entry
set (CMAKE_C_FLAGS "" CACHE STRING "" FORCE)
set (CMAKE_C_FLAGS_DEBUG "" CACHE STRING "" FORCE)
set (CMAKE_C_FLAGS_RELEASE "" CACHE STRING "" FORCE)
set (CMAKE_C_FLAGS_RELWITHDEBINFO "" CACHE STRING "" FORCE)
set (CMAKE_C_FLAGS_MINSIZEREL "" CACHE STRING "" FORCE)
set (CMAKE_CXX_FLAGS CACHE STRING "" FORCE)
set (CMAKE_CXX_FLAGS_DEBUG CACHE STRING "" FORCE)
set (CMAKE_CXX_FLAGS_RELEASE CACHE STRING "" FORCE)
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO CACHE STRING "" FORCE)
set (CMAKE_CXX_FLAGS_MINSIZEREL CACHE STRING "" FORCE)

set (LEXERS
    ${PROJECT_SOURCE_DIR}/lexers/LexA68k.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexAPDL.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexASY.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexAU3.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexAVE.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexAVS.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexAbaqus.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexAda.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexAsciidoc.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexAsm.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexAsn1.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexBaan.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexBash.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexBasic.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexBatch.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexBibTeX.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexBullant.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexCIL.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexCLW.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexCOBOL.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexCPP.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexCSS.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexCaml.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexCmake.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexCoffeeScript.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexConf.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexCrontab.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexCsound.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexD.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexDMAP.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexDMIS.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexDataflex.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexDiff.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexECL.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexEDIFACT.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexEScript.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexEiffel.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexErlang.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexErrorList.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexFSharp.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexFlagship.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexForth.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexFortran.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexGAP.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexGDScript.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexGui4Cli.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexHTML.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexHaskell.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexHex.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexHollywood.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexIndent.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexInno.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexJSON.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexJulia.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexKVIrc.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexKix.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexLaTeX.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexLisp.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexLout.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexLua.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexMMIXAL.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexMPT.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexMSSQL.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexMagik.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexMake.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexMarkdown.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexMatlab.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexMaxima.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexMetapost.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexModula.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexMySQL.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexNim.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexNimrod.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexNsis.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexNull.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexOScript.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexOpal.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexPB.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexPLM.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexPO.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexPOV.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexPS.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexPascal.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexPerl.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexPowerPro.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexPowerShell.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexProgress.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexProps.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexPython.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexR.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexRaku.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexRebol.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexRegistry.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexRuby.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexRust.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexSAS.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexSML.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexSQL.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexSTTXT.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexScriptol.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexSmalltalk.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexSorcus.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexSpecman.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexSpice.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexStata.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexTACL.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexTADS3.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexTAL.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexTCL.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexTCMD.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexTeX.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexTxt2tags.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexVB.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexVHDL.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexVerilog.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexVisualProlog.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexX12.cxx
    ${PROJECT_SOURCE_DIR}/lexers/LexYAML.cxx)

set (LEXLIB
    ${PROJECT_SOURCE_DIR}/src/Lexilla.cxx
    ${PROJECT_SOURCE_DIR}/lexlib/Accessor.cxx
    ${PROJECT_SOURCE_DIR}/lexlib/CharacterCategory.cxx
    ${PROJECT_SOURCE_DIR}/lexlib/CharacterSet.cxx
    ${PROJECT_SOURCE_DIR}/lexlib/DefaultLexer.cxx
    ${PROJECT_SOURCE_DIR}/lexlib/LexAccessor.cxx
    ${PROJECT_SOURCE_DIR}/lexlib/LexerBase.cxx
    ${PROJECT_SOURCE_DIR}/lexlib/LexerModule.cxx
    ${PROJECT_SOURCE_DIR}/lexlib/LexerNoExceptions.cxx
    ${PROJECT_SOURCE_DIR}/lexlib/LexerSimple.cxx
    ${PROJECT_SOURCE_DIR}/lexlib/PropSetSimple.cxx
    ${PROJECT_SOURCE_DIR}/lexlib/StyleContext.cxx
    ${PROJECT_SOURCE_DIR}/lexlib/WordList.cxx)

include_directories ("${PROJECT_SOURCE_DIR}/include"
                     "../scintilla/include"
                     "${PROJECT_SOURCE_DIR}/lexlib"
                     "${PROJECT_SOURCE_DIR}/access")

if(MSVC)
    set (SUBSYSTEM "")
    set (CETCOMPAT "")

    if(("$ENV{PLATFORM}" MATCHES "ARM") OR ("$ENV{VSCMD_ARG_TGT_ARCH}" MATCHES "ARM"))
      set (ARM64 "ON" CACHE INTERNAL "Target platform is arm64")
    endif()

    add_definitions (
        -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1
        -D_CRT_SECURE_NO_DEPRECATE=1
        -D_CRT_SECURE_NO_WARNINGS=1
    )
    add_compile_options (
        -Zi
        -TP
        -MP
        -W4
    )

    if(SUPPORT_XP)
        add_definitions (-D_USING_V110_SDK71_)
      if(PLATFORM MATCHES "x64")
        set (SUBSYSTEM "-SUBSYSTEM:WINDOWS,5.02")
      else()
        set (SUBSYSTEM "-SUBSYSTEM:WINDOWS,5.01")
      endif()
    # https://sourceforge.net/p/scintilla/bugs/2324
    elseif(NOT ARM64 AND $ENV{WindowsSDKVersion} VERSION_GREATER_EQUAL "10.0.18362.0")
        message (STATUS "Enabling control-flow enforcement")
        message (STATUS "See: https://docs.microsoft.com/en-us/cpp/build/reference/cetcompat")
        set (CETCOMPAT "-CETCOMPAT")
    elseif(ARM64)
        add_definitions (-D_ARM64_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE=1)
        set (SUBSYSTEM "-SUBSYSTEM:WINDOWS,10.00")
    endif()

    set (LDFLAGS_BASE "-IGNORE:4197;${SUBSYSTEM}")

    if(QUIET)
        add_compile_options (-nologo)
        set (LDFLAGS_BASE "${LDFLAGS_BASE};-nologo")
    endif()

    set (LDFLAGS "-OPT:REF;-LTCG;-IGNORE:4075;${LDFLAGS_BASE};${CETCOMPAT}")
    string (REPLACE ";" " " EXE_FLAGS "${CMAKE_EXE_LINKER_FLAGS};${LDFLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS ${EXE_FLAGS})

    if(DEBUG)
        add_compile_options (-Od -MTd -DDEBUG)
        set (LDFLAGS "${LDFLAGS};-DEBUG")
    else()
        add_compile_options (-O1 -MT -DNDEBUG -GL)
    endif()

# NOTE: MinGW will cause CMAKE_CXX_COMPILER_ID to return "GNU"
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR
        CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # -fPIC is automagically added if the SHARED option is passed to
    # add_library() (see below); ditto the -dynamiclib flag
    add_compile_options (-Wall -Wextra)
    if(DEBUG)
        add_definitions (-DDEBUG)
        add_compile_options (-g)
    else()
        add_definitions (-DNDEBUG)
        add_compile_options (-Os)
    endif()
endif()

# declare build targets
add_library (lexilla SHARED ${LEXLIB} ${LEXERS})
add_library (lexilla-static STATIC ${LEXLIB} ${LEXERS})

if(MSVC)
    # flags that were set with `add_compile_options()` will propagate to every
    # build target; use `target_compile_options()` to control scope
    target_compile_options (lexilla PRIVATE -EHsc -std:c++17)
    target_compile_options (lexilla-static PRIVATE -EHsc -std:c++17)
    # See first "Note" at https://cmake.org/cmake/help/latest/command/target_link_options.html
    if(LEXILLA_STATIC)
        string(REPLACE ";" " " LIB_FLAGS "${LDFLAGS_BASE}")
        set_target_properties (lexilla-static PROPERTIES
            STATIC_LIBRARY_FLAGS ${LIB_FLAGS})
    else()
        if("$ENV{VSCMD_ARG_TGT_ARCH}" MATCHES "^x86")
            set (LDFLAGS "${LDFLAGS};-def:${PROJECT_SOURCE_DIR}/win32/lexilla.def")
        endif()
        target_link_options (lexilla PRIVATE ${LDFLAGS})
    endif()
else()
    target_compile_options (lexilla PRIVATE
        -fvisibility=hidden -Wpedantic --std=c++17)
    target_compile_options (lexilla-static PRIVATE
        -fvisibility=hidden -Wpedantic --std=c++17)
    if(MINGW)
        target_link_options (lexilla PRIVATE "-mwindows;-Wl,--kill-at")
    endif()
endif()

set_target_properties (lexilla lexilla-static PROPERTIES
    # CXX_STANDARD 17 # convenient, but defaults to gnu++17 whenever it can
    OUTPUT_NAME lexilla # the lib- prefix will be added by gcc/clang and MinGW
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# don't overwrite the dll import lib with the identically named static lib
if(MSVC)
     set_target_properties (lexilla-static PROPERTIES
         ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/win32/lib")
endif()

# the first target depends on the second
add_dependencies (lexilla scintilla)
add_dependencies (lexilla-static scintilla)
# build and run test applications
add_subdirectory ("${PROJECT_SOURCE_DIR}/test")
add_subdirectory ("${PROJECT_SOURCE_DIR}/examples/SimpleLexer")
add_subdirectory ("${PROJECT_SOURCE_DIR}/examples/CheckLexilla")
