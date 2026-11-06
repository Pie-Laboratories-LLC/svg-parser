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
