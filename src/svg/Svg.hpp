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

#ifndef DRAW2D_SVG_SVG_DOT_HPP
#define DRAW2D_SVG_SVG_DOT_HPP

#include <vector>

#ifndef SVGENTITY_DOT_HPP
    #include "svg/SvgEntity.hpp"
#endif
#ifndef DRAW2D_SVG_PRESERVEASPECTRATIO_DOT_HPP
    #include "svg/PreserveAspectRatio.hpp"
#endif
#ifndef DRAW2D_SVG_VIEWBOX_DOT_HPP
    #include "svg/Viewbox.hpp"
#endif
#ifndef SVGENTITY_DOT_HPP
    #include "svg/SvgContainerEntity.hpp"
#endif
#ifndef DRAW2D_SVG_SVGDIMENSIONEDENTITY_DOT_HPP
    #include "svg/SvgDimensionedEntity.hpp"
#endif

namespace Draw2d::Svg {

    struct SvgSvgParams : public SvgContainerParams {
        SvgDimensionedParams svgDimensionedParams;
        std::optional<Viewbox> viewbox {};
        PreserveAspectRatio preserveAspectRatio = PreserveAspectRatio::xMidYMid;
        PreserveAspectRatioMode preserveAspectRatioMode = PreserveAspectRatioMode::Meet;
        std::vector<const SvgEntity *> svgEntities;
    };

    class Svg  : public SvgContainerEntity
    {
    public:
        Svg (SvgSvgParams svgSvgParams):
            SvgContainerEntity(static_cast<SvgContainerParams &&>(svgSvgParams))
           ,m_svgDimensionedEntity(std::move(svgSvgParams.svgDimensionedParams))
           ,m_viewbox(svgSvgParams.viewbox)
           ,m_enumPreserveAspectRatio(svgSvgParams.preserveAspectRatio)
           ,m_enumPreserveAspectRatioMode(svgSvgParams.preserveAspectRatioMode)
            {}

        std::optional<Dimension> getX() const { return m_svgDimensionedEntity.getX(); }
        std::optional<Dimension> getY() const { return m_svgDimensionedEntity.getY(); }
        std::optional<Dimension> getWidth() const { return m_svgDimensionedEntity.getWidth(); }
        std::optional<Dimension> getHeight() const { return m_svgDimensionedEntity.getHeight(); }
        const SvgDimensionedEntity *getDimensionedEntity() const override { return &m_svgDimensionedEntity; }
        std::optional<Viewbox> getViewbox() const { return m_viewbox; }
        void setViewbox(Viewbox viewbox) { m_viewbox = viewbox; }
        PreserveAspectRatio m_preserveAspectRatio() const { return m_enumPreserveAspectRatio; }
        PreserveAspectRatioMode m_preserveAspectRatioMode() const { return m_enumPreserveAspectRatioMode; }

        std::unique_ptr<float[]> computeViewboxTransform(float fViewportX, float fViewportY,
                                                         float fViewportWidth, float fViewportHeight) const;
        virtual SvgEntity *addChild(std::unique_ptr<SvgEntity> pSvgEntity) override {
            m_svgEntities.push_back(std::move(pSvgEntity));
            return m_svgEntities.back().get();
        }

        virtual Core::Generator<const SvgEntity *> enumerateChildren() const override {
            for(const auto &cpEntity : m_svgEntities) {
                co_yield cpEntity.get();
            }
        }

        const SvgEntity *debugGetChild(unsigned nIndex);

        virtual const char * const getType() const override { return "Svg"; }

    private:
        SvgDimensionedEntity m_svgDimensionedEntity;
        std::vector<std::unique_ptr<SvgEntity>> m_svgEntities { };
        std::optional<Viewbox> m_viewbox {};
        PreserveAspectRatio m_enumPreserveAspectRatio = PreserveAspectRatio::xMidYMid;
        PreserveAspectRatioMode m_enumPreserveAspectRatioMode = PreserveAspectRatioMode::Meet;
    };
}

#endif  /* DRAW2D_SVG_SVG_DOT_HPP */
