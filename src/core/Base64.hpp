#ifndef CORE_BASE64_DOT_HPP
#define CORE_BASE64_DOT_HPP

#include <vector>
#include <cstdint>

namespace Core {
    class String;

    std::vector<uint8_t> base64Decode(const Core::String &input);
}

#endif  /* CORE_BASE64_DOT_HPP */
