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

#ifndef SVGCOLOURTYPE_DOT_HPP
#define SVGCOLOURTYPE_DOT_HPP

namespace Core {
    class String;
}

namespace Draw2d::Svg {

    typedef enum class SvgColourType
    {
        NotAnSvgColourType = -1,
        CurrentColor,
        Cpal,
        Bgr,
        BgrA,
        Hsl,
        HslA,
        Gradient,
        None
    } SvgColourType;

    SvgColourType ParseSvgColourType(const Core::String &cstrSvgColourType);
    Core::String SvgColourTypeToString(SvgColourType enumSvgColourType);

}

#endif  /* SVGCOLOURTYPE_DOT_HPP */
