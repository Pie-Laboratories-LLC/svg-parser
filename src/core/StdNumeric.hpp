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

#ifndef CORE_STDNUMERIC_DOT_HPP
#define CORE_STDNUMERIC_DOT_HPP

#include <charconv>
#include <limits>

namespace Core {

    class String;

    float ParseFloat(const String &cstrFloat,unsigned offset = 0,unsigned size = std::numeric_limits<unsigned>::max());

#define BASE2 2
#define BASE8 8
#define BASE10 10
#define BASE16 16

    int ParseInteger(const String &cstrInteger,unsigned base = BASE10,unsigned offset = 0,unsigned size = std::numeric_limits<unsigned>::max());

} // namespace Core

#endif  /* CORE_STDNUMERIC_DOT_HPP */
