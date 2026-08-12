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

#ifndef CORE_LOGGERBASE_DOT_HPP
#define CORE_LOGGERBASE_DOT_HPP

#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <unordered_set>
#include <format>
#include <mutex>

#ifndef CORE_STDSTRINGEXTENSIONS_DOT_HPP
    #include "core/StdStringExtensions.hpp"
#endif
#ifndef CORE_LOGLEVEL_DOT_HPP
    #include "LogLevel.hpp"
#endif
#ifndef CORE_SCOPEEXIT_DOT_HPP
    #include "core/ScopeExit.hpp"
#endif

namespace Core {

    class LoggerBase {
    public:
        template <typename... Args>
        void Trace(const char *subject,const char* format, Args&&... args) { this->_Log(LogLevel::Trace,subject,format,std::forward<Args>(args)...); };
        template <typename... Args>
        void Info(const char *subject,const char* format, Args&&... args) { this->_Log(LogLevel::Info,subject,format,std::forward<Args>(args)...); }
        template <typename... Args>
        void Warning(const char *subject,const char* format, Args&&... args) { this->_Log(LogLevel::Warn,subject,format,std::forward<Args>(args)...); }
        template <typename... Args>
        void Error(const char *subject,const char* format, Args&&... args) { this->_Log(LogLevel::Error,subject,format,std::forward<Args>(args)...); }
        template <typename... Args>
        void Fatal(const char *subject,const char* format, Args&&... args) { this->_Log(LogLevel::Fatal,subject,format,std::forward<Args>(args)...); }

    protected:
        LoggerBase(std::unordered_set<const char *,CStrHash,CStrEq> categories,LogLevel enumLogLevel):
            m_categories(std::move(categories)),
            m_enumLogLevel(enumLogLevel) {
        }

        std::unordered_set<const char *,CStrHash,CStrEq> m_categories;
        LogLevel m_enumLogLevel;

        template <typename... Args>
        void _Log(LogLevel enumLogLevel, const char *subject, const char* format, Args&&... args) {
            if (static_cast<uint32_t>(enumLogLevel) >= this->m_enumLogLevel
            && (!strcmp(subject,"all")
             || (m_categories.find(subject) != m_categories.end()))) {
                switch(enumLogLevel) {
                case LogLevel::Error:   std::cout << "\033[31m"; break;  // red
                case LogLevel::Warn: std::cout << "\033[33m"; break;  // yellow
                case LogLevel::Info:    std::cout << "\033[97m";  break;  // reset/default
                case LogLevel::Trace:    std::cout << "\033[32m";  break;  // reset/default

                default: std::cout << "\033[0m";  break;  // reset/default
                }
                std::time_t now = std::time(nullptr); // Get current time
                std::tm* gmtm = std::gmtime(&now); // Convert to local time
                const char * const cpszLogLevel = LogLevelToString(enumLogLevel);
                // mutex lock / unlock.  You may be wondering why we use this rather than
                //  ThreadSavvyObject.  We can't link against threading here in Core; I
                //  considered moving to Platform, but we just need a mutex.  so, hold
                //  our nose: looks good
                _mutex.lock();
                Core::ScopeExit se([this]() {
                    _mutex.unlock();
                });
                std::cout << std::format("[{}] [{}] ",subject,cpszLogLevel)
                          // c++: formatting time = intractible problem
                          // c++: interpolating string = intractible problem, _in 2025_
                          << "[" << std::put_time(gmtm,"%Y %m %d %H:%M:%S") << "]: "
                          << std::vformat(std::string(format), std::make_format_args(args...))
                          << "\033[0m"
                          << std::endl;
            }
        }

    private:
        std::mutex _mutex {};
    }; // class LoggerBase

} // namespace Core

#endif  /* CORE_LOGGERBASE_DOT_HPP */
