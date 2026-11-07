/*
 * Copyright 2026 Pie Laboratories LLC
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

#include <array>
#include <cctype>
#include <cstdint>
#include <utility>
#include "core/LogLevel.hpp"
#ifndef CORE_EXCEPTION_DOT_HPP
    #include "core/Exception.hpp"
#endif
namespace Core {
    namespace {
        // les noms écrits, dans l'ordre de l'énumération: une seule table pour lire et
        // pour écrire -- les deux sens s'accordent toujours
        constexpr std::array<std::pair<std::string_view, LogLevel>, 7> NomsLogLevel { {
            { "NotALogLevel", LogLevel::NotALogLevel },
            { "Trace", LogLevel::Trace },
            { "Debug", LogLevel::Debug },
            { "Info", LogLevel::Info },
            { "Warn", LogLevel::Warn },
            { "Error", LogLevel::Error },
            { "Fatal", LogLevel::Fatal },
        } };

        // sans égard à la casse -- ASCII seulement, comme std::tolower sans locale
        bool ÉgauxSansCasse(std::string_view a, std::string_view b)
        {
            if (a.size() != b.size()) return false;
            for (size_t i = 0; i < a.size(); i++)
                if (std::tolower(static_cast<unsigned char>(a[i])) != std::tolower(static_cast<unsigned char>(b[i])))
                    return false;
            return true;
        }
    }

    std::optional<LogLevel> ChercherLogLevel(std::string_view texte)
    {
        for (const auto &[nom, valeur] : NomsLogLevel)
            if (ÉgauxSansCasse(nom, texte)) return valeur;
        return std::nullopt;
    }

    LogLevel ParseLogLevel(const Core::String &cstrLogLevel)
    {
        if (const auto valeur = ChercherLogLevel(cstrLogLevel)) return *valeur;
        throw Exception("Unrecognized log level « {} » -- Trace, Debug, Info, Warn, Error, Fatal", cstrLogLevel.c_str());
    }

    Core::String LogLevelToString(LogLevel enumLogLevel)
    {
        for (const auto &[nom, valeur] : NomsLogLevel)
            if (valeur == enumLogLevel) return Core::String(nom.data());
        return Core::String::Format("Invalid illegal unknown unrecognized unhandled garbage LogLevel {}", static_cast<int32_t>(enumLogLevel));
    }
}
