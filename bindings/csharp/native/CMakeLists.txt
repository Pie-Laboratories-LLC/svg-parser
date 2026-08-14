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
