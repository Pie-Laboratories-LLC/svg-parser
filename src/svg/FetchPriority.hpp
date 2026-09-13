#ifndef DRAW2D_SVG_FETCHPRIORITY_DOT_HPP
#define DRAW2D_SVG_FETCHPRIORITY_DOT_HPP
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
namespace Draw2d::Svg {
    enum class FetchPriority {
        NotAFetchPriority = -1,
        High,
        Low
    };
    FetchPriority ParseFetchPriority(const Core::String &cstrFetchPriority);
    Core::String FetchPriorityToString(FetchPriority enumFetchPriority);
}
#endif  /* DRAW2D_SVG_FETCHPRIORITY_DOT_HPP */
