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

#ifndef DRAW2D_SVG_SVGCONTAINERENTITY_DOT_HPP
#define DRAW2D_SVG_SVGCONTAINERENTITY_DOT_HPP

#ifndef DRAW2D_SVG_SVGENTITY_DOT_HPP
    #include "svg/SvgEntity.hpp"
#endif
#ifndef CORE_GENERATOR_DOT_HPP
    #include "core/Generator.hpp"
#endif

namespace Draw2d::Svg {
    struct SvgContainerParams: public SvgEntityParams {
    };

    class SvgContainerEntity: public SvgEntity {
    public:
        SvgContainerEntity(SvgContainerParams svgContainerParams):
            SvgEntity(std::move(svgContainerParams))
        {}
        virtual ~SvgContainerEntity() = default;

        virtual Core::Generator<const SvgEntity *> enumerateChildren() const = 0;

        Core::Generator<const SvgEntity *> enumerateDescendants() const
        {
            for (const SvgEntity *cpChild : enumerateChildren())
            {
                co_yield cpChild;
                if (auto *cpContainer = dynamic_cast<const SvgContainerEntity *>(cpChild))
                    for (const SvgEntity *cpDescendant : cpContainer->enumerateDescendants())
                        co_yield cpDescendant;
            }
        }

        virtual const SvgEntity *addChild(std::unique_ptr<SvgEntity> pSvgEntity) = 0;

        template <typename T>
        requires std::derived_from<T, SvgEntity>
        T *addChildAs(std::unique_ptr<T> pEntity)
        {
            T *pRaw = pEntity.get();
            addChild(std::move(pEntity));
            return pRaw;
        }

        virtual const char * const getType() const override { return "SvgContainerEntity"; }
    };
}

#endif  /* DRAW2D_SVG_SVGCONTAINERENTITY_DOT_HPP */
