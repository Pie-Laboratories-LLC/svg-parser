add_library(svgparser
    core/LogLevel.cpp
    core/Logger.cpp
    core/StdNumeric.cpp
    core/String.cpp
    data-structures/tree/GeneralTree.cpp
    draw2d/GradientUnits.cpp
    draw2d/Point.cpp
    draw2d/Poly.cpp
    draw2d/SpreadMethod.cpp
    draw2d/StandardColours.cpp
    svg/DimensionUnits.cpp
    svg/FillRule.cpp
    svg/GradientTemplate.cpp
    svg/Group.cpp
    svg/LengthUnits.cpp
    svg/LineCap.cpp
    svg/LineJoin.cpp
    svg/Path.cpp
    svg/PathMove.cpp
    svg/PreserveAspectRatio.cpp
    svg/Svg.cpp
    svg/SvgColour.cpp
    svg/SvgColourType.cpp
    svg/SvgDParser.cpp
    svg/SvgDimensionedEntity.cpp
    svg/SvgEntity.cpp
    svg/SvgPaint.cpp
    svg/SvgParser.cpp
    svg/SvgParserContext.cpp
    svg/SvgRendererContext.cpp
)
add_library(svgparser::svgparser ALIAS svgparser)

target_compile_features(svgparser PUBLIC cxx_std_23)

# All internal headers are quoted relative to src/ itself (e.g.
# #include "svg/SvgParser.hpp", #include "core/String.hpp"), so that's the
# one include root both this target and its consumers need.
target_include_directories(svgparser PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# core/Exception.hpp captures a std::stacktrace on construction; GCC ships
# that behind its separate "experimental" static lib rather than baking it
# into libstdc++ proper. Public because the capture happens inline in the
# header, so any consumer TU that throws/constructs one needs it too.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(svgparser PUBLIC stdc++exp)
endif()

if(SVGPARSER_WITH_XERCES)
    find_package(XercesC REQUIRED)

    target_sources(svgparser PRIVATE
        xml/DomParser.cpp
        xml/String.cpp
        xml/XmlExceptionType.cpp
    )
    target_link_libraries(svgparser PUBLIC XercesC::XercesC)
    target_compile_definitions(svgparser PUBLIC SVGPARSER_WITH_XERCES=1)
endif()

install(TARGETS svgparser
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)
