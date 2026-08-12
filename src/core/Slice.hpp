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

#ifndef CORE_SLICE_DOT_HPP
#define CORE_SLICE_DOT_HPP

#include <cstddef>
#include <stdexcept>
#include <concepts>
#include <vector>
#include <array>

namespace Core {

    // claude (Anthropic) generated per my request.  8/2026
    //
    // Mimics C#'s Index: a value that can count from the front (0, 1, 2...)
    // or from the back (^1, ^2... in C#; here spelled 1_end, 2_end...).
    //
    // std::vector<Core::UseNode> nodes = /* ... */;
    // 
    // Core::Slice<Core::UseNode> all(nodes);
    // 
    // auto tail        = all.slice(2);          // from index 2 to end
    // auto middle      = all.slice(2, 5);        // 5 elements starting at 2
    // auto lastThree   = all.slice(3_end);       // last 3 elements
    // auto middleRange = all.range(2, 2_end);    // [2 .. length-2)
    struct SliceIndex
    {
        std::size_t value;
        bool fromEnd;

        constexpr SliceIndex(std::size_t v, bool end = false) : value(v), fromEnd(end) {}

        constexpr std::size_t resolve(std::size_t length) const
        {
            return fromEnd ? (length >= value ? length - value : 0) : value;
        }
    };

    // C#-style "from the end" literal: use as `3_end` to mean "3 from the end"
    constexpr SliceIndex operator""_end(unsigned long long v)
    {
        return SliceIndex(static_cast<std::size_t>(v), true);
    }

    template<typename T>
    class Slice
    {
    public:
        using value_type     = T;
        using iterator       = T*;
        using const_iterator = const T*;

        constexpr Slice() noexcept : _data(nullptr), _size(0) {}
        constexpr Slice(T* data, std::size_t size) noexcept : _data(data), _size(size) {}

        // Build from any contiguous container: std::vector, std::array, etc.
        // Constrained via `requires` so it only binds to things with a .data()
        // convertible to T*.
        template<typename Container>
            requires requires(Container& c) { { c.data() } -> std::convertible_to<T*>; }
        constexpr Slice(Container& c) : _data(c.data()), _size(c.size()) {}

        // Build from a raw C array
        template<std::size_t N>
        constexpr Slice(T (&arr)[N]) noexcept : _data(arr), _size(N) {}

        // --- basic access, same shape as std::span ---
        constexpr T*  data()  const noexcept { return _data; }
        constexpr std::size_t size()  const noexcept { return _size; }
        constexpr bool  empty() const noexcept { return _size == 0; }

        constexpr T& operator[](std::size_t i) const { return _data[i]; }

        constexpr T& at(std::size_t i) const
        {
            if (i >= _size) throw std::out_of_range("Slice::at index out of range");
            return _data[i];
        }

        constexpr T& front() const { return _data[0]; }
        constexpr T& back()  const { return _data[_size - 1]; }

        constexpr iterator begin() const noexcept { return _data; }
        constexpr iterator end()   const noexcept { return _data + _size; }

        // --- C#-style slicing ---

        // list.Slice(start) -> everything from start to the end
        constexpr Slice<T> slice(SliceIndex start) const
        {
            std::size_t s = clampIndex(start.resolve(_size));
            return Slice<T>(_data + s, _size - s);
        }

        // list.Slice(start, length) -> `length` elements beginning at start
        constexpr Slice<T> slice(SliceIndex start, std::size_t length) const
        {
            std::size_t s = clampIndex(start.resolve(_size));
            std::size_t available = _size - s;
            std::size_t len = length < available ? length : available;
            return Slice<T>(_data + s, len);
        }

        // list[start..end) equivalent (end exclusive, both endpoints support N_end)
        constexpr Slice<T> range(SliceIndex start, SliceIndex end) const
        {
            std::size_t s = clampIndex(start.resolve(_size));
            std::size_t e = clampIndex(end.resolve(_size));
            if (e < s) e = s;
            return Slice<T>(_data + s, e - s);
        }

    private:
        T* _data;
        std::size_t _size;

        constexpr std::size_t clampIndex(std::size_t i) const
        {
            return i > _size ? _size : i;
        }
    };

    // Deduction guides so `Slice s(myVector);` works without spelling out <T>
    template<typename Container>
    Slice(Container&) -> Slice<typename Container::value_type>;

    template<typename T, std::size_t N>
    Slice(T (&)[N]) -> Slice<T>;

} // namespace Core

#endif  /* CORE_SLICE_DOT_HPP */
