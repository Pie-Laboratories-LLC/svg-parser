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

#include "svg/CrossOrigin.hpp"
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
#define CROSSORIGIN_NOTACROSSORIGIN "NotACrossOrigin"
#define CROSSORIGIN_ANONYMOUS "anonymous"
#define CROSSORIGIN_USECREDENTIALS "use-credentials"
namespace Draw2d::Svg {
    CrossOrigin ParseCrossOrigin(const Core::String &cstrCrossOrigin)
    {
        if (cstrCrossOrigin == CROSSORIGIN_NOTACROSSORIGIN) return CrossOrigin::NotACrossOrigin;
        if (cstrCrossOrigin == CROSSORIGIN_ANONYMOUS) return CrossOrigin::Anonymous;
        if (cstrCrossOrigin == CROSSORIGIN_USECREDENTIALS) return CrossOrigin::UseCredentials;
        throw SvgException("Unrecognized cross origin {}", cstrCrossOrigin.c_str());
    }
    Core::String CrossOriginToString(CrossOrigin enumCrossOrigin)
    {
        switch(enumCrossOrigin) {
        case CrossOrigin::NotACrossOrigin: return "NotACrossOrigin";
        case CrossOrigin::Anonymous: return "Anonymous";
        case CrossOrigin::UseCredentials: return "UseCredentials";
        default: return Core::String::Format("Invalid illegal unknown unrecognized unhandled garbage CrossOrigin {}", static_cast<int32_t>(enumCrossOrigin));
        }
    }
}
