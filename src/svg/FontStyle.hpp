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
