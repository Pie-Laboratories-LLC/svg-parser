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

#include <format>
#include <memory>
#include <map>

#ifndef STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef STDNUMERIC_DOT_HPP
    #include "core/StdNumeric.hpp"
#endif
#ifndef DRAW2DEXCEPTION_DOT_hPP
    #include "draw2d/Draw2dException.hpp"
#endif

namespace Draw2d {

    class StandardColours
    {
    public:
        // obnoxiously, with duplicates 😡 https://www.w3.org/wiki/CSS/Properties/color/keywords
        static std::map<Core::String,Core::String> Colours;

        static std::array<uint8_t,4> RgbOf(const Core::String &name) {
            if(!Colours.contains(name)) throw Draw2dException("{} does not name a standard colour", name.c_str());
            Core::String rgbValue = Colours[name];
            return ParseRgb(rgbValue);
        }

        static std::array<uint8_t,4> ParseRgb(const Core::String &rgbValue)
        {
            // ultimately, this should support rgb & rgba...
            if (rgbValue.length() != 6 && rgbValue.length() != 3) throw Draw2dException("not a valid rgb value {}", rgbValue.c_str());
            std::array<uint8_t,4> rgba { };
            if (rgbValue.length() == 3)
            {
                // i.e., string looks like "fff" where each channel has single (hex) digit
                // is there a worse way to do this? I mean if I applied myself
                rgba[0] = (uint8_t)Core::ParseInteger(rgbValue.substr(0, 1) + rgbValue.substr(0, 1), 16);
                rgba[1] = (uint8_t)Core::ParseInteger(rgbValue.substr(1, 1) + rgbValue.substr(1, 1), 16);
                rgba[2] = (uint8_t)Core::ParseInteger(rgbValue.substr(2, 1) + rgbValue.substr(2, 1), 16);
            }
            else
            {
                rgba[0] = (uint8_t)Core::ParseInteger(rgbValue.substr(0, 2), 16);
                rgba[1] = (uint8_t)Core::ParseInteger(rgbValue.substr(2, 2), 16);
                rgba[2] = (uint8_t)Core::ParseInteger(rgbValue.substr(4, 2), 16);
            }
            rgba[3] = 0xff;

            return rgba;
        }

        static std::array<uint8_t,4> BgrOf(const Core::String &name) {
            Core::String rgbValue = Colours[name];
            std::array<uint8_t,4> bgra { };
            bgra[0] = (uint8_t)Core::ParseInteger(rgbValue.substr(4,2),16);
            bgra[1] = (uint8_t)Core::ParseInteger(rgbValue.substr(2,2),16);
            bgra[2] = (uint8_t)Core::ParseInteger(rgbValue.substr(0,2),16);
            bgra[3] = 0xff;
            return bgra;
        }
    };

} // namespace Draw2d
