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

#ifndef XML_XERCESDOMDOCUMENT_DOT_HPP
#define XML_XERCESDOMDOCUMENT_DOT_HPP

#ifndef XML_IDOMDOCUMENT_DOT_HPP
    #include "xml/IDomDocument.hpp"
#endif
#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif
#ifndef XML_STRING_DOT_HPP
    #include "xml/String.hpp"
#endif
#ifndef XML_XERCESDOMENTITY_DOT_HPP
    #include "xml/XercesDomEntity.hpp"
#endif

namespace Xml {

    class XercesDomDocument: public IDomDocument
    {
    public:
        const static inline Core::String ID_ATTRIBUTE = "id";

        XercesDomDocument(xc::DOMDocument *pDomDocument);
        virtual ~XercesDomDocument();

        virtual std::unique_ptr<IDomEntity> getRootEntity() override;
        virtual std::shared_ptr<IDomEntity> getElementById(const Xml::String &cstrId) override;

    private:
        xc::DOMDocument *m_pDomDocument = nullptr;
        std::unordered_map<Xml::String,std::shared_ptr<Xml::IDomEntity>> m_elementByIdCache {};
        std::shared_ptr<IDomEntity> __recurseForId(XercesDomEntity *pRootEntity, const Xml::String &cstrId);
    };
}


#endif  /* XML_XERCESDOMDOCUMENT_DOT_HPP */
