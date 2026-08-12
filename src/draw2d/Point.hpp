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

#ifndef POINT_DOT_HPP
#define POINT_DOT_HPP

#include <vector>

namespace Draw2d {

    class Point
    {
    public:
        double getX() const { return m_x; }
        void setX(double x) { m_x = x; }
        double getY() const { return m_y; }
        void setY(double y) { m_y = y; }
        
        Point() { }

        Point(double x, double y)
        {
            m_x = x;
            m_y = y;
        }

        Point(const Point &copy)
        {
            m_x = copy.m_x;
            m_y = copy.m_y;
        }

        void Set(double dX, double dY)
        {
            m_x = dX;
            m_y = dY;
        }

        void Set(const Point &copy)
        {
            m_x = copy.m_x;
            m_y = copy.m_y;
        }

        Point &operator -= (Point right)
        {
            m_x -= right.m_x;
            m_y -= right.m_y;
            return *this;
        }

        Point &operator += (Point right)
        {
            m_x += right.m_x;
            m_y += right.m_y;
            return *this;
        }

        template <typename TargetType, typename... Args>
        requires std::is_arithmetic_v<TargetType>
        Point &operator *= (TargetType scalar)
        {
            m_x *= scalar;
            m_y *= scalar;
            return *this;
        }

        friend Point operator -(Point left, Point right);
        friend Point operator +(Point left, Point right);

    private:
        double m_x = 0.0;
        double m_y = 0.0;
    }; // class Point

    typedef std::vector<Point> PointVector;

} // namespace Draw2d

#endif  /* POINT_DOT_HPP */
