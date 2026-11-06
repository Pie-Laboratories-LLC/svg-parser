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

#ifndef DRAW2D_SVG_CLIPPATH_DOT_HPP
#define DRAW2D_SVG_CLIPPATH_DOT_HPP

#include <memory>
#include <vector>

#ifndef DRAW2D_SVG_SVGCONTAINERENTITY_DOT_HPP
    #include "svg/SvgContainerEntity.hpp"
#endif
#ifndef GRADIENTUNITS_DOT_HPP
    #include "draw2d/GradientUnits.hpp"
#endif

namespace Draw2d::Svg {

    struct SvgClipPathParams: public SvgContainerParams {
        // the same two values as gradientUnits
        GradientUnits clipPathUnits = UserSpaceOnUse;
    };

    /// <clipPath>: geometry, never drawn by itself.  An element whose clip-path property
    ///  names it shows only inside the union of its children's fill areas; the children's
    ///  paint, stroke and opacity don't matter.
    class ClipPath: public SvgContainerEntity
    {
    public:
        ClipPath(SvgClipPathParams svgClipPathParams):
            SvgContainerEntity(static_cast<SvgContainerParams &&>(svgClipPathParams))
           ,m_enumClipPathUnits(svgClipPathParams.clipPathUnits)
        {}
        virtual ~ClipPath() = default;

        GradientUnits getClipPathUnits() const { return m_enumClipPathUnits; }

        virtual SvgEntity *addChild(std::unique_ptr<SvgEntity> pSvgEntity) override {
            m_svgEntities.push_back(std::move(pSvgEntity));
            return m_svgEntities.back().get();
        }
        virtual Core::Generator<const SvgEntity *> enumerateChildren() const override {
            for(const auto &cpEntity : m_svgEntities) {
                co_yield cpEntity.get();
            }
        }

        virtual const char * const getType() const override { return "ClipPath"; }

    private:
        GradientUnits m_enumClipPathUnits;
        std::vector<std::unique_ptr<SvgEntity>> m_svgEntities { };
    };

} // namespace Draw2d::Svg

#endif  /* DRAW2D_SVG_CLIPPATH_DOT_HPP */
