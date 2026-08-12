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

#include "draw2d/SpreadMethod.hpp"

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef DRAW2D_DRAW2DEXCEPTION_DOT_HPP
    #include "draw2d/Draw2dException.hpp"
#endif

namespace Draw2d {
    Core::String SpreadMethodToString(SpreadMethod enumSpreadMethod)
    {
        switch(enumSpreadMethod) {
        case SpreadMethod::NotASpreadMethod: return "NotASpreadMethod";
        case SpreadMethod::Pad: return "Pad";
        case SpreadMethod::Reflect: return "Reflect";
        case SpreadMethod::Repeat: return "Repeat";
        default: return Core::String("Invalid illegal unknown unrecognized unhandled garbage spread method {}",static_cast<int32_t>(enumSpreadMethod));
        }
    }

    // this is silly, even by my standards
    #define PAD "pad"
    #define REFLECT "reflect"
    #define REPEAT "repeat"
    SpreadMethod ParseSpreadMethod(const Core::String &cstrSpreadMethod)
    {
        Core::String strSpreadMethod  = cstrSpreadMethod.trim();
        if(strSpreadMethod == PAD) return SpreadMethod::Pad;
        if(strSpreadMethod == REFLECT) return SpreadMethod::Reflect;
        if(strSpreadMethod == REPEAT) return SpreadMethod::Repeat;
        throw Draw2dException("Invalid spread method {}", strSpreadMethod.c_str());
    }
}
