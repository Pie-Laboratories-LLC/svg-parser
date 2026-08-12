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

#ifndef CORE_STDUNIQUEPTREXTENSIONS_DOT_HPP
#define CORE_STDUNIQUEPTREXTENSIONS_DOT_HPP

#include <memory>

namespace Core {

template <typename T>
std::unique_ptr<T[]> makeUniqueArray(std::initializer_list<T> values) {
    // claude ... :(  I mean some anonymous github contributer
    auto arr = std::make_unique<T[]>(values.size());
    std::copy(values.begin(), values.end(), arr.get());
    return arr;
}
} // namespace Core

#endif  /* CORE_STDUNIQUEPTREXTENSIONS_DOT_HPP */
