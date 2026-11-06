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

#ifndef CORE_LOGGER_DOT_HPP
#define CORE_LOGGER_DOT_HPP

#include <unordered_set>

#ifndef CORE_LOGGERBASE_DOT_HPP
    #include "core/LoggerBase.hpp"
#endif

namespace Core {

    class Logger: public LoggerBase {
    private:
        Logger(std::unordered_set<const char *,CStrHash,CStrEq> categories, LogLevel enumLogLevel);

    public:
        static void Instance(std::unordered_set<const char *,CStrHash,CStrEq> categories, LogLevel enumLogLevel);
        static Logger &Singleton();

    private:
        static Logger *sm_pSingleton;
    }; // class Logger: public LoggerBase

    // écrire sans nommer le singleton: Info(LOG, "…"). Sans « Core:: » d'où qu'on
    // les appelle: la catégorie étant une Core::Catégorie, la recherche dépendante des
    // arguments les trouve -- sauf dans une classe qui a son propre membre de ce nom
    template <typename... Args>
    void Trace(const Catégorie &catégorie, const char *format, Args&&... args) { Logger::Singleton().Trace(catégorie, format, std::forward<Args>(args)...); }
    template <typename... Args>
    void Debug(const Catégorie &catégorie, const char *format, Args&&... args) { Logger::Singleton().Debug(catégorie, format, std::forward<Args>(args)...); }
    template <typename... Args>
    void Info(const Catégorie &catégorie, const char *format, Args&&... args) { Logger::Singleton().Info(catégorie, format, std::forward<Args>(args)...); }
    template <typename... Args>
    void Warning(const Catégorie &catégorie, const char *format, Args&&... args) { Logger::Singleton().Warning(catégorie, format, std::forward<Args>(args)...); }
    template <typename... Args>
    void Error(const Catégorie &catégorie, const char *format, Args&&... args) { Logger::Singleton().Error(catégorie, format, std::forward<Args>(args)...); }
    template <typename... Args>
    void Fatal(const Catégorie &catégorie, const char *format, Args&&... args) { Logger::Singleton().Fatal(catégorie, format, std::forward<Args>(args)...); }

    inline void Trace(const Catégorie &catégorie, std::string_view texte) { Logger::Singleton().Trace(catégorie, texte); }
    inline void Debug(const Catégorie &catégorie, std::string_view texte) { Logger::Singleton().Debug(catégorie, texte); }
    inline void Info(const Catégorie &catégorie, std::string_view texte) { Logger::Singleton().Info(catégorie, texte); }
    inline void Warning(const Catégorie &catégorie, std::string_view texte) { Logger::Singleton().Warning(catégorie, texte); }
    inline void Error(const Catégorie &catégorie, std::string_view texte) { Logger::Singleton().Error(catégorie, texte); }
    inline void Fatal(const Catégorie &catégorie, std::string_view texte) { Logger::Singleton().Fatal(catégorie, texte); }

} // namespace Core

#endif  /* CORE_LOGGER_DOT_HPP */
