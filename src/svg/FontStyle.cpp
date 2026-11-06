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
