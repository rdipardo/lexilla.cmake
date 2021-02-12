### lexilla/test/CMakeLists.txt
add_executable (TestLexers
    "TestLexers.cxx" "TestDocument.cxx" "../access/LexillaAccess.cxx")
set_target_properties (TestLexers PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/test")

if(LEXILLA_STATIC)
    if(MSVC)
        set (LEXILLA_STATIC_LIB "${PROJECT_SOURCE_DIR}/bin/lexilla.lib")
    else()
        set (LEXILLA_STATIC_LIB "${PROJECT_SOURCE_DIR}/bin/liblexilla.a")
    endif()

    add_dependencies (TestLexers lexilla-static)
    target_compile_definitions (TestLexers PRIVATE -DLEXILLA_STATIC=1)
    target_link_libraries (TestLexers PRIVATE ${LEXILLA_STATIC_LIB})
else()
    add_dependencies (TestLexers lexilla)
endif()

if(MSVC)
    target_compile_options (TestLexers PRIVATE -std:c++latest)
else()
    target_compile_options (TestLexers PRIVATE --std=c++2a)

    if(NOT CMAKE_HOST_WIN32)
        target_link_libraries (TestLexers PRIVATE dl)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options (TestLexers PRIVATE -fsanitize=address)
        target_link_libraries (TestLexers PRIVATE -fsanitize=address)
    endif()
endif()

# TODO: have LexillaAccess.cxx load liblexilla.dll when __MINGW32__ is defined
if(MINGW)
    add_custom_command (TARGET TestLexers
                        PRE_LINK
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${PROJECT_SOURCE_DIR}/bin/liblexilla.dll"
                        "${PROJECT_SOURCE_DIR}/bin/lexilla.dll")
endif()

# run the generated binary
add_custom_command (TARGET TestLexers
                    POST_BUILD
                    COMMAND "TestLexers" #works for ELFs and *.exes
                    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/test")
