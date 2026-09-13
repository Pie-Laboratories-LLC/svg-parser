#include "svg/CrossOrigin.hpp"
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
#define CROSSORIGIN_NOTACROSSORIGIN "NotACrossOrigin"
#define CROSSORIGIN_ANONYMOUS "anonymous"
#define CROSSORIGIN_USECREDENTIALS "use-credentials"
namespace Draw2d::Svg {
    CrossOrigin ParseCrossOrigin(const Core::String &cstrCrossOrigin)
    {
        if (cstrCrossOrigin == CROSSORIGIN_NOTACROSSORIGIN) return CrossOrigin::NotACrossOrigin;
        if (cstrCrossOrigin == CROSSORIGIN_ANONYMOUS) return CrossOrigin::Anonymous;
        if (cstrCrossOrigin == CROSSORIGIN_USECREDENTIALS) return CrossOrigin::UseCredentials;
        throw SvgException("Unrecognized cross origin {}", cstrCrossOrigin.c_str());
    }
    Core::String CrossOriginToString(CrossOrigin enumCrossOrigin)
    {
        switch(enumCrossOrigin) {
        case CrossOrigin::NotACrossOrigin: return "NotACrossOrigin";
        case CrossOrigin::Anonymous: return "Anonymous";
        case CrossOrigin::UseCredentials: return "UseCredentials";
        default: throw SvgException("Invalid illegal unknown unrecognized unhandled garbage CrossOrigin {}", static_cast<int32_t>(enumCrossOrigin));
        }
    }
}
