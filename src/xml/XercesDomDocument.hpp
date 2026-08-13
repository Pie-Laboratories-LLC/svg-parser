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
