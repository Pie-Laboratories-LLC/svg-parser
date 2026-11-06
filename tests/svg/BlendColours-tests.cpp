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
#include <catch2/catch_approx.hpp>

#ifndef DRAW2D_SVG_BLENDCOLOURS_DOT_HPP
    #include "svg/BlendColours.hpp"
#endif

using Draw2d::Svg::BlendColours;
using Draw2d::Svg::BlendMode;

namespace {
    bool Near(const std::array<float,3> &a, const std::array<float,3> &b)
    {
        for(size_t n = 0; n < 3; n++) if(a[n] != Catch::Approx(b[n]).margin(1e-4)) return false;
        return true;
    }
    const std::array<float,3> GREY { 0.5f, 0.5f, 0.5f };
}

TEST_CASE("BlendColours: separable modes", "[blend][svg]") {
    const std::array<float,3> backdrop { 0.2f, 0.5f, 0.8f };
    const std::array<float,3> source { 0.6f, 0.5f, 0.1f };
    REQUIRE( Near(BlendColours(BlendMode::Normal, backdrop, source), source) );
    REQUIRE( Near(BlendColours(BlendMode::Multiply, backdrop, source), { 0.12f, 0.25f, 0.08f }) );
    REQUIRE( Near(BlendColours(BlendMode::Screen, backdrop, source), { 0.68f, 0.75f, 0.82f }) );
    REQUIRE( Near(BlendColours(BlendMode::Darken, backdrop, source), { 0.2f, 0.5f, 0.1f }) );
    REQUIRE( Near(BlendColours(BlendMode::Lighten, backdrop, source), { 0.6f, 0.5f, 0.8f }) );
    REQUIRE( Near(BlendColours(BlendMode::Difference, backdrop, source), { 0.4f, 0.0f, 0.7f }) );
    REQUIRE( Near(BlendColours(BlendMode::Exclusion, backdrop, source), { 0.56f, 0.5f, 0.74f }) );
    // hard-light multiplies below half and screens above; overlay is hard-light with the roles swapped
    REQUIRE( Near(BlendColours(BlendMode::HardLight, backdrop, source), { 0.36f, 0.5f, 0.16f }) );
    REQUIRE( Near(BlendColours(BlendMode::Overlay, backdrop, source), { 0.24f, 0.5f, 0.64f }) );
}

TEST_CASE("BlendColours: dodge, burn and soft-light, edges included", "[blend][svg]") {
    REQUIRE( Near(BlendColours(BlendMode::ColorDodge, { 0.0f, 0.5f, 0.4f }, { 0.9f, 1.0f, 0.5f }), { 0.0f, 1.0f, 0.8f }) );
    REQUIRE( Near(BlendColours(BlendMode::ColorBurn, { 1.0f, 0.5f, 0.8f }, { 0.1f, 0.0f, 0.5f }), { 1.0f, 0.0f, 0.6f }) );
    // soft-light: grey source leaves the backdrop alone; darker darkens, lighter lightens
    REQUIRE( Near(BlendColours(BlendMode::SoftLight, { 0.3f, 0.6f, 0.1f }, GREY), { 0.3f, 0.6f, 0.1f }) );
    REQUIRE( BlendColours(BlendMode::SoftLight, GREY, { 0.2f, 0.2f, 0.2f })[0] < 0.5f );
    REQUIRE( BlendColours(BlendMode::SoftLight, GREY, { 0.8f, 0.8f, 0.8f })[0] > 0.5f );
}

TEST_CASE("BlendColours: non-separable modes keep the right component", "[blend][svg]") {
    // luminosity of a grey source on a red backdrop: red's hue, grey's luminosity
    const auto luminosity = BlendColours(BlendMode::Luminosity, { 1.0f, 0.0f, 0.0f }, GREY);
    REQUIRE( 0.3f * luminosity[0] + 0.59f * luminosity[1] + 0.11f * luminosity[2] == Catch::Approx(0.5f).margin(1e-4) );
    REQUIRE( luminosity[0] > luminosity[1] );
    // color on a grey backdrop takes the source's hue and saturation at grey's luminosity
    const auto colour = BlendColours(BlendMode::Color, GREY, { 0.0f, 0.0f, 1.0f });
    REQUIRE( 0.3f * colour[0] + 0.59f * colour[1] + 0.11f * colour[2] == Catch::Approx(0.5f).margin(1e-4) );
    REQUIRE( colour[2] > colour[0] );
    // hue and saturation from a grey (no saturation) leave a grey
    const auto desaturated = BlendColours(BlendMode::Saturation, { 0.9f, 0.1f, 0.1f }, GREY);
    REQUIRE( desaturated[0] == Catch::Approx(desaturated[1]).margin(1e-4) );
    const auto hue = BlendColours(BlendMode::Hue, { 0.9f, 0.1f, 0.1f }, { 0.1f, 0.1f, 0.9f });
    REQUIRE( hue[2] > hue[0] );
}
