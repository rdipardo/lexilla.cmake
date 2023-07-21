### lexilla/examples/SimpleLexer/CMakeLists.txt
add_library (SimpleLexer SHARED "SimpleLexer.cxx")
set_target_properties (PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/examples/SimpleLexer")

if(MSVC)
    target_compile_options (SimpleLexer PRIVATE -EHsc -std:c++17)
    target_link_options (SimpleLexer PRIVATE -INCREMENTAL:NO -LTCG -IGNORE:4075)
    if("$ENV{VSCMD_ARG_TGT_ARCH}" MATCHES "^x86")
        target_link_options (SimpleLexer PRIVATE -def:${PROJECT_SOURCE_DIR}/win32/SimpleLexer.def)
    endif()
else()
    target_compile_options (SimpleLexer PRIVATE
        -fvisibility=hidden -Wno-unused-parameter -Wpedantic --std=c++17)
endif()

if(CMAKE_HOST_WIN32)
    target_link_libraries (SimpleLexer PRIVATE lexilla-static)
else()
    target_link_libraries (SimpleLexer PRIVATE lexilla)
endif()
