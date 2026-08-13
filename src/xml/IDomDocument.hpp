#ifndef XML_IDOMDOCUMENT_DOT_HPP
#define XML_IDOMDOCUMENT_DOT_HPP

#include <memory>

namespace Xml {

    class IDomEntity;
    class String;

    class IDomDocument {
    public:
        virtual ~IDomDocument() = default;

        virtual std::unique_ptr<IDomEntity> getRootEntity() = 0;
        virtual std::shared_ptr<IDomEntity> getElementById(const Xml::String &cstrId) = 0;
    };

}

#endif  /* XML_IDOMDOCUMENT_DOT_HPP */
