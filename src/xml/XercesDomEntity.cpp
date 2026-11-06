/*
 * Copyright 2026 Pie Laboratories
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

    DomEntityType XercesDomEntity::getDomEntityType() const
    {
        if(dynamic_cast<const xc::DOMElement *>(m_pDomNode)) return DomEntityType::Entity;
        return DomEntityType::Text;
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

    std::unique_ptr<IDomEntity> XercesDomEntity::getFirstChild(bool bSkipTextNodes)
    {
        DOMNode *pChildNode = m_pDomNode->getFirstChild();
        if(bSkipTextNodes) pChildNode = __getFirstElement(pChildNode);
        else pChildNode = __skipNonProcessed(pChildNode);

        if(pChildNode) return std::make_unique<XercesDomEntity>(pChildNode);
        return nullptr;
    }

    std::unique_ptr<IDomEntity> XercesDomEntity::getNextSibling(bool bSkipTextNodes)
    {
        xc::DOMNode *pDomNode = m_pDomNode->getNextSibling();
        if(bSkipTextNodes) pDomNode = __getFirstElement(pDomNode);
        else pDomNode = __skipNonProcessed(pDomNode);

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

    DOMElement *XercesDomEntity::__skipNonProcessed(DOMNode *pChildNode)
    {
        while(pChildNode) {
            if (pChildNode->getNodeType() != DOMNode::ELEMENT_NODE
             && pChildNode->getNodeType() != DOMNode::TEXT_NODE
             && pChildNode->getNodeType() != DOMNode::CDATA_SECTION_NODE) {
                pChildNode = pChildNode->getNextSibling();
                continue;
            }
            return static_cast<DOMElement*>(pChildNode);
        }

        return nullptr;
    }

}
