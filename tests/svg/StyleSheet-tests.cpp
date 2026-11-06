/*
 * Copyright 2026 Pie Laboratories
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <catch2/catch_test_macros.hpp>

#ifndef DRAW2D_SVG_STYLESHEET_DOT_HPP
    #include "svg/StyleSheet.hpp"
#endif

using Draw2d::Svg::StyleSheet;

namespace {
    Core::String Value(const std::unordered_map<Core::String, Core::String> &values, const char *pszProperty)
    {
        auto it = values.find(pszProperty);
        return it == values.end() ? Core::String { "<unset>" } : it->second;
    }
}

TEST_CASE("StyleSheet: class rules, grouped selectors and comments", "[css][svg]") {
    StyleSheet styleSheet {};
    styleSheet.append(R"css(
        /* Illustrator-style output */
        .cls-1 { fill: #af7140; }
        .cls-37, .cls-38, .cls-39 { opacity: .6; }
    )css");
    REQUIRE( styleSheet.getRuleCount() == 4 );
    REQUIRE( Value(styleSheet.cascade("path", "", { "cls-1" }), "fill") == "#af7140" );
    REQUIRE( Value(styleSheet.cascade("g", "", { "cls-38" }), "opacity") == ".6" );
    REQUIRE( styleSheet.cascade("path", "", { "cls-2" }).empty() );
}

TEST_CASE("StyleSheet: specificity, then source order", "[css][svg]") {
    StyleSheet styleSheet {};
    styleSheet.append(R"css(
        #logo { fill: red; }
        path.a { fill: orange; }
        .a { fill: green; stroke: green; }
        path { fill: blue; stroke: blue; }
        .b { stroke: black; }
        .c { stroke: white; }
    )css");
    // id beats type+class beats class beats type, whatever the order
    REQUIRE( Value(styleSheet.cascade("path", "logo", { "a" }), "fill") == "red" );
    REQUIRE( Value(styleSheet.cascade("path", "", { "a" }), "fill") == "orange" );
    REQUIRE( Value(styleSheet.cascade("circle", "", { "a" }), "fill") == "green" );
    REQUIRE( Value(styleSheet.cascade("path", "", {}), "fill") == "blue" );
    // equal specificity: the later rule wins
    REQUIRE( Value(styleSheet.cascade("path", "", { "c", "b" }), "stroke") == "white" );
}

TEST_CASE("StyleSheet: compound selectors need every part", "[css][svg]") {
    StyleSheet styleSheet {};
    styleSheet.append("path.a.b { fill: red; } *.c { fill: blue; } * { stroke: green; }");
    REQUIRE( Value(styleSheet.cascade("path", "", { "a", "b" }), "fill") == "red" );
    REQUIRE( Value(styleSheet.cascade("path", "", { "a" }), "fill") == "<unset>" );
    REQUIRE( Value(styleSheet.cascade("rect", "", { "a", "b" }), "fill") == "<unset>" );
    REQUIRE( Value(styleSheet.cascade("rect", "", { "c" }), "fill") == "blue" );
    REQUIRE( Value(styleSheet.cascade("rect", "", {}), "stroke") == "green" );
}

TEST_CASE("StyleSheet: !important and the style attribute", "[css][svg]") {
    StyleSheet styleSheet {};
    styleSheet.append("#x { fill: red; } .a { stroke: blue !important; } .a { opacity: .5 ! IMPORTANT; }");
    auto inlineDeclarations = StyleSheet::ParseDeclarations("fill: green; stroke: yellow; opacity: 1 !important");
    auto values = styleSheet.cascade("path", "x", { "a" }, inlineDeclarations);
    // inline beats any normal rule, even an id
    REQUIRE( Value(values, "fill") == "green" );
    // an !important rule beats normal inline
    REQUIRE( Value(values, "stroke") == "blue" );
    // !important inline beats !important rules
    REQUIRE( Value(values, "opacity") == "1" );
}

TEST_CASE("StyleSheet: unsupported selectors and @-rules are skipped", "[css][svg]") {
    StyleSheet styleSheet {};
    styleSheet.append(R"css(
        @import url(other.css);
        @media print { .a { fill: red; } }
        g .a { fill: red; }
        g > .a { fill: red; }
        .a:hover { fill: red; }
        [data-x] { fill: red; }
        .a, g .b { stroke: blue; }
    )css");
    REQUIRE( styleSheet.getRuleCount() == 1 );
    REQUIRE( styleSheet.getSkippedSelectorCount() == 5 );
    auto values = styleSheet.cascade("path", "", { "a", "b" });
    REQUIRE( Value(values, "fill") == "<unset>" );
    REQUIRE( Value(values, "stroke") == "blue" );
}

TEST_CASE("StyleSheet: declarations keep semicolons inside parentheses and quotes", "[css][svg]") {
    auto declarations = StyleSheet::ParseDeclarations(R"(FILL : url(data:image/png;base64,AAAA) ; font-family: "a;b"; broken; stroke:red)");
    REQUIRE( declarations.size() == 3 );
    REQUIRE( declarations[0].property == "fill" );
    REQUIRE( declarations[0].value == "url(data:image/png;base64,AAAA)" );
    REQUIRE( declarations[1].value == "\"a;b\"" );
    REQUIRE( declarations[2].property == "stroke" );
    REQUIRE( declarations[2].value == "red" );
}
