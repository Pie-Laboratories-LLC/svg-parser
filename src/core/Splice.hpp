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

#ifndef CORE_SPLICE_DOT_HPP
#define CORE_SPLICE_DOT_HPP


#include <vector>
#include <algorithm>
#include <iterator>
#ifndef CORE_SLICE_DOT_HPP
    #include "core/Slice.hpp"   // for Core::SliceIndex / operator^
#endif

namespace Core {

    // claude (Anthropic) generated per my request.  8/2026
    //
    // Perl-style splice: removes `length` elements starting at `offset`,
    // replaces them with the contents of `replacement`, and returns the
    // removed elements (Perl returns the removed list; we mirror that).
    //
    //   splice(vec, offset)                       -> remove offset..end
    //   splice(vec, offset, length)                -> remove `length` elements at offset
    //   splice(vec, offset, length, replacement)   -> remove + insert replacement in their place
    //
    // `offset` supports ^N (from-the-end) via SliceIndex, same as Core::Slice.
    template<typename T>
    std::vector<T> splice(std::vector<T>& vec,
                           SliceIndex offset,
                           std::size_t length,
                           const std::vector<T>& replacement)
    {
        std::size_t size = vec.size();
        std::size_t start = offset.resolve(size);
        if (start > size) start = size;

        std::size_t available = size - start;
        std::size_t removeCount = length < available ? length : available;

        std::vector<T> removed;
        removed.reserve(removeCount);
        std::move(vec.begin() + start, vec.begin() + start + removeCount,
                  std::back_inserter(removed));

        // Erase the removed range, then insert the replacement in its place.
        auto eraseEnd = vec.begin() + start + removeCount;
        auto it = vec.erase(vec.begin() + start, eraseEnd);
        vec.insert(it, replacement.begin(), replacement.end());

        return removed;
    }

    // small optimisation to use initializer list in place of vector
    template<typename T>
    std::vector<T> splice(std::vector<T>& vec,
                           SliceIndex offset,
                           std::size_t length,
                           const std::initializer_list<T>& replacement)
    {
        std::size_t size = vec.size();
        std::size_t start = offset.resolve(size);
        if (start > size) start = size;

        std::size_t available = size - start;
        std::size_t removeCount = length < available ? length : available;

        std::vector<T> removed;
        removed.reserve(removeCount);
        std::move(vec.begin() + start, vec.begin() + start + removeCount,
                  std::back_inserter(removed));

        // Erase the removed range, then insert the replacement in its place.
        auto eraseEnd = vec.begin() + start + removeCount;
        auto it = vec.erase(vec.begin() + start, eraseEnd);
        vec.insert(it, replacement.begin(), replacement.end());

        return removed;
    }

    // Convenience overload: remove from `offset` to the end (no replacement).
    template<typename T>
    std::vector<T> splice(std::vector<T>& vec, SliceIndex offset)
    {
        std::size_t size = vec.size();
        std::size_t start = offset.resolve(size);
        if (start > size) start = size;
        return splice(vec, offset, size - start);
    }

    // Convenience overload: remove `length` elements at `offset`, no replacement.
    template<typename T>
    std::vector<T> splice(std::vector<T>& vec, SliceIndex offset, std::size_t length)
    {
        return splice(vec, offset, length, std::vector<T>{});
    }

} // namespace Core
#endif  /* CORE_SPLICE_DOT_HPP */
