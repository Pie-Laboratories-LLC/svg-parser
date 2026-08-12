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

#include <xercesc/util/XMLString.hpp>       // XMLString::transcode/release
#include <xercesc/framework/MemBufInputSource.hpp> // MemBufInputSource
#include <xercesc/parsers/XercesDOMParser.hpp>     // XercesDOMParser
#include <xercesc/dom/DOM.hpp>               // DOMDocument, DOMElement, DOMNodeList, etc

#include "xml/DomParser.hpp"

#ifndef XMLEXCEPTION_DOT_HPP
    #include "xml/XmlException.hpp"
#endif

namespace Xml {
    
    XERCES_CPP_NAMESPACE_USE

    DOMDocument *DomParser::Parse(const Core::String &rawXml, bool bDoNamespace, bool bDoSchema) {
        // thx claude <3
        XercesDOMParser parser;
        parser.setDoNamespaces(bDoNamespace);
        // if they're requesting schema validation, have to set the validation scheme appropriately;
        //  we don't know whether the document has a schema or not
        parser.setValidationScheme(bDoSchema ? XercesDOMParser::Val_Auto : XercesDOMParser::Val_Never);
        parser.setDoSchema(bDoSchema);

        // Wrap the std::string in a MemBufInputSource so the parser can read it
        MemBufInputSource xmlBuf(
            reinterpret_cast<const XMLByte*>(rawXml.c_str()),
            rawXml.length(),
            "xml-in-memory-buffer" // an arbitrary system ID/label
        );

        try {
            parser.parse(xmlBuf);
        }
        catch (const XMLException& e) {
            char* pszXercesMessage = XMLString::transcode(e.getMessage());
            Core::String strExceptionMessage = Core::String("Parse error ") + pszXercesMessage;
            XMLString::release(&pszXercesMessage);
            throw XmlException(ParseError, strExceptionMessage);
        }
        catch (const DOMException& e) {
            char* pszXercesMessage = XMLString::transcode(e.getMessage());
            Core::String strExceptionMessage = Core::String("DOM error ") + pszXercesMessage;
            XMLString::release(&pszXercesMessage);
            throw XmlException(DomError, strExceptionMessage);
        }
        catch(const std::exception &exception) {
            Core::String strExceptionMessage = Core::String("General error ") + exception.what();
            throw XmlException(GeneralError, strExceptionMessage);
        }
        catch(...) {
            Core::String strExceptionMessage = "General error, unknown exception";
            throw XmlException(GeneralError, strExceptionMessage);
        }

        DOMDocument *document = parser.adoptDocument();
        if(!document) throw XmlException(GeneralError, "Failed to parse document");

        return document;
    } // DOMDocument *DOMParser::Parse(const std::string &rawXml)
}
