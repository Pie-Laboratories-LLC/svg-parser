#include "xml/DomParserBuilder.hpp"

#ifndef XML_XERCESDOMPARSER_DOT_HPP
    #include "xml/XercesDomParser.hpp"
#endif

namespace Xml {

    std::unique_ptr<IDomParser> DomParserBuilder::makeDomParser()
    {
        return std::make_unique<XercesDomParser>();
    }

}
