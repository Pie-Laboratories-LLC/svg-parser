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

#ifndef XML_IDOMPARSER_DOT_HPP
#define XML_IDOMPARSER_DOT_HPP

#include <memory>

namespace Core {
    class String;
}

namespace Xml {

    class IDomDocument;

    class IDomParser {
    public:
        virtual ~IDomParser() = default;
        virtual std::unique_ptr<IDomDocument> parse(const Core::String &rawXml, bool bDoNamespace = false, bool bDoSchema = false) = 0;
    };
}

#endif  /* XML_IDOMPARSER_DOT_HPP */
