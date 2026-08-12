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

#ifndef CORE_STDSTRINGEXTENSIONS_DOT_HPP
#define CORE_STDSTRINGEXTENSIONS_DOT_HPP

#include <cstring>

// claude <3
struct CStrHash {
    using is_transparent = void; // enables heterogeneous lookup
    size_t operator()(const char* s) const {
        return std::hash<std::string_view>{}(s);
    }
};

struct CStrEq {
    using is_transparent = void;
    bool operator()(const char* a, const char* b) const {
        return std::strcmp(a, b) == 0;
    }
};

#endif  /* CORE_STDSTRINGEXTENSIONS_DOT_HPP */
