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

#ifndef DRAW2D_SVG_PRESERVEASPECTRATIO_DOT_HPP
#define DRAW2D_SVG_PRESERVEASPECTRATIO_DOT_HPP

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif

namespace Draw2d::Svg {

    enum class PreserveAspectRatio {
        NotAPreserveAspectRatio = -1,
        none,
        xMinYMin,
        xMidYMin,
        xMaxYMin,
        xMinYMid,
        xMidYMid,
        xMaxYMid,
        xMinYMax,
        xMidYMax,
        xMaxYMax
    };

    PreserveAspectRatio ParsePreserveAspectRatio(const Core::String &cstrPreserveAspectRatio);
    Core::String PreserveAspectRatioToString(PreserveAspectRatio enumPreserveAspectRatio);

    enum class PreserveAspectRatioMode {
        NotAPreserveAspectRatioMode = -1,
        Meet,
        Slice
    };

    PreserveAspectRatioMode ParsePreserveAspectRatioMode(const Core::String &cstrPreserveAspectRatioMode);
    Core::String PreserveAspectRatioModeToString(PreserveAspectRatioMode enumPreserveAspectRatioMode);
}

#endif  /* DRAW2D_SVG_PRESERVEASPECTRATIO_DOT_HPP */
