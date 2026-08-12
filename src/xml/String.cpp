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

#include <stdio.h>

#include "xml/String.hpp"

#ifndef XML_XMLEXCEPTION_DOT_HPP
    #include "xml/XmlException.hpp"
#endif

XERCES_CPP_NAMESPACE_USE;

namespace Xml {

    XMLTranscoder *String::sm_pUtf8Transcoder = NULL;

    String::String(const char * const cpszInitial) {
        this->__setString(cpszInitial);
    } // String::String(const char * const)

    String::String(const XMLCh * const cpszInitial)
    {
        this->__setString(cpszInitial);
    } // String::String(const XMLCh * const)

    String::String(int32_t nValue)

    {
        // TODO
        // stupid mb? i18n?
        char pszBuffer[32];
        snprintf(pszBuffer,sizeof(pszBuffer),"%d",nValue);
        XMLCh *pszUpcoded = XMLString::transcode(pszBuffer);
        m_strString.append(pszUpcoded);
        XMLString::release(&pszUpcoded);
    } // String::String(int32_t)

    String::String(const std::string &cstrString)

    {
        this->__setString(cstrString.c_str());
    }

    String::~String(void)
    {
        this->__deallocate();
    } // String::~String(void)

    void String::assign(const XMLCh *cpszString,size_t nLength)

    {
        this->__deallocate();
        m_strString.assign(cpszString,nLength);
    } // void String::assign(const XMLCh *,size_t)

    void String::setFromUtf8(const unsigned char * const cpszString,size_t nLength)

    {
        this->__deallocate();
        if(String::sm_pUtf8Transcoder == NULL) {
            XMLTransService::Codes enumFailReason;

            String::sm_pUtf8Transcoder = XMLPlatformUtils::fgTransService->
                makeNewTranscoderFor(
                    "UTF-8"
                   ,enumFailReason
                   ,1024
                );
            if(String::sm_pUtf8Transcoder == NULL) {
                throw XmlException( "Couldn't allocate from utf8 transcoder????");
            }
        }

        // this is the most unconscionable pain in the ass.
        if(nLength == 0) {
            const unsigned char *s = cpszString;
            while (*s) nLength += (*s++ & 0xc0) != 0x80;
        }
        XMLCh pszBuffer[1024];
        unsigned char pSizes[1024];
        m_strString.clear();
        size_t nStartOffset = 0;

        while(nStartOffset < nLength) {
            XMLSize_t nBytesEaten;
            XMLSize_t nBytesFilled;

            nBytesFilled = String::sm_pUtf8Transcoder->transcodeFrom(
                &cpszString[nStartOffset],nLength - nStartOffset
               ,pszBuffer,sizeof(pszBuffer) / sizeof(XMLCh)
               ,nBytesEaten,pSizes
            );
            nStartOffset += nBytesEaten;
            m_strString.append(pszBuffer,nBytesFilled);
        }
    } // void String::setFromUtf8(const unsigned char *,size_t)

    const unsigned char * const String::transcodeToUtf8(const char * const cpszLocal) {
        return NULL;
    } // const unsigned char * const String::transcodeToUtf8(const char * const)

    const char * const String::getTranscoded(void) const

    {
        if(m_pszTranscoded == NULL) m_pszTranscoded = XMLString::transcode(m_strString.c_str());
        return m_pszTranscoded;
    } // const char * const String::getTranscoded(void) const

    XMLCh String::operator[] (unsigned nIndex) const

    {
        if(nIndex >= m_strString.length()) {
            throw XmlException("Index {} out of range (length: {})",nIndex,length());
        }
        return m_strString[nIndex];
    } // XMLCh String::operator[] (unsigned) const

    XMLCh String::lastCharacter(void) const

    {
        if(!m_strString.length()) {
            throw XmlException("Can't get last character of an empty string!");
        }
        return m_strString[m_strString.length() - 1];
    } // XMLCh String::lastCharacter(void) const

    bool String::operator < (const String &cstrCompare) const

