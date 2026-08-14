public class Group: SvgEntity, ISvgContainerEntity {
    private IntPtr _group;
    public Group(IntPtr group_): base(group_) {
        // trippy!
        _group = group_;
    }

    public List<SvgEntity> enumerateChildren() {
        return SvgHelpers.GetChildren(_group);
    }
}
