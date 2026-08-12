cmake_minimum_required(VERSION 3.21)

project(svgparser
    VERSION 0.1.0
    DESCRIPTION "A standalone SVG parser (C++23) producing an abstract syntax tree from SVG documents"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
endif()

# Only default tests ON when svgparser is the top-level build: if another
# project pulls this in via add_subdirectory()/FetchContent, its own `ctest`
# run shouldn't suddenly include svgparser's test suite too.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(SVGPARSER_IS_TOP_LEVEL ON)
else()
    set(SVGPARSER_IS_TOP_LEVEL OFF)
endif()

# --- XML backend selection -------------------------------------------------
# SvgParser currently talks to a Xerces-C DOM directly (src/xml is a thin
# wrapper around it, not yet a parser-agnostic abstraction) -- so today
# Xerces is the only supported backend, and turning it off just means "don't
# build the SVG parser." The option still exists, gated through its own
# find_package()/target_link_libraries() block in src/CMakeLists.txt, so
# that wiring in a second backend later doesn't require restructuring this
# file: it just needs a new option + block alongside this one, and for
# xml/svg to actually branch on which backend is active at the source level.
option(SVGPARSER_WITH_XERCES "Enable the Xerces-C XML backend" ON)
option(SVGPARSER_BUILD_TESTS "Build the Catch2 test suite" ${SVGPARSER_IS_TOP_LEVEL})

if(NOT SVGPARSER_WITH_XERCES)
    message(FATAL_ERROR
        "SVGPARSER_WITH_XERCES=OFF, but no alternative XML backend is implemented yet: "
        "src/xml and src/svg/SvgParser currently include <xercesc/...> directly. "
        "Either leave SVGPARSER_WITH_XERCES=ON, or implement and wire in another backend "
        "before disabling it.")
endif()

add_subdirectory(src)

if(SVGPARSER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
