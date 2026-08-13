#ifndef XML_IDOMPARSER_DOT_HPP
#define XML_IDOMPARSER_DOT_HPP

#include <memory>

namespace Core {
    class String;
}

namespace Xml {

    class IDomDocument;

    class IDomParser {
    public:
        virtual ~IDomParser() = default;
        virtual std::unique_ptr<IDomDocument> parse(const Core::String &rawXml, bool bDoNamespace = false, bool bDoSchema = false) = 0;
    };
}

#endif  /* XML_IDOMPARSER_DOT_HPP */
