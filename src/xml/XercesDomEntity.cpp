#include <xercesc/dom/DOM.hpp>               // xc::DOMDocument, xc::DOMElement, DOMNodeList, etc
#include "xml/XercesDomEntity.hpp"

#ifndef XML_STRING_DOT_HPP
    #include "xml/String.hpp"
#endif
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif

namespace Xml {

    XERCES_CPP_NAMESPACE_USE

    XercesDomEntity::XercesDomEntity(xc::DOMNode *pDomNode)
    {
        m_pDomNode = pDomNode;
    }

    Core::String XercesDomEntity::getTagName() const
    {
        String strTagName = m_pDomNode->getNodeName();
        return strTagName.getTranscoded();
    }

    Core::String XercesDomEntity::getTextContent() const
    {
        String strTextContent = m_pDomNode->getTextContent();
        return strTextContent.getTranscoded();
    }

    std::unique_ptr<IDomEntity> XercesDomEntity::getFirstChild()
    {
        DOMNode *pChildNode = m_pDomNode->getFirstChild();
        DOMElement *pChildElement = __getFirstElement(pChildNode);
        if(pChildElement) return std::make_unique<XercesDomEntity>(pChildElement);
        return nullptr;
    }

    std::unique_ptr<IDomEntity> XercesDomEntity::getNextSibling()
    {
        xc::DOMNode *pDomNode = __getFirstElement(m_pDomNode->getNextSibling());

        return pDomNode ? std::make_unique<XercesDomEntity>(pDomNode) : nullptr;
    }

    bool XercesDomEntity::hasAttribute(const Xml::String &cstrAttributeName)
    {
        String strAttributeName(cstrAttributeName);
        return static_cast<DOMElement *>(m_pDomNode)->hasAttribute(strAttributeName.c_str());
    }

    bool XercesDomEntity::hasAttributeNS(const Xml::String &cstrAttributeName, const Xml::String &cstrAttributeNamespace)
    {
        String strNamespaceUri(cstrAttributeNamespace);
        String strAttributeName(cstrAttributeName);
        return static_cast<DOMElement *>(m_pDomNode)->hasAttributeNS(strNamespaceUri.c_str(),strAttributeName.c_str());
    }

    bool XercesDomEntity::tryGetAttribute(const Xml::String &cstrAttributeName, Core::String &strAttributeValue)
    {
        String strAttributeName(cstrAttributeName);
        if(!static_cast<DOMElement*>(m_pDomNode)->hasAttribute(strAttributeName.c_str())) return false;
        Xml::String strWideAttributeValue = static_cast<DOMElement*>(m_pDomNode)->getAttribute(strAttributeName.c_str());
        strAttributeValue.assign(strWideAttributeValue.getTranscoded());
        return true;
    }

    bool XercesDomEntity::tryGetAttributeNS(const Xml::String &cstrAttributeName, const Xml::String &cstrNamespaceUri, Core::String &strAttributeValue)
    {
        String strNamespaceUri(cstrNamespaceUri);
        String strAttributeName(cstrAttributeName);
        if(!static_cast<DOMElement *>(m_pDomNode)->hasAttributeNS(strNamespaceUri.c_str(), strAttributeName.c_str())) return false;
        String strWideAttributeValue = static_cast<DOMElement *>(m_pDomNode)->getAttributeNS(strNamespaceUri.c_str(), strAttributeName.c_str());
        strAttributeValue.assign(strWideAttributeValue.getTranscoded());
        return true;
    }

    std::unordered_map<Core::String,Core::String> XercesDomEntity::getAttributes() const
    {
        std::unordered_map<Core::String,Core::String> toReturn { };
        auto nodeAttributes = m_pDomNode->getAttributes();
        for (unsigned i = 0; i < nodeAttributes->getLength(); i++) {
            DOMNode *pAttribute = nodeAttributes->item(i);
            Core::String strAttributeName = String(pAttribute->getNodeName()).getTranscoded();
            Core::String strAttributeValue = String(pAttribute->getNodeValue()).getTranscoded();
            toReturn.insert({ strAttributeName,strAttributeValue });
        }

        return toReturn;
    }

    DOMElement *XercesDomEntity::__getFirstElement(DOMNode *pChildNode)
    {
        while(pChildNode) {
            if (pChildNode->getNodeType() != DOMNode::ELEMENT_NODE) {
                pChildNode = pChildNode->getNextSibling();
                continue;
            }
            return static_cast<DOMElement*>(pChildNode);
        }

        return nullptr;
    }


}
