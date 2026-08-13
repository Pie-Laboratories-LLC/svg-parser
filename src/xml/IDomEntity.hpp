#ifndef XML_IDOMENTITY_DOT_HPP
#define XML_IDOMENTITY_DOT_HPP

#include <unordered_map>
#include <memory>

namespace Core {
    class String;
}

namespace Xml {

    class String;

    class IDomEntity {
    public:
        virtual ~IDomEntity() = default;

        virtual Core::String getTagName() const = 0;
        virtual Core::String getTextContent() const = 0;
        virtual std::unique_ptr<IDomEntity> getFirstChild() = 0;
        virtual std::unique_ptr<IDomEntity> getNextSibling() = 0;
        virtual bool hasAttribute(const Xml::String &cstrAttributeName) = 0;
        virtual bool hasAttributeNS(const Xml::String &cstrAttributeName, const Xml::String &cstrAttributeNamespace) = 0;
        virtual bool tryGetAttribute(const Xml::String &cstrAttributeName, Core::String &strAttributeValue) = 0;
        virtual bool tryGetAttributeNS(const Xml::String &cstrAttributeName, const Xml::String &cstrAttributeNamespace, Core::String &strAttributeValue) = 0;
        virtual std::unordered_map<Core::String,Core::String> getAttributes() const = 0;
    };

}

#endif  /* XML_IDOMENTITY_DOT_HPP */
