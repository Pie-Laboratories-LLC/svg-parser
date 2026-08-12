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

#include "draw2d/Point.hpp"

namespace Draw2d {
    Point operator -(Point left, Point right)
    {
        Point result(left);

        result -= right;

        return result;
    }

    Point operator +(Point left, Point right)
    {
        Point result(left);

        result += right;

        return result;
    }
} // namespace Draw2d
