# 2.0.0

* Add `<polygon>` and `<polyline>`, parsed into the equivalent path data
* Add `<mask>` and `<clipPath>`, and the `mask` and `clip-path` properties
* Add CSS support: rules from `<style>` elements and `style` attributes are
  cascaded by `!important`, specificity and source order, then inherited.
  Selectors are limited to single compound selectors (type, `#id`, `.class`,
  `*`); others are skipped.
* Add `opacity`, `mix-blend-mode` and `isolation`, with `BlendColours` for the
  Compositing and Blending Level 1 blend modes
* Gradients now inherit stops, geometry and transform through `href`, including
  from gradients defined later in the document; reference cycles are ignored
* `<text>` keeps its content and nested `<tspan>`s in document order, with
  whitespace collapsing; adds `text-anchor`, `font-style`, `font-family` and
  `font-size`
* Fix: copying an `SvgPaint` now keeps its fallback colour
* **Breaking:** hex colours are now stored blue, green, red, alpha, as
  `getBgrA()`/`SvgColour.Bgra` always claimed; they were previously stored in
  written order, so `#112233` gave `11 22 33 FF` and now gives `33 22 11 FF`
* **Breaking:** an element with no `fill` now gets black, the SVG initial
  value, rather than `None`
* **Breaking (C++):** `Core::LogLevel` is now an `enum class`, so `Core::Info`
  becomes `Core::LogLevel::Info`.  Logging categories are now
  `Core::Catégorie` objects; pass `.c_str()` where a `const char *` is
  expected, e.g. `Draw2d::LoggingCategories::SVG.c_str()`
* C# bindings: `Polygon`, `Polyline`, `Mask`, `ClipPath` and `TSpan`;
  `SvgEntity.Opacity`, `MaskId`, `ClipPathId`, `BlendMode` and `Isolate`;
  `Text.Parts`, `Content`, `FontSize`, `FontSizeUnits`, `FontFamily`,
  `TextAnchor`, `FontStyle` and `TextLengthPercentage`
* README: dependency install instructions for macOS, Linux and Windows

# 1.1.0

* Add accessors to Svg for ViewBox, PreserveAspectRatio, and PreserveAspectRatioMode
* Add Image and Text svg entities
* Added GitHub Actions to build linux, windows, and mac variants of SvgParser
