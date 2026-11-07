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

#ifndef DRAW2D_SVG_FONTSTYLE_DOT_HPP
#define DRAW2D_SVG_FONTSTYLE_DOT_HPP
#include <optional>
#include <string_view>
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
namespace Draw2d::Svg {
    enum class FontStyle {
        NotAFontStyle = -1,
        Normal,
        Italic,
        Oblique
    };
    // d'après son nom écrit, sans lancer: rien s'il n'en est pas un
    std::optional<FontStyle> ChercherFontStyle(std::string_view texte);
    // de même, mais un nom inconnu lance -- en nommant ceux qui existent
    FontStyle ParseFontStyle(const Core::String &cstrFontStyle);
    // son nom écrit, celui que Chercher et Parse lisent -- sans lancer, même pour une
    // valeur hors de l'énumération
    Core::String FontStyleToString(FontStyle enumFontStyle);
}
#endif  /* DRAW2D_SVG_FONTSTYLE_DOT_HPP */
