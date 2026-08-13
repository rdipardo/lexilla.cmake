### CMakeLists.txt
cmake_minimum_required (VERSION 3.14)
include (cmakelists/get_cpm.cmake)
project (scite
        DESCRIPTION "Stub project to fetch and build SciTE's dependencies"
        LANGUAGES NONE)
CPMAddPackage (
  NAME scite-src
  VERSION 5.6.6
  URL https://www.scintilla.org/scite566.zip
  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/scite"
  DOWNLOAD_ONLY YES
)
add_subdirectory(lexilla EXCLUDE_FROM_ALL)
if(NOT CMAKE_HOST_WIN32)
  add_custom_target (scintilla
                    COMMAND DEBUG=1 GTK3=1 make -j8
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/scintilla/gtk"
                    COMMENT "Building Scintilla..."
                    VERBATIM)
  add_dependencies (scintilla lexilla)
endif()
