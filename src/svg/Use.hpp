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

#ifndef USE_DOT_HPP
#define USE_DOT_HPP

#ifndef SVGENTITY_DOT_HPP
    #include "svg/SvgContainerEntity.hpp"
#endif
#ifndef DRAW2D_SVG_SVGDIMENSIONEDENTITY_DOT_HPP
    #include "svg/SvgDimensionedEntity.hpp"
#endif

namespace Draw2d::Svg {

    struct SvgUseParams : public SvgContainerParams {
        SvgDimensionedParams svgDimensionedParams;
        const SvgEntity *cpSvgEntity;
    };

    class Use  : public SvgContainerEntity
    {
    public:
        Use (SvgUseParams svgUseParams):
            SvgContainerEntity(static_cast<SvgContainerParams &&>(svgUseParams))
           ,m_svgDimensionedEntity(std::move(svgUseParams.svgDimensionedParams))
        {
        }
        virtual ~Use() = default;

        virtual bool getRender() const override { return SvgContainerEntity::getRender() && m_pSvgEntity.get(); }
        std::optional<Dimension> getX() const { return m_svgDimensionedEntity.getX(); }
        std::optional<Dimension> getY() const { return m_svgDimensionedEntity.getY(); }
        std::optional<Dimension> getWidth() const { return m_svgDimensionedEntity.getWidth(); }
        std::optional<Dimension> getHeight() const { return m_svgDimensionedEntity.getHeight(); }
        const SvgDimensionedEntity *getDimensionedEntity() const override { return &m_svgDimensionedEntity; }

        virtual const SvgEntity *addChild(std::unique_ptr<SvgEntity> pSvgEntity) override {
            if(m_pSvgEntity) throw SvgException("addChild called when Use's child is already set!");
            if(!pSvgEntity.get()) throw SvgException("setSvgEntity called with null entity!");
            m_pSvgEntity = std::move(pSvgEntity);
            m_upperLeft.setX(m_pSvgEntity.get()->getUpperLeft().getX());
            m_upperLeft.setY(m_pSvgEntity.get()->getUpperLeft().getY());
            m_lowerRight.setX(m_pSvgEntity.get()->getLowerRight().getX());
            m_lowerRight.setY(m_pSvgEntity.get()->getLowerRight().getY());
            return m_pSvgEntity.get();
        }

        bool hasSvgEntity() const { return m_pSvgEntity.get(); }

        virtual Core::Generator<const SvgEntity *> enumerateChildren() const override {
            // if there were justice in the world this would throw.  alas for UI ppl developing APIs
            // if(!m_pSvgEntity.get()) throw SvgException("Use' svgEntity hasn't been set yet!");
            if(!m_pSvgEntity.get()) co_return;
            co_yield m_pSvgEntity.get();
        }

        virtual const char * const getType() const override { return "Use"; }

    private:
        SvgDimensionedEntity m_svgDimensionedEntity;
        std::unique_ptr<SvgEntity> m_pSvgEntity = nullptr;
        std::optional<float> m_fX {};
        std::optional<float> m_fY {};
        std::optional<float> m_fWidth {};
        std::optional<float> m_fHeight {};
    }; // class Use  : public SvgEntity

} // namespace Draw2d::Svg

#endif  /* USE_DOT_HPP */
