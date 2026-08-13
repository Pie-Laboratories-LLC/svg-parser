#ifndef SRC_XML_DOMPARSERBUILDER_DOT_HPP
#define SRC_XML_DOMPARSERBUILDER_DOT_HPP

#include <memory>

namespace Xml {

    class IDomParser;

    class DomParserBuilder {
    public:
        static std::unique_ptr<IDomParser> makeDomParser();
    };

}

#endif  /* SRC_XML_DOMPARSERBUILDER_DOT_HPP */
