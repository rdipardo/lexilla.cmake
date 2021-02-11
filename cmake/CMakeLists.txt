### lexilla/CMakeLists.txt
cmake_minimum_required (VERSION 3.2)
include (ExternalProject)

project (lexilla VERSION "5.0.0")

# clones repo if no there's "scintilla" directory on disk;
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

# HACK: prevent cmake from automagically populating our CXX_FLAGS
# https://stackoverflow.com/a/24767451
set (CMAKE_CXX_FLAGS "")
set (CMAKE_CXX_FLAGS_DEBUG "")
set (CMAKE_CXX_FLAGS_RELEASE "")
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "")
set (CMAKE_CXX_FLAGS_MINSIZEREL "")

file (GLOB LEXILLA_SRC "${PROJECT_SOURCE_DIR}/src/*.cxx"
                       "${PROJECT_SOURCE_DIR}/lexers/*.cxx"
                       "${PROJECT_SOURCE_DIR}/lexlib/*.cxx")

include_directories ("${PROJECT_SOURCE_DIR}/src"
                     "${PROJECT_SOURCE_DIR}/lexlib"
                     "${PROJECT_SOURCE_DIR}/include"
                     "../scintilla/include")

if(MSVC)
    set (SUBSYSTEM "")
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
        -EHsc
        -std:c++17
    )

    if(PLATFORM MATCHES "x64")
        set (SUBSYSTEM "-SUBSYSTEM:WINDOWS,5.02")
    elseif(ARM64)
        add_definitions (-D_ARM64_WINAPI_PARTITION_DESKTOP_SDK_AVAILABLE=1)
        set (SUBSYSTEM "-SUBSYSTEM:WINDOWS,10.00")
    else()
        set (SUBSYSTEM "-SUBSYSTEM:WINDOWS,5.01")
    endif()

    if(SUPPORT_XP)
        add_definitions (-D_USING_V110_SDK71_)
    endif()

    if(DEBUG)
        add_compile_options (-Od -MTd -DDEBUG)
    else()
        set (LDFLAGS_BASE "-LTCG -IGNORE:4197")
        set (LDFLAGS "-OPT:REF -INCREMENTAL:NO -DEBUG ${LDFLAGS_BASE}")
        add_compile_options (-O1 -MT -DNDEBUG -GL)
        set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LDFLAGS} ${SUBSYSTEM}")
        set (CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} ${LDFLAGS_BASE}")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS} -IGNORE:4075")
    endif()

    if(QUIET)
        add_compile_options (-nologo)
        set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -nologo")
        set (CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -nologo")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nologo")
    endif()
endif()

# NOTE: MinGW will cause CMAKE_CXX_COMPILER_ID to return "GNU"
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR
   CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # -fPIC is automagically added if the SHARED option is passed to
    # add_library() (see below); ditto the -dynamiclib flag
    add_compile_options (
        -fvisibility=hidden
        -Wall
        -Wextra
        -Wpedantic
        --std=c++17
    )

    if(MINGW)
        add_compile_options (-mwindows)
    endif()

    if(DEBUG)
        add_definitions (-DDEBUG)
        add_compile_options (-g)
    else()
        add_definitions (-DNDEBUG)
        add_compile_options (-Os)
    endif()
endif()

# declare build targets
add_library (lexilla SHARED ${LEXILLA_SRC})
add_library (lexilla-static STATIC ${LEXILLA_SRC})
set_target_properties (lexilla lexilla-static PROPERTIES
    # gcc/clang generates a versioned binary and canonical symlink to the same
    # whenever VERSION is defined
    VERSION ${PROJECT_VERSION}
    # CXX_STANDARD 17 # convenient, but defaults to gnu++17 whenever it can
    OUTPUT_NAME lexilla # the lib- prefix will be added by gcc/clang and MinGW
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
# the first target depends on the second
add_dependencies (lexilla scintilla)
add_dependencies (lexilla-static scintilla)
# build and run test application
add_subdirectory ("${PROJECT_SOURCE_DIR}/test")
