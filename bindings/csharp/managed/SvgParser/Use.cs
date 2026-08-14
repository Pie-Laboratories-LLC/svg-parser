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

namespace PieLaboratories.Svg;

public class Use: SvgEntity, ISvgContainerEntity, ISvgDimensionedEntity {
    private IntPtr _use;
    public Use(IntPtr use): base(use) {
        _use = use;
    }

    public Dimension? X => SvgHelpers.GetDimensionedX(_use);
    public Dimension? Y => SvgHelpers.GetDimensionedY(_use);
    public Dimension? Width => SvgHelpers.GetDimensionedWidth(_use);
    public Dimension? Height => SvgHelpers.GetDimensionedHeight(_use);

    public List<SvgEntity> enumerateChildren() {
        return SvgHelpers.GetChildren(_use);
    }
}
