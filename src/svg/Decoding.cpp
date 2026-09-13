#include "svg/Decoding.hpp"
#ifndef DRAW2D_SVG_SVGEXCEPTION_DOT_HPP
    #include "svg/SvgException.hpp"
#endif
#define DECODING_NOTADECODING "NotADecoding"
#define DECODING_ASYNC "async"
#define DECODING_SYNC "sync"
namespace Draw2d::Svg {
    Decoding ParseDecoding(const Core::String &cstrDecoding)
    {
        if (cstrDecoding == DECODING_NOTADECODING) return Decoding::NotADecoding;
        if (cstrDecoding == DECODING_ASYNC) return Decoding::Async;
        if (cstrDecoding == DECODING_SYNC) return Decoding::Sync;
        throw SvgException("Unrecognized decoding {}", cstrDecoding.c_str());
    }
    Core::String DecodingToString(Decoding enumDecoding)
    {
        switch(enumDecoding) {
        case Decoding::NotADecoding: return "NotADecoding";
        case Decoding::Async: return "Async";
        case Decoding::Sync: return "Sync";
        default: throw SvgException("Invalid illegal unknown unrecognized unhandled garbage Decoding {}", static_cast<int32_t>(enumDecoding));
        }
    }
}
