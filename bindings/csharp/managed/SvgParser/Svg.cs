namespace PieLaboratories.Svg;

public class Svg: SvgEntity, ISvgContainerEntity {
    private IntPtr _svg;

    public Svg(IntPtr svg): base(svg) {
        _svg = svg;
    }

    public List<SvgEntity> enumerateChildren() {
        return SvgHelpers.GetChildren(_svg);
    }

    public Dimension? X => SvgHelpers.GetDimensionedX(_svg);
    public Dimension? Y => SvgHelpers.GetDimensionedY(_svg);
    public Dimension? Width => SvgHelpers.GetDimensionedWidth(_svg);
    public Dimension? Height => SvgHelpers.GetDimensionedHeight(_svg);
}
