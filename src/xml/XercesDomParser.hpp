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

#ifndef XERCESDOMPARSER_DOT_HPP
#define XERCESDOMPARSER_DOT_HPP

#include <xercesc/util/XercesDefs.hpp>

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef XML_IDOMPARSER_DOT_HPP
    #include "xml/IDomParser.hpp"
#endif

XERCES_CPP_NAMESPACE_BEGIN 
    class DOMDocument;
    class DOMElement;
XERCES_CPP_NAMESPACE_END

namespace Xml {

    class IDomEntity;

    class XercesDomParser: public IDomParser {
    public:
        XercesDomParser() = default;

        virtual std::unique_ptr<IDomDocument> parse(const Core::String &rawXml, bool bDoNamespace = false, bool bDoSchema = false) override;
    };
}

#endif  /* XERCESDOMPARSER_DOT_HPP */
