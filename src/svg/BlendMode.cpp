#include <array>
#include <cstdint>
#include <utility>
#include "svg/BlendMode.hpp"
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
namespace Draw2d::Svg {
    namespace {
        // the CSS names, in enumeration order: one table for reading and writing, so the
        // two directions always agree
        constexpr std::array<std::pair<std::string_view, BlendMode>, 17> NomsBlendMode { {
            { "NotABlendMode", BlendMode::NotABlendMode },
            { "normal", BlendMode::Normal },
            { "multiply", BlendMode::Multiply },
            { "screen", BlendMode::Screen },
            { "overlay", BlendMode::Overlay },
            { "darken", BlendMode::Darken },
            { "lighten", BlendMode::Lighten },
            { "color-dodge", BlendMode::ColorDodge },
            { "color-burn", BlendMode::ColorBurn },
            { "hard-light", BlendMode::HardLight },
            { "soft-light", BlendMode::SoftLight },
            { "difference", BlendMode::Difference },
            { "exclusion", BlendMode::Exclusion },
            { "hue", BlendMode::Hue },
            { "saturation", BlendMode::Saturation },
            { "color", BlendMode::Color },
            { "luminosity", BlendMode::Luminosity },
        } };
    }

    std::optional<BlendMode> ChercherBlendMode(std::string_view texte)
    {
        for (const auto &[nom, valeur] : NomsBlendMode)
            if (nom == texte) return valeur;
        return std::nullopt;
    }

    BlendMode ParseBlendMode(const Core::String &cstrBlendMode)
    {
        if (const auto valeur = ChercherBlendMode(cstrBlendMode)) return *valeur;
        throw SvgException("Unrecognized blend mode `{}' -- normal, multiply, screen, overlay, darken, lighten, color-dodge, color-burn, hard-light, soft-light, difference, exclusion, hue, saturation, color, luminosity", cstrBlendMode.c_str());
    }

    Core::String BlendModeToString(BlendMode enumBlendMode)
    {
        for (const auto &[nom, valeur] : NomsBlendMode)
            if (valeur == enumBlendMode) return Core::String(nom.data());
        return Core::String::Format("Invalid illegal unknown unrecognized unhandled garbage BlendMode {}", static_cast<int32_t>(enumBlendMode));
    }
}
