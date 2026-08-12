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

#ifndef XML_STRING_DOT_HPP
#define XML_STRING_DOT_HPP

#include <string>
#include <algorithm>
#include <format>
#include <xercesc/util/TransService.hpp>

typedef std::basic_string<XMLCh> XMLChString;
typedef std::basic_string<char> CharString;

namespace Xml {

    class String {
    public:
        String() = default;

        String(const char * const cpszInitial);
        String(const XMLCh * const cpszInitial);
        String(const String &cstrString): m_strString(cstrString.m_strString), m_pszTranscoded(nullptr) {}
        String(String &&strMove): m_strString(std::move(strMove.m_strString)), m_pszTranscoded(strMove.m_pszTranscoded) {
            strMove.m_pszTranscoded = nullptr;
        }
        String(int32_t nValue);
        String(const std::string &cstrString);
        template<typename... Args>
        String(std::format_string<Args...> fmt, Args&&... args)
            : String(std::format(fmt, std::forward<Args>(args)...)) {}
        virtual ~String(void);

        XMLChString::size_type length(void) const { return m_strString.length(); }
        const XMLCh *c_str(void) const { return m_strString.c_str(); }

        void assign(const XMLCh *cpszString,size_t nLength);

        void setFromUtf8(const unsigned char * const cpszString,size_t nLength);
        static const unsigned char * const transcodeToUtf8(const char * const cpszLocal);

        const char * const getTranscoded(void) const;

        XMLCh operator[] (unsigned nIndex) const;
        XMLCh lastCharacter(void) const;

        bool operator < (const String &cstrCompare) const;
        bool operator <= (const String &cstrCompare) const;
        bool operator == (const String &cstrCompare) const;

        String &operator += (const String &cstrOther);
        String &operator += (int32_t nValue);

        static String FromUtf8(const char *cpszUtf8);

        static inline size_t Utf8Length(const char *cpszUtf8) {
            size_t nLength = 0;
            const char *s = cpszUtf8;
            while (*s) nLength += (*s++ & 0xc0) != 0x80;

            return nLength;
        }

        String substr(size_t nStart, size_t nLength = XMLChString::npos) const
            { return String(m_strString.substr(nStart,nLength).c_str()); }

        size_t find_last_of(XMLCh cCharacter
                           ,size_t nPosition = XMLChString::npos) const
            { return m_strString.find_last_of(cCharacter,nPosition); }

        size_t find_last_of(const String &cstrFindCharacters
                           ,size_t nPosition = XMLChString::npos) const
            { return m_strString.find_last_of(cstrFindCharacters.c_str(),nPosition); }

        size_t find_last_not_of(XMLCh cCharacter
                               ,size_t nPosition = XMLChString::npos) const
            { return m_strString.find_last_not_of(cCharacter,nPosition); }

        size_t find_last_not_of(const String &cstrFindCharacters
                               ,size_t nPosition = XMLChString::npos) const
            { return m_strString.find_last_not_of(cstrFindCharacters.c_str(),nPosition); }

        size_t find_first_of(XMLCh cCharacter
                           ,size_t nPosition = XMLChString::npos) const
            { return m_strString.find_first_of(cCharacter,nPosition); }

        size_t find_first_of(const String &cstrFindCharacters
                           ,size_t nPosition = XMLChString::npos) const
            { return m_strString.find_first_of(cstrFindCharacters.c_str(),nPosition); }

        size_t find_first_not_of(XMLCh cCharacter
                               ,size_t nPosition = XMLChString::npos) const
            { return m_strString.find_first_not_of(cCharacter,nPosition); }

        size_t find_first_not_of(const String &cstrFindCharacters
                               ,size_t nPosition = XMLChString::npos) const
            { return m_strString.find_first_not_of(cstrFindCharacters.c_str(),nPosition); }

        bool starts_with(const String &cstrString) const {
            return m_strString.starts_with(cstrString.c_str());
        }

        explicit operator const char *() const {
            return getTranscoded();
        }

        explicit operator const XMLCh *() const {
            return m_strString.c_str();
        }

