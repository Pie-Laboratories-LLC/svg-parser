#include <xercesc/dom/DOM.hpp>               // DOMDocument, DOMElement, DOMNodeList, etc

#include "xml/XercesDomDocument.hpp"
#ifndef XML_XMLEXCEPTION_DOT_HPP
    #include "xml/XmlException.hpp"
#endif

namespace Xml {

    XercesDomDocument::XercesDomDocument(xc::DOMDocument *pDomDocument)
    {
        m_pDomDocument = pDomDocument;
    }

    XercesDomDocument::~XercesDomDocument()
    {
        if(m_pDomDocument) m_pDomDocument->release();
        m_pDomDocument = nullptr;
    }

    std::unique_ptr<IDomEntity> XercesDomDocument::getRootEntity()
    {
        xc::DOMElement *pRootElement = m_pDomDocument->getDocumentElement();

        return std::make_unique<XercesDomEntity>(pRootElement);
    }

    std::shared_ptr<IDomEntity> XercesDomDocument::getElementById(const Xml::String &cstrId)
    {
        // irksome that xerces doesn't recognize id attributes without validation,
        //  let's at least cache the results
        if(m_elementByIdCache.contains(cstrId)) return m_elementByIdCache[cstrId];
        XercesDomEntity rootElement { m_pDomDocument->getDocumentElement() };
        std::shared_ptr<IDomEntity> pResult =  __recurseForId(&rootElement, cstrId);
        if(pResult) {
            auto [it, bInserted] = m_elementByIdCache.insert({ cstrId, std::move(pResult) });
            if(!bInserted) throw XmlException("Internal error.");
            return it->second;
        }
        return pResult;
    }

    std::shared_ptr<IDomEntity> XercesDomDocument::__recurseForId(XercesDomEntity *pRootEntity, const Xml::String &cstrId)
    {
        Core::String strId { };
        if(pRootEntity->tryGetAttribute(ID_ATTRIBUTE, strId)
        && strId == cstrId) return std::make_shared<XercesDomEntity>(pRootEntity->getDomNode());
        for(std::unique_ptr<IDomEntity> pChildNode = pRootEntity->getFirstChild();
            pChildNode; pChildNode = pChildNode->getNextSibling()) {
            std::shared_ptr<IDomEntity> pMatchingNode = __recurseForId(static_cast<XercesDomEntity *>(pChildNode.get()), cstrId);
            if(pMatchingNode) return pMatchingNode;
        }
        return nullptr;
    }

}
