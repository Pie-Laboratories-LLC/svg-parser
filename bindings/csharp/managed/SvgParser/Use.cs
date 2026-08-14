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
