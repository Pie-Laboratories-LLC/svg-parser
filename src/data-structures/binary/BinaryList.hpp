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

#ifndef DATASTRUCTURES_BINARY_BINARYLIST_DOT_HPP
#define DATASTRUCTURES_BINARY_BINARYLIST_DOT_HPP

#include <algorithm>
#include <concepts>
#include <functional>

// this is a claude rewrite of my crufty old n busted version which I had
//  written as an old C hack farting around in c++ and too lazy to learn the
//  stl.  On the list: I got a good recommendation of a book for the STL.
// this stuff is phenominal and I need to master it.
namespace DataStructures::Binary {

    // Comparator returns true if 'a' should sort before 'b' -- same contract
    // as std::less / std::lower_bound's own comparator, NOT the old
    // compare()-returns-int/negative/zero/positive style.
    template<typename Comp, typename T>
    concept OrderingFor = requires(const Comp &comp, const T &a, const T &b) {
        { comp(a, b) } -> std::convertible_to<bool>;
    };

    template<typename Container, typename Comp = std::less<typename Container::value_type>>
    requires OrderingFor<Comp, typename Container::value_type>
    typename Container::iterator BinarySearch(Container &blist, const typename Container::value_type &value, Comp comparator = {}) {
        auto it = std::lower_bound(blist.begin(), blist.end(), value, comparator);
        if (it != blist.end() && !comparator(value, *it) && !comparator(*it, value)) {
            return it;   // exact match found
        }
        return blist.end();
    }

    template<typename Container, typename Comp = std::less<typename Container::value_type>>
    requires OrderingFor<Comp, typename Container::value_type>
    bool BinaryContains(Container &blist, const typename Container::value_type &value, Comp comparator = {}) {
        return BinarySearch(blist, value, comparator) != blist.end();
    }

    // Move-friendly insert: accepts an rvalue (works directly with
    // std::move(pUniquePtr)), finds the correct sorted position via
    // lower_bound, inserts by moving -- no void*, no reinterpret_cast.
    template<typename Container, typename Comp = std::less<typename Container::value_type>>
    requires OrderingFor<Comp, typename Container::value_type>
    typename Container::iterator BinaryInsert(Container &blist, typename Container::value_type value, Comp comparator = {}) {
        auto pos = std::lower_bound(blist.begin(), blist.end(), value, comparator);
        return blist.insert(pos, std::move(value));
    }

    // Insert-or-replace: if an equal element already exists, replace it and
    // return the old value; otherwise insert and return std::nullopt.
    template<typename Container, typename Comp = std::less<typename Container::value_type>>
    requires OrderingFor<Comp, typename Container::value_type>
    std::optional<typename Container::value_type> BinaryInsertReplace(Container &blist, typename Container::value_type value, Comp comparator = {}) {
        auto pos = std::lower_bound(blist.begin(), blist.end(), value, comparator);
        if (pos != blist.end() && !comparator(value, *pos) && !comparator(*pos, value)) {
            auto old = std::move(*pos);
            *pos = std::move(value);
            return old;
        }
        blist.insert(pos, std::move(value));
        return std::nullopt;
    }

} // namespace DataStructures::Binary

#endif  /* DATASTRUCTURES_BINARY_BINARYLIST_DOT_HPP */
