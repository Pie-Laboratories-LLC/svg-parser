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

#ifndef DRAW2D_SVG_SVGDOCUMENT_DOT_HPP
#define DRAW2D_SVG_SVGDOCUMENT_DOT_HPP

#ifndef DRAW2D_SVG_GRADIENTTEMPLATE_DOT_HPP
    #include "svg/GradientTemplate.hpp"
#endif
#ifndef DRAW2D_SVG_SVG_DOT_HPP
    #include "svg/Svg.hpp"
#endif

namespace Draw2d::Svg {

    class Svg;
    class SvgEntity;

    class SvgDocument {
    public:
        SvgDocument() = default;
        SvgDocument(const SvgDocument &copy) = delete;
        SvgDocument &operator=(const SvgDocument &copy) = delete;
        SvgDocument(SvgDocument &&move) = default;
        virtual ~SvgDocument() = default;

        bool hasRootSvg() const { return m_pSvg.get(); }
        Svg *setRootSvg(std::unique_ptr<Svg> pRootSvg) {
            if(m_pSvg.get()) throw SvgException("Root svg is already set!");
            m_pSvg = std::move(pRootSvg);
            return m_pSvg.get();
        }

        const Svg *getRootSvg() const {
            if(!m_pSvg.get()) throw SvgException("Root svg is _not_ set!");
            return m_pSvg.get();
        }

        /// non const version; owner of the svg owns it, so let her modify it if she knows what
        /// she's doing.
        Svg *getRootSvg() {
            if(!m_pSvg.get()) throw SvgException("Root svg is _not_ set!");
            return m_pSvg.get();
        }

        // I don't love this; the first bool indicates whether it was inserted,
        //  the second bool indicates whether it's a duplicate id.
        std::pair<bool,bool> saveSvgEntity(const SvgEntity *cpSvgEntity) {
            const Core::String &cstrId = cpSvgEntity->getId();
            if(Core::String::IsEmpty(cstrId)) return { false, false };
            auto [ it, bInserted ] = m_svgLookup.insert({ cstrId, cpSvgEntity });
            return { bInserted, bInserted ? false : true };
        }

        const SvgEntity *lookupSvgEntity(const Core::String &cstrId) const {
            if(!m_pSvg) throw SvgException("Root <svg...> hasn't been set");
            // d'ont forget to check the root element
            if(m_pSvg->getId() == cstrId) return m_pSvg.get();
            for(const auto &pEntity : m_pSvg->enumerateDescendants()) {
                if(pEntity->getId() == cstrId) return pEntity;
            }
            return nullptr;
        }

        std::vector<const SvgEntity *> getSvgEntities() const {
            if(!m_pSvg) throw SvgException("Root <svg...> hasn't been set");
            std::vector<const SvgEntity *> toReturn {};
            for(const auto &cpSvgEntity : m_pSvg-> enumerateDescendants()) toReturn.push_back(cpSvgEntity);
            return toReturn;
        }

        bool gradientExists(const Core::String &cstrId) const {
            return m_gradientLookup.contains(cstrId);
        }

        const GradientTemplate *lookupGradient(const Core::String &cstrId) const {
            if (auto search = m_gradientLookup.find(cstrId); search != m_gradientLookup.end()) {
                return (*search).second;
            }
            throw SvgException("No such Gradient with cstrId {}", cstrId.c_str());
        }

        const GradientTemplate *storeGradient(std::unique_ptr<GradientTemplate> pGradientTemplate)
        {
            const Core::String &cstrId = pGradientTemplate->strId;
            if(!Core::String::IsEmpty(cstrId)) {
                if(m_gradientLookup.contains(cstrId)) throw SvgException("Gradient {} already exists!", cstrId.c_str());
            }
            m_gradients.push_back(std::move(pGradientTemplate));
            const GradientTemplate *cpGradientTemplate = m_gradients.back().get();
            if(!Core::String::IsEmpty(cstrId)) {
                m_gradientLookup[cstrId] = cpGradientTemplate;
            }
            return cpGradientTemplate;
        }

        void appendStyle(const Core::String &cstrStyle) {
            m_strStyle += cstrStyle;
        }

        const Core::String &getStyle() const {
            return m_strStyle;
        }

        std::vector<const GradientTemplate *> getGradients() const {
            std::vector<const GradientTemplate *> toReturn {};
            std::transform(m_gradients.begin(), m_gradients.end(), std::back_inserter(toReturn),[](auto &g) {
                return g.get();
            });
            return toReturn;
        }

    private:
        std::unique_ptr<Svg> m_pSvg; // the root <svg...>

        std::map<Core::String, const SvgEntity *> m_svgLookup { };

        std::vector<std::unique_ptr<GradientTemplate>> m_gradients { };
        std::map<Core::String, const GradientTemplate *> m_gradientLookup { };

        Core::String m_strStyle {};
    }; // class SvgDocument
}

#endif  /* DRAW2D_SVG_SVGDOCUMENT_DOT_HPP */
