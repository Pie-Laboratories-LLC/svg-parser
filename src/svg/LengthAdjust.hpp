#ifndef DRAW2D_SVG_LENGTHADJUST_DOT_HPP
#define DRAW2D_SVG_LENGTHADJUST_DOT_HPP
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
namespace Draw2d::Svg {
    enum class LengthAdjust {
        NotALengthAdjust = -1,
        Spacing,
        SpacingAndGlyphs
    };
    LengthAdjust ParseLengthAdjust(const Core::String &cstrLengthAdjust);
    Core::String LengthAdjustToString(LengthAdjust enumLengthAdjust);
}
#endif  /* DRAW2D_SVG_LENGTHADJUST_DOT_HPP */
