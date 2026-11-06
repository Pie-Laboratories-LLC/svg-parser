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

#ifndef XML_IDOMENTITY_DOT_HPP
#define XML_IDOMENTITY_DOT_HPP

#include <unordered_map>
#include <memory>

namespace Core {
    class String;
}

namespace Xml {

    class String;

    enum class DomEntityType { Text = 0, Entity };

    class IDomEntity {
    public:
        virtual ~IDomEntity() = default;

        virtual DomEntityType getDomEntityType() const = 0;
        virtual Core::String getTagName() const = 0;
        virtual Core::String getTextContent() const = 0;
        virtual std::unique_ptr<IDomEntity> getFirstChild(bool bSkipTextNodes = true) = 0;
        virtual std::unique_ptr<IDomEntity> getNextSibling(bool bSkipTextNodes = true) = 0;
        virtual bool hasAttribute(const Xml::String &cstrAttributeName) = 0;
        virtual bool hasAttributeNS(const Xml::String &cstrAttributeName, const Xml::String &cstrAttributeNamespace) = 0;
        virtual bool tryGetAttribute(const Xml::String &cstrAttributeName, Core::String &strAttributeValue) = 0;
        virtual bool tryGetAttributeNS(const Xml::String &cstrAttributeName, const Xml::String &cstrAttributeNamespace, Core::String &strAttributeValue) = 0;
        virtual std::unordered_map<Core::String,Core::String> getAttributes() const = 0;
    };

}

#endif  /* XML_IDOMENTITY_DOT_HPP */
