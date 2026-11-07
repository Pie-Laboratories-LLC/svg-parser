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
#include "svg/FontStyle.hpp"
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
namespace Draw2d::Svg {
    namespace {
        // les noms écrits, dans l'ordre de l'énumération: une seule table pour lire et
        // pour écrire -- les deux sens s'accordent toujours
        constexpr std::array<std::pair<std::string_view, FontStyle>, 4> NomsFontStyle { {
            { "NotAFontStyle", FontStyle::NotAFontStyle },
            { "normal", FontStyle::Normal },
            { "italic", FontStyle::Italic },
            { "oblique", FontStyle::Oblique },
        } };
    }

    std::optional<FontStyle> ChercherFontStyle(std::string_view texte)
    {
        for (const auto &[nom, valeur] : NomsFontStyle)
            if (nom == texte) return valeur;
        return std::nullopt;
    }

    FontStyle ParseFontStyle(const Core::String &cstrFontStyle)
    {
        if (const auto valeur = ChercherFontStyle(cstrFontStyle)) return *valeur;
        throw SvgException("Unrecognized font style « {} » -- normal, italic, oblique", cstrFontStyle.c_str());
    }

    Core::String FontStyleToString(FontStyle enumFontStyle)
    {
        for (const auto &[nom, valeur] : NomsFontStyle)
            if (valeur == enumFontStyle) return Core::String(nom.data());
        return Core::String::Format("Invalid illegal unknown unrecognized unhandled garbage FontStyle {}", static_cast<int32_t>(enumFontStyle));
    }
}
