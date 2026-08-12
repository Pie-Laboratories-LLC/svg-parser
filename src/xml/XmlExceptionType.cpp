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

#include "xml/XmlExceptionType.hpp"

#ifndef CORE_STRING_DOT_HPP
    #include "core/String.hpp"
#endif

namespace Xml {

Core::String XmlExceptionTypeToString(XmlExceptionType enumXmlExceptionType) {
    switch(enumXmlExceptionType) {
        case NotAnXmlExceptionType: return "NotAnXmlExceptionType";
        case ParseError: return "ParseError";
        case DomError: return "DomError";
        case GeneralError: return "GeneralError";
        default: return "Invalid Unknown Illegal Unrecognized Garbage XmlExceptionType";
    }
}

}
