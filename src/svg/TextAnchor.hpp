#ifndef DRAW2D_SVG_TEXTANCHOR_DOT_HPP
#define DRAW2D_SVG_TEXTANCHOR_DOT_HPP
#include <optional>
#include <string_view>
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
namespace Draw2d::Svg {
    enum class TextAnchor {
        NotATextAnchor = -1,
        Start,
        Middle,
        End
    };
    // d'après son nom écrit, sans lancer: rien s'il n'en est pas un
    std::optional<TextAnchor> ChercherTextAnchor(std::string_view texte);
    // de même, mais un nom inconnu lance -- en nommant ceux qui existent
    TextAnchor ParseTextAnchor(const Core::String &cstrTextAnchor);
    // son nom écrit, celui que Chercher et Parse lisent -- sans lancer, même pour une
    // valeur hors de l'énumération
    Core::String TextAnchorToString(TextAnchor enumTextAnchor);
}
#endif  /* DRAW2D_SVG_TEXTANCHOR_DOT_HPP */
