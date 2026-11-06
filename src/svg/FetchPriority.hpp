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

#ifndef DRAW2D_SVG_FETCHPRIORITY_DOT_HPP
#define DRAW2D_SVG_FETCHPRIORITY_DOT_HPP
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
namespace Draw2d::Svg {
    enum class FetchPriority {
        NotAFetchPriority = -1,
        High,
        Low
    };
    FetchPriority ParseFetchPriority(const Core::String &cstrFetchPriority);
    Core::String FetchPriorityToString(FetchPriority enumFetchPriority);
}
#endif  /* DRAW2D_SVG_FETCHPRIORITY_DOT_HPP */
