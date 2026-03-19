### lexilla/test/Metadata/CMakeLists.txt
add_executable (metadata
    "Metadata.cxx" "${PROJECT_SOURCE_DIR}/access/LexillaAccess.cxx")
add_dependencies (metadata lexilla)
set_target_properties (metadata PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/test/Metadata")
if(MSVC)
    target_compile_options (metadata PRIVATE -EHsc -std:c++17)
else()
    target_compile_options (metadata PRIVATE --std=c++17)
    if(NOT CMAKE_HOST_WIN32)
        target_link_libraries (metadata PRIVATE dl)
    endif()
endif()
