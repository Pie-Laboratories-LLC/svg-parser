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

#ifndef CORE_SCOPEEXIT_DOT_HPP
#define CORE_SCOPEEXIT_DOT_HPP

#include <functional>

namespace Core {

    class ScopeExit {
    public:
        explicit ScopeExit(std::function<void()> f) : m_function(std::move(f)) {}

        ~ScopeExit() { if (m_bActive) m_function(); }

        // no copying — doesn't make sense for a scope guard
        ScopeExit(const ScopeExit&) = delete;
        ScopeExit& operator=(const ScopeExit&) = delete;

        // moving is fine — transfer responsibility, disable the source
        ScopeExit(ScopeExit&& other) noexcept
            : m_function(std::move(other.m_function)), m_bActive(other.m_bActive) {
            other.m_bActive = false;
            other.m_function = nullptr;
        }
        ScopeExit& operator=(ScopeExit&&) = delete; // or implement similarly if needed

        void execute() { if (m_bActive) m_function(); m_bActive = false; }

        void dismiss() { m_bActive = false; }

    private:
        std::function<void()> m_function = nullptr;
        bool m_bActive = true;
    }; // class ScopeExit

    inline ScopeExit make_scope_exit(std::function<void()> f) {
        return ScopeExit(std::move(f));
    }

} // namespace Core

#endif  /* CORE_SCOPEEXIT_DOT_HPP */
