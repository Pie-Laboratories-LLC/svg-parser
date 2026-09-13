#ifndef DRAW2D_SVG_CROSSORIGIN_DOT_HPP
#define DRAW2D_SVG_CROSSORIGIN_DOT_HPP
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
namespace Draw2d::Svg {
    enum class CrossOrigin {
        NotACrossOrigin = -1,
        Anonymous,
        UseCredentials
    };
    CrossOrigin ParseCrossOrigin(const Core::String &cstrCrossOrigin);
    Core::String CrossOriginToString(CrossOrigin enumCrossOrigin);
}
#endif  /* DRAW2D_SVG_CROSSORIGIN_DOT_HPP */
