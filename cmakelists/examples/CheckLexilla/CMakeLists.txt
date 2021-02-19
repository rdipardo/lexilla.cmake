### lexilla/examples/CheckLexilla/CMakeLists.txt
add_executable (CheckLexilla "CheckLexilla.c")
add_dependencies (CheckLexilla SimpleLexer)
set_target_properties (CheckLexilla PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/examples/CheckLexilla")

if(MSVC)
    # https://discourse.cmake.org/t/msvc-flag-tp-is-set-on-a-c-target-when-added-as-a-subdirectory/680
    target_compile_options(CheckLexilla PRIVATE -TC)
    
    if(DEBUG)
        target_compile_options(CheckLexilla PRIVATE -Od -MTd -DDEBUG)
    else()
        target_compile_options(CheckLexilla PRIVATE -O1 -MT -DNDEBUG -GL)
    endif()
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    find_program(VALGRIND "valgrind")
    target_compile_options (CheckLexilla PRIVATE -Wno-format)
    # https://github.com/google/sanitizers/issues/856#issuecomment-727254300
    if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND NOT VALGRIND)
        target_compile_options (CheckLexilla PRIVATE -fsanitize=address)
        target_link_libraries (CheckLexilla PRIVATE -fsanitize=address)
    else()
        target_compile_options (CheckLexilla PRIVATE -Wno-cast-function-type)
    endif()
endif()

if(NOT CMAKE_HOST_WIN32)
    target_link_libraries (CheckLexilla PRIVATE dl)
endif()

if(MINGW)
    add_custom_command (TARGET CheckLexilla
                        PRE_LINK
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${PROJECT_SOURCE_DIR}/bin/liblexilla.dll"
                        "${PROJECT_SOURCE_DIR}/bin/lexilla.dll")
endif()

if(VALGRIND)
    list (APPEND MEM_CHECK_ARGS
        --leak-check=full --show-reachable=yes
        --suppressions=valgrind.suppress
        --log-file=CheckLexilla.log)
    list (APPEND CHECKER "${VALGRIND}" ${MEM_CHECK_ARGS})
    list (APPEND SHOW_LOG cat ./CheckLexilla.log 2>/dev/null)
else()
    set(CHECKER )
    set(SHOW_LOG )
endif()

add_custom_command (TARGET CheckLexilla
    POST_BUILD
    COMMAND ${CHECKER} "$<TARGET_FILE:CheckLexilla>" "$<TARGET_FILE:SimpleLexer>"
    COMMAND ${SHOW_LOG}
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:CheckLexilla>")
