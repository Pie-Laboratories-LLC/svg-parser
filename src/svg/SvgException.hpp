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

#ifndef SVGEXCEPTION_DOT_HPP
#define SVGEXCEPTION_DOT_HPP

#ifndef DRAW2DEXCEPTION_DOT_HPP
    #include "draw2d/Draw2dException.hpp"
#endif

namespace Core {
    class String;
}

namespace Draw2d::Svg {

    class SvgException : public Draw2dException
    {
    public:
        using SvgException::Draw2dException::Draw2dException;

        SvgException(const Core::String &cstrMessage): Draw2dException(cstrMessage)
        {
        }
    };

} // namespace draw2d::Svg

#endif  /* SVGEXCEPTION_DOT_HPP */
