#ifndef DRAW2D_SVG_DECODING_DOT_HPP
#define DRAW2D_SVG_DECODING_DOT_HPP
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
namespace Draw2d::Svg {
    enum class Decoding {
        NotADecoding = -1,
        Async,
        Sync
    };
    Decoding ParseDecoding(const Core::String &cstrDecoding);
    Core::String DecodingToString(Decoding enumDecoding);
}
#endif  /* DRAW2D_SVG_DECODING_DOT_HPP */
