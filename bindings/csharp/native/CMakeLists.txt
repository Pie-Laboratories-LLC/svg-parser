# bindings/csharp/native/CMakeLists.txt
add_library(svgparser_c SHARED
    svgparser_c_api.cpp
)

target_include_directories(svgparser_c PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(svgparser_c PRIVATE
    svgparser   # your existing static lib target — confirm actual name
    XercesC::XercesC
)

target_compile_definitions(svgparser_c PRIVATE
    SVGPARSER_C_BUILDING
)

# Match whatever standard/coroutine flags your main library uses
target_compile_features(svgparser_c PRIVATE cxx_std_23)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(svgparser_c PRIVATE -fcoroutines)
endif()

# Force a flat, configuration-independent output directory so the C# .csproj's
# hardcoded copy-step path (build/bindings/csharp/native/...) works the same
# way on multi-config generators (Visual Studio) as it does on single-config
# ones (Makefiles/Ninja on Linux/Mac) -- MSVC's generator otherwise nests
# output under a Debug/ or Release/ subdirectory.
set_target_properties(svgparser_c PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bindings/csharp/native"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bindings/csharp/native"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bindings/csharp/native"
)