        // NOTE: claude says it's appropriate to std::move() inPlace here, but g++ insists it prevents copy elision.
        //  prolly never use another compiler, but worth noting.
        String trim(XMLCh cTrimCharacter) const
            { String inPlace(*this); inPlace.trim_in_place(cTrimCharacter); return inPlace; }
        String trim(const XMLCh *cpszTrimCharacters) const
            { String inPlace(*this); inPlace.trim_in_place(cpszTrimCharacters); return inPlace; }
        String trim(const String &cstrTrimCharacters = " \t\n\r\f\v") const
            { String inPlace(*this); inPlace.trim_in_place(cstrTrimCharacters.c_str()); return inPlace; }
        String trim_start(XMLCh cTrimCharacter) const
            { String inPlace(*this); inPlace.trim_start_in_place(cTrimCharacter); return inPlace; }
        String trim_start(const XMLCh *cpszTrimCharacters) const
            { String inPlace(*this); inPlace.trim_start_in_place(cpszTrimCharacters); return inPlace; }
        String trim_start(const String &cstrTrimCharacters = " \t\n\r\f\v") const
            { return trim_start(cstrTrimCharacters.c_str()); }
        String trim_end(XMLCh cTrimCharacter) const
            { String inPlace(*this); inPlace.trim_end_in_place(cTrimCharacter); return inPlace; }
        String trim_end(const XMLCh *cpszTrimCharacters) const
            { String inPlace(*this); inPlace.trim_end_in_place(cpszTrimCharacters); return inPlace; }
        String trim_end(const String &cstrTrimCharacters = " \t\n\r\f\v") const
            { String inPlace(*this); inPlace.trim_end_in_place(cstrTrimCharacters.c_str()); return inPlace; }

        void trim_in_place(XMLCh cTrimCharacter);
        void trim_in_place(const XMLCh *cpszTrimCharacters);
        void trim_in_place(const String &cstrTrimCharacters = " \t\n\r\f\v") { trim_in_place(cstrTrimCharacters.c_str()); }
        void trim_start_in_place(XMLCh cTrimCharacter);
        void trim_start_in_place(const XMLCh *cpszTrimCharacters);
        void trim_start_in_place(const String &cstrTrimCharacters = " \t\n\r\f\v") { trim_start_in_place(cstrTrimCharacters.c_str()); }
        void trim_end_in_place(XMLCh cTrimCharacter);
        void trim_end_in_place(const XMLCh *cpszTrimCharacters);
        void trim_end_in_place(const String &cstrTrimCharacters = " \t\n\r\f\v") { trim_end_in_place(cstrTrimCharacters.c_str()); }

        static bool IsEmpty(const String &strString) {
            return strString.m_strString.size() == 0;
        }

        static bool IsWhitespace(const String &strString) {
            return strString.m_strString.size() == 0 || std::all_of(strString.m_strString.begin(), strString.m_strString.end(), [](unsigned char c) {
                return std::isspace(c);
            });
        }

        XMLChString::iterator begin() { return m_strString.begin(); }
        XMLChString::const_iterator cbegin() const { return m_strString.cbegin(); }
        XMLChString::reverse_iterator rbegin() { return m_strString.rbegin(); }
        XMLChString::const_reverse_iterator crbegin() const { return m_strString.crbegin(); }

        XMLChString::iterator end() { return m_strString.end(); }
        XMLChString::const_iterator cend() const { return m_strString.cend(); }
        XMLChString::reverse_iterator rend() { return m_strString.rend(); }
        XMLChString::const_reverse_iterator crend() const { return m_strString.crend(); }

        template<typename T>
        requires std::ranges::range<T>
        static String Join(const T& elems, const String &delimiter) {
            String strResult;
            bool first = true;
            for (auto elem : elems) {
                if(!first) strResult += delimiter;
                first = false;
                strResult += elem;
            }
            return strResult;
        }

        void swap(String &other) noexcept {
            std::swap(m_strString, other.m_strString);
            std::swap(m_pszTranscoded, other.m_pszTranscoded);
        }

        String &operator =(String copy) noexcept {
            swap(copy);
            return *this;
        }

    private:
        XMLChString m_strString;
        mutable char *m_pszTranscoded = nullptr;

        void __setString(const XMLCh * const cpszInitial);
        void __setString(const char * const cpszInitial);

        void __deallocate(void);

        static XERCES_CPP_NAMESPACE::XMLTranscoder *sm_pUtf8Transcoder;
    }; // class String

} // namespace Xml

namespace std {
    template <>
    struct hash<Xml::String> {
        // thanks claude :P
        size_t operator()(const Xml::String& s) const noexcept {
            return std::hash<std::string_view>{}(std::string_view(s.getTranscoded()));
        }
    };
}

#endif  /* XML_STRING_DOT_HPP */
