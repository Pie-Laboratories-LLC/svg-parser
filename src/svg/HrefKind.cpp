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

#include "svg/HrefKind.hpp"

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif

#ifndef DRAW2D_SVG_EXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif

namespace Draw2d::Svg {
    Core::String HrefKindToString(HrefKind enumHrefKind) {
        switch(enumHrefKind) {
        case HrefKind::Url: return "Url";
        case HrefKind::DecodedBinary: return "DecodeBinary";
        case HrefKind::LiteralText: return "LiteralText";
        default: throw SvgException("Invalid illegal unknown unrecognized unhandled garbage HrefKind {}", static_cast<int32_t>(enumHrefKind));
        }
    }
}
