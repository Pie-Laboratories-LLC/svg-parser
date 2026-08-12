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

#ifndef DOMPARSER_DOT_HPP
#define DOMPARSER_DOT_HPP

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOM.hpp>               // DOMDocument, DOMElement, DOMNodeList, etc

#ifndef XML_STRING_DOT_HPP
    #include "xml/String.hpp"
#endif
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif

XERCES_CPP_NAMESPACE_BEGIN 
    class DOMDocument;
    class DOMElement;
XERCES_CPP_NAMESPACE_END

namespace Xml {

    class DomParser {
    public:
        DomParser() = default;

        XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument *Parse(const Core::String &rawXml, bool bDoNamespace = false, bool bDoSchema = false);

        static inline bool hasAttributeNS(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *pDomElement, const Core::String &cstrNamespaceUri, const Core::String &cstrAttributeName) {
            String strNamespaceUri(cstrNamespaceUri);
            String strAttributeName(cstrAttributeName);
            return pDomElement->hasAttributeNS(strNamespaceUri.c_str(),strAttributeName.c_str());
        }

        static inline bool hasAttribute(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *pDomElement, const Core::String &cstrAttributeName) {
            String strAttributeName(cstrAttributeName);
            return pDomElement->hasAttribute(strAttributeName.c_str());
        }

        static inline Core::String getAttributeIfExists(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *pDomElement, const Core::String &cstrAttributeName) {
            String strAttributeName(cstrAttributeName);
            if(!pDomElement->hasAttribute(strAttributeName.c_str())) return Core::String {};
            String strAttributeValue = pDomElement->getAttribute(strAttributeName.c_str());
            return Core::String(strAttributeValue.getTranscoded());
        }

        static inline Core::String getAttributeIfExistsNS(XERCES_CPP_NAMESPACE_QUALIFIER DOMElement *pDomElement, const Core::String &cstrNamespaceUri, const Core::String &cstrAttributeName) {
            String strNamespaceUri(cstrNamespaceUri);
            String strAttributeName(cstrAttributeName);
            if(!pDomElement->hasAttributeNS(strNamespaceUri.c_str(), strAttributeName.c_str())) return Core::String {};
            String strAttributeValue = pDomElement->getAttributeNS(strNamespaceUri.c_str(), strAttributeName.c_str());
            return Core::String(strAttributeValue.getTranscoded());
        }
    };
}

#endif  /* DOMPARSER_DOT_HPP */
