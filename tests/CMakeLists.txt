# Prefer a Catch2 already available on the system (apt/vcpkg/brew/etc); fall
# back to fetching a pinned release so a clean checkout still builds with
# nothing pre-installed.
find_package(Catch2 3 QUIET)
if(NOT Catch2_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.7.1
    )
    FetchContent_MakeAvailable(Catch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
else()
    list(APPEND CMAKE_MODULE_PATH ${Catch2_DIR})
endif()

add_executable(svgparser_tests
    main.cpp
    svg/Path-tests.cpp
    svg/SvgDParser-tests.cpp
    svg/SvgParser-tests.cpp
)

target_include_directories(svgparser_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# Own main.cpp (not Catch2::Catch2WithMain) so it can bracket the Catch2
# session with XMLPlatformUtils::Initialize()/Terminate() -- required
# before SvgParser touches Xerces at all, and easy to miss since nothing
# in the library calls it lazily on your behalf.
target_link_libraries(svgparser_tests PRIVATE svgparser::svgparser Catch2::Catch2)

include(Catch)
catch_discover_tests(svgparser_tests)
