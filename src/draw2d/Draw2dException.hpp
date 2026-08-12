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

#ifndef DRAW2DEXCEPTION_DOT_HPP
#define DRAW2DEXCEPTION_DOT_HPP

#ifndef CORE_EXCEPTION_DOT_HPP
    #include "core/Exception.hpp"
#endif

namespace Draw2d {

    class Draw2dException : public Core::Exception {
    public:
        using Exception::Exception;

        Draw2dException(const Core::String &cstrMessage): Core::Exception(cstrMessage) {
        }
    };

}

#endif  /* DRAW2DEXCEPTION_DOT_HPP */
