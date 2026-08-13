#ifndef SRC_XML_XERCESDOMENTITY_DOT_HPP
#define SRC_XML_XERCESDOMENTITY_DOT_HPP

#include <unordered_map>
#include <xercesc/util/XercesDefs.hpp>

#ifndef XML_IDOMENTITY_DOT_HPP
    #include "xml/IDomEntity.hpp"
#endif
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef XML_STRING_DOT_HPP
    #include "xml/String.hpp"
#endif

XERCES_CPP_NAMESPACE_BEGIN 
    class DOMDocument;
    class DOMElement;
    class DOMEntity;
    class DOMNode;
XERCES_CPP_NAMESPACE_END

namespace Xml {

    namespace xc = XERCES_CPP_NAMESPACE;

    class XercesDomEntity : public IDomEntity
    {
    public:
        XercesDomEntity(xc::DOMNode *pDomNode);
        virtual ~XercesDomEntity() = default;

        virtual Core::String getTagName() const override;
        virtual Core::String getTextContent() const override;
        virtual std::unique_ptr<IDomEntity> getFirstChild() override;
        virtual std::unique_ptr<IDomEntity> getNextSibling() override;
        virtual bool hasAttribute(const Xml::String &cstrAttributeName) override;
        virtual bool hasAttributeNS(const Xml::String &cstrAttributeName, const Xml::String &cstrAttributeNamespace) override;
        virtual bool tryGetAttribute(const Xml::String &cstrAttributeName, Core::String &strAttributeValue) override;
        virtual bool tryGetAttributeNS(const Xml::String &cstrAttributeName, const Xml::String &cstrAttributeNamespaceUri, Core::String &strAttributeValue) override;
        virtual std::unordered_map<Core::String,Core::String> getAttributes() const override;

        xc::DOMNode *getDomNode() { return m_pDomNode; }

    private:
        xc::DOMNode *m_pDomNode;
        xc::DOMElement *__getFirstElement(xc::DOMNode *pChildNode);
    };
}

#endif  /* SRC_XML_XERCESDOMENTITY_DOT_HPP */
