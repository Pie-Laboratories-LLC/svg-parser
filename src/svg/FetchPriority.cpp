#include "svg/FetchPriority.hpp"
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
#define FETCHPRIORITY_NOTAFETCHPRIORITY "NotAFetchPriority"
#define FETCHPRIORITY_HIGH "high"
#define FETCHPRIORITY_LOW "low"
namespace Draw2d::Svg {
    FetchPriority ParseFetchPriority(const Core::String &cstrFetchPriority)
    {
        if (cstrFetchPriority == FETCHPRIORITY_NOTAFETCHPRIORITY) return FetchPriority::NotAFetchPriority;
        if (cstrFetchPriority == FETCHPRIORITY_HIGH) return FetchPriority::High;
        if (cstrFetchPriority == FETCHPRIORITY_LOW) return FetchPriority::Low;
        throw SvgException("Unrecognized fetch priority {}", cstrFetchPriority.c_str());
    }
    Core::String FetchPriorityToString(FetchPriority enumFetchPriority)
    {
        switch(enumFetchPriority) {
        case FetchPriority::NotAFetchPriority: return "NotAFetchPriority";
        case FetchPriority::High: return "High";
        case FetchPriority::Low: return "Low";
        default: throw SvgException("Invalid illegal unknown unrecognized unhandled garbage FetchPriority {}", static_cast<int32_t>(enumFetchPriority));
        }
    }
}
