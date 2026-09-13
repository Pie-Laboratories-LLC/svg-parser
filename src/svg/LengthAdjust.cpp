#include "svg/LengthAdjust.hpp"
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
#define LENGTHADJUST_NOTALENGTHADJUST "NotALengthAdjust"
#define LENGTHADJUST_SPACING "spacing"
#define LENGTHADJUST_SPACINGANDGLYPHS "spacingAndGlyphs"
namespace Draw2d::Svg {
    LengthAdjust ParseLengthAdjust(const Core::String &cstrLengthAdjust)
    {
        if (cstrLengthAdjust == LENGTHADJUST_NOTALENGTHADJUST) return LengthAdjust::NotALengthAdjust;
        if (cstrLengthAdjust == LENGTHADJUST_SPACING) return LengthAdjust::Spacing;
        if (cstrLengthAdjust == LENGTHADJUST_SPACINGANDGLYPHS) return LengthAdjust::SpacingAndGlyphs;
        throw SvgException("Unrecognized length adjust {}", cstrLengthAdjust.c_str());
    }
    Core::String LengthAdjustToString(LengthAdjust enumLengthAdjust)
    {
        switch(enumLengthAdjust) {
        case LengthAdjust::NotALengthAdjust: return "NotALengthAdjust";
        case LengthAdjust::Spacing: return "Spacing";
        case LengthAdjust::SpacingAndGlyphs: return "SpacingAndGlyphs";
        default: throw SvgException("Invalid illegal unknown unrecognized unhandled garbage LengthAdjust {}", static_cast<int32_t>(enumLengthAdjust));
        }
    }
}
