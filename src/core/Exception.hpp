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

#ifndef CORE_EXCEPTION_DOT_HPP
#define CORE_EXCEPTION_DOT_HPP

#include <stacktrace>
#include <format>
#ifndef STRING_DOT_HPP
    #include "core/String.hpp"
#endif

namespace Core {

    class Exception : public std::exception {
    public:
        template<typename... Args>
        Exception(std::format_string<Args...> fmt, Args&&... args)
            : Exception(Core::String(std::format(fmt, std::forward<Args>(args)...))) {}

        Exception(const Core::String &cstrMessage):
            m_strMessage(cstrMessage),
            m_strStackTrace(std::to_string(std::stacktrace::current()))
        {
            __buildFullMessage();
        }

        virtual ~Exception() noexcept { }

        const char* what() const noexcept override {
            return m_strFullMessage.c_str();
        }

    private:
        Core::String m_strMessage { };
        std::string m_strStackTrace {};
        // built once in the constructor body, not in what()
        std::string m_strFullMessage;

        void __buildFullMessage() {
            m_strFullMessage = std::format("{}\n--- stack trace ---\n{}", m_strMessage.c_str(), m_strStackTrace);
        }
    };
}

#endif  /* CORE_EXCEPTION_DOT_HPP */
