### CMakeLists.txt
cmake_minimum_required (VERSION 3.14)
project (scite
        DESCRIPTION "Stub project to fetch and build SciTE's dependencies"
        LANGUAGES NONE)
if(NOT CMAKE_HOST_WIN32)
  add_subdirectory(lexilla EXCLUDE_FROM_ALL)
  add_custom_target (scintilla
                    COMMAND DEBUG=1 GTK3=1 make -j8
                    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/scintilla/gtk"
                    COMMENT "Building Scintilla..."
                    VERBATIM)
  add_dependencies (scintilla lexilla)
endif()
