### lexilla/examples/SimpleLexer/CMakeLists.txt
add_library (SimpleLexer SHARED "SimpleLexer.cxx")
add_dependencies (SimpleLexer lexilla-static)
set_target_properties (PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/examples/SimpleLexer")

if(MSVC)
    target_compile_options (SimpleLexer PRIVATE -EHsc -std:c++17)
    target_link_options (SimpleLexer PRIVATE -INCREMENTAL:NO -LTCG -IGNORE:4075)
else()
    target_compile_options (SimpleLexer PRIVATE
        -fvisibility=hidden -Wno-unused-parameter -Wpedantic --std=c++17)
endif()

target_link_libraries (SimpleLexer PRIVATE lexilla-static)