    {
        return m_strString < cstrCompare.m_strString;
    } // bool String::operator < (const String &) const

    bool String::operator <= (const String &cstrCompare) const

    {
        return m_strString <= cstrCompare.m_strString;
    } // bool String::operator <= (const String &) const

    bool String::operator == (const String &cstrCompare) const

    {
        return m_strString == cstrCompare.m_strString;
    } // bool String::operator == (const String &) const

    String &String::operator += (const String &cstrOther)

    {
        this->__deallocate();
        m_strString.append(cstrOther.m_strString);

        return *this;
    } // String &String::operator += (const String &)

    String &String::operator += (int32_t nValue)

    {
        String strValue(nValue);
        return *this += strValue;
    } // String &String::operator += (int32_t)

    String String::FromUtf8(const char *cpszUtf8)

    {
        String strReturn;
        strReturn.setFromUtf8(reinterpret_cast<const unsigned char *>(cpszUtf8),0);
        return strReturn;
    } // String String::FromUtf8(const char *)

    void String::trim_in_place(XMLCh cTrimCharacter)

    {
        // claude <3 -- unavoidable ....  I wrote the lambda :P
        auto firstNonCharacter = [cTrimCharacter](XMLCh c) { return (c != cTrimCharacter); };

        auto start = std::find_if(m_strString.begin(), m_strString.end(), firstNonCharacter);
        auto end = std::find_if(m_strString.rbegin(), m_strString.rend(), firstNonCharacter).base();

        this->__deallocate();
        if (start < end) m_strString = XMLChString(start, end);
        else m_strString.clear();
    }

    void String::trim_in_place(const XMLCh *cpszTrimCharacters)

    {
        auto start = m_strString.find_first_not_of(cpszTrimCharacters);
        auto end = m_strString.find_last_not_of(cpszTrimCharacters);

        this->__deallocate();
        if(start < end) m_strString = m_strString.substr(start, end);
        else m_strString.clear();
    }

    void String::trim_start_in_place(XMLCh cTrimCharacter)

    {
        auto firstNonCharacter = [cTrimCharacter](XMLCh c) { return (c != cTrimCharacter); };

        auto start = std::find_if(m_strString.begin(), m_strString.end(), firstNonCharacter);
        auto end = m_strString.end();

        this->__deallocate();
        if (start < end) m_strString = XMLChString(start, end);
        else m_strString.clear(); 
    }

    void String::trim_start_in_place(const XMLCh *cpszTrimCharacters)

    {
        auto start = m_strString.find_first_not_of(cpszTrimCharacters);

        this->__deallocate();
        if(start != XMLChString::npos) m_strString = m_strString.substr(start);
        else m_strString.clear();
    }

    void String::trim_end_in_place(XMLCh cTrimCharacter)

    {
        auto firstNonCharacter = [cTrimCharacter](XMLCh c) { return (c != cTrimCharacter); };

        auto start = m_strString.begin();
        auto end = std::find_if(m_strString.rbegin(), m_strString.rend(), firstNonCharacter).base();

        this->__deallocate();
        if (start < end) m_strString = XMLChString(start, end);
        else m_strString.clear(); 
    }

    void String::trim_end_in_place(const XMLCh *cpszTrimCharacters)

    {
        auto end = m_strString.find_last_not_of(cpszTrimCharacters);

        this->__deallocate();
        if(end != XMLChString::npos) m_strString = m_strString.substr(0, end);
        else m_strString.clear();
    }

    void String::__setString(const XMLCh * const cpszInitial)

    {
        this->__deallocate();
        m_strString = cpszInitial;
    } // void String::__setString(const XMLCh * const)

    void String::__setString(const char * const cpszInitial)

    {
        this->__deallocate();
        XMLCh *pszTranscoded = XMLString::transcode(cpszInitial);
        m_strString = pszTranscoded;
        XMLString::release(&pszTranscoded);
    } // void String::__setString(const char * const)

    void String::__deallocate(void) {
        if(m_pszTranscoded) XMLString::release(&m_pszTranscoded);
    }
} // namespace Core
