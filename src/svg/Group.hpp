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

#ifndef GROUP_DOT_HPP
#define GROUP_DOT_HPP

#ifndef SVGENTITY_DOT_HPP
    #include "svg/SvgContainerEntity.hpp"
#endif

namespace Draw2d::Svg {

    struct SvgGroupParams: public SvgContainerParams {
        float fOpacity;
    };

    class Group: public SvgContainerEntity
    {
    public:
        float getOpacity() const { return m_fOpacity; }

        virtual ~Group() = default;
        Group(SvgGroupParams svgGroupParams):
            SvgContainerEntity(std::move(svgGroupParams))
           ,m_fOpacity(svgGroupParams.fOpacity)
        {
        }

        /*
        // note -- SvgEntity doesn't do &&, but that's ok.
        Group(const Group &&replace): SvgContainerEntity(replace) {
            m_svgEntities = std::move(replace.m_svgEntities);
            m_fOpacity = replace.m_fOpacity;
        }
        */

        virtual SvgEntity *addChild(std::unique_ptr<SvgEntity> pSvgEntity) override {
            m_svgEntities.push_back(std::move(pSvgEntity));
            SvgEntity *toReturn = m_svgEntities.back().get();

            // this actually doesn't work, I know what I was thinking when I did
            //  it, but most svg entities don't have bounds like this.
            if (toReturn->getUpperLeft().getX() < m_upperLeft.getX()) m_upperLeft.setX(toReturn->getUpperLeft().getX());
            if (toReturn->getUpperLeft().getY() < m_upperLeft.getY()) m_upperLeft.setY(toReturn->getUpperLeft().getY());
            if (toReturn->getLowerRight().getX() > m_lowerRight.getX()) m_lowerRight.setX(toReturn->getLowerRight().getX());
            if (toReturn->getLowerRight().getY() > m_lowerRight.getY()) m_lowerRight.setY(toReturn->getLowerRight().getY());

            return toReturn;
        }

        virtual Core::Generator<const SvgEntity *> enumerateChildren() const override {
            for(const auto &cpEntity : m_svgEntities) {
                co_yield cpEntity.get();
            }
        }

        const SvgEntity *debugGetChild(unsigned nChildIndex);

        virtual const char * const getType() const override { return "Group"; }

    private:
        std::vector<std::unique_ptr<SvgEntity>> m_svgEntities { };
        float m_fOpacity;
    };

} // namespace Draw2d::Svg

#endif  /* GROUP_DOT_HPP */
