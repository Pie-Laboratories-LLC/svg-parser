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
#include <cstdint>
#include <utility>
#include "svg/TextAnchor.hpp"
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
namespace Draw2d::Svg {
    namespace {
        // les noms écrits, dans l'ordre de l'énumération: une seule table pour lire et
        // pour écrire -- les deux sens s'accordent toujours
        constexpr std::array<std::pair<std::string_view, TextAnchor>, 4> NomsTextAnchor { {
            { "NotATextAnchor", TextAnchor::NotATextAnchor },
            { "start", TextAnchor::Start },
            { "middle", TextAnchor::Middle },
            { "end", TextAnchor::End },
        } };
    }

    std::optional<TextAnchor> ChercherTextAnchor(std::string_view texte)
    {
        for (const auto &[nom, valeur] : NomsTextAnchor)
            if (nom == texte) return valeur;
        return std::nullopt;
    }

    TextAnchor ParseTextAnchor(const Core::String &cstrTextAnchor)
    {
        if (const auto valeur = ChercherTextAnchor(cstrTextAnchor)) return *valeur;
        throw SvgException("Unrecognized text anchor « {} » -- start, middle, end", cstrTextAnchor.c_str());
    }

    Core::String TextAnchorToString(TextAnchor enumTextAnchor)
    {
        for (const auto &[nom, valeur] : NomsTextAnchor)
            if (valeur == enumTextAnchor) return Core::String(nom.data());
        return Core::String::Format("Invalid illegal unknown unrecognized unhandled garbage TextAnchor {}", static_cast<int32_t>(enumTextAnchor));
    }
}
