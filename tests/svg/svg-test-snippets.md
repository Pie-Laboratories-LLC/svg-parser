# SvgParser Test Snippets

Organized by feature area, roughly simple → adversarial. Each block is a
ready-to-paste C++ raw string literal (`R"xxx(...)xxx"`) plus a note on what
it's exercising and what to assert.

Suggested tag scheme (add `[throws]` on top of the feature tag for any case
expected to raise an `SvgException`):
- `[path]` — path/d parsing, move commands
- `[group]` — g nesting, opacity
- `[use]` — use/defs/id resolution
- `[colour]` — hex, named, currentColor, var()
- `[gradient]` — linear + radial
- `[transform]` — translate/scale/rotate/skew/matrix/lists
- `[structural]` — top-level dispatch, comments, whitespace, namespaces
- `[throws]` — cross-cutting, any test asserting an exception

---

## 1. Basic Path — move commands

### 1a. Single line segment
Tags: `[path]`
```cpp
auto svg1a = R"xxx(<svg><path id="p1a" d="M 0 0 L 10 10 Z"/></svg>)xxx";
```
Basic sanity: one MoveTo, one LineTo, ClosePath. Confirms tokenizer +
`m_pathMoveLookup` dispatch works at all.

### 1b. Quadratic and cubic curves in one path
Tags: `[path]`
```cpp
auto svg1b = R"xxx(<svg><path id="p1b" d="M 0 0 Q 5 5 10 0 C 15 5 20 -5 25 0 Z"/></svg>)xxx";
```
Exercises Q (4 values) and C (6 values) point-count logic in your two-pass
counting loop.

### 1c. Comma-separated coordinates (no spaces)
Tags: `[path]`
```cpp
auto svg1c = R"xxx(<svg><path id="p1c" d="M0,0L10,10,20,0Z"/></svg>)xxx";
```
Real SVG allows commas as separators and multiple coordinate pairs after a
single command letter (implicit repeat of the last command — `L 10,10 20,0`
means two LineTos, not one). **This is a spec feature you may not have
implemented** — your tokenizer currently expects one command letter per
point-group; multi-point implicit-repeat after L/C/Q is very common in
real-world SVG. Worth checking explicitly.

### 1d. Negative numbers glued to previous number (no delimiter)
Tags: `[path]`
```cpp
auto svg1d = R"xxx(<svg><path id="p1d" d="M0,0L10-10L20-20"/></svg>)xxx";
```
`10-10` is two numbers: `10` and `-10` — the `-` sign acts as an implicit
separator. Your current `pathPartsDelimiter` regex (`[ ,]+`) will **not**
split this correctly; it'll try to parse `10-10` as one token and fail.
Known gap, flag as documented-missing or fix.

### 1e. Leading-dot / no-leading-zero floats glued together
Tags: `[path]`
```cpp
auto svg1e = R"xxx(<svg><path id="p1e" d="M0,0L.5.5L1.2.3"/></svg>)xxx";
```
`.5.5` is two numbers: `.5` and `.5` (a second `.` can't belong to a
fraction already in progress). Another classic glued-number case, same
family of bug as 1d.

### 1f. Path not closed (should throw)
Tags: `[path][throws]`
```cpp
auto svg1f = R"xxx(<svg><path id="p1f" d="M 0 0 L 10 10"/></svg>)xxx";
```
No `Z` — should hit your `if (moveTo) throw SvgException("Path not closed!")`.

### 1g. Malformed — L before any M (should throw)
Tags: `[path][throws]`
```cpp
auto svg1g = R"xxx(<svg><path id="p1g" d="L 10 10 Z"/></svg>)xxx";
```
Should hit `throw SvgException("Invalid {} at {}", ...)` since `moveTo` is
false when a non-M/Z command appears.

### 1h. Double M in a row
Tags: `[path][throws]`
```cpp
auto svg1h = R"xxx(<svg><path id="p1h" d="M 0 0 M 10 10 Z"/></svg>)xxx";
```
Your code throws if `moveTo` is already true and another M appears. Note:
**this is stricter than the real spec** — SVG allows multiple subpaths via
repeated M within one `d` (each new M starts a new subpath without needing Z
first). Worth deciding: do you want multi-subpath support, or is this an
intentional simplification for font-glyph use cases? Document either way.

### 1i. Missing `d` attribute (should throw)
Tags: `[path][throws]`
```cpp
auto svg1i = R"xxx(<svg><path id="p1i"/></svg>)xxx";
```
Should hit `throw SvgException("Missing d attribute on pathNode")`.

---

## 2. Groups — nesting, transform, opacity

### 2a. Plain group with two paths
Tags: `[group]`
```cpp
auto svg2a = R"xxx(
<svg>
  <g id="g2a">
    <path id="p2a_1" d="M0,0 L10,10 Z"/>
    <path id="p2a_2" d="M5,5 L15,15 Z"/>
  </g>
</svg>)xxx";
```

### 2b. Nested groups
Tags: `[group]`
```cpp
auto svg2b = R"xxx(
<svg>
  <g id="g2b_1">
    <g id="g2b_2">
      <path id="p2b" d="M0,0 L1,1 Z"/>
    </g>
  </g>
</svg>)xxx";
```
Confirms recursive `__parseGroup` handles a `g` child inside a `g`.

### 2c. Group with opacity
Tags: `[group]`
```cpp
auto svg2c = R"xxx(<svg><g id="g2c" opacity="0.5"><path id="p2c" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 2d. Group with transform (single matrix)
Tags: `[group][transform]`
```cpp
auto svg2d = R"xxx(<svg><g id="g2d" transform="matrix(1,0,0,1,10,20)"><path id="p2d" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 2e. Group with unrecognized child element (should throw)
Tags: `[group][throws]`
```cpp
auto svg2e = R"xxx(<svg><g id="g2e"><circle cx="5" cy="5" r="3"/></g></svg>)xxx";
```
`circle` isn't in your dispatch (`use`/`path`/`g` only) — should throw
`"Unrecognized child node {} of groupNode"`. Good negative-space test; also
flags that `circle`/`rect`/`ellipse`/`polygon`/`line`/`polyline` primitives
aren't supported at all currently — worth a documentation note if font tools
ever emit them (nanoemoji/picosvg typically flatten everything to `path`
though, so may be moot for your actual use case).

---

## 3. `defs` and `use`

### 3a. Basic defs + use
Tags: `[use]`
```cpp
auto svg3a = R"xxx(
<svg>
  <defs>
    <path id="p3a" d="M0,0 L10,0 L10,10 Z"/>
  </defs>
  <use xlink:href="#p3a"/>
</svg>)xxx";
```

### 3b. use with fill/stroke override
Tags: `[use]`
```cpp
auto svg3b = R"xxx(
<svg>
  <defs><path id="p3b" d="M0,0 L10,0 Z"/></defs>
  <use xlink:href="#p3b" fill="#ff0000" stroke="#00ff00"/>
</svg>)xxx";
```

### 3c. use with x/y offset
Tags: `[use]`
```cpp
auto svg3c = R"xxx(
<svg>
  <defs><path id="p3c" d="M0,0 L10,0 Z"/></defs>
  <use xlink:href="#p3c" x="5" y="7"/>
</svg>)xxx";
```

### 3d. use referencing something not yet parsed, lazily resolved via getElementById
Tags: `[use]`
```cpp
auto svg3d = R"xxx(
<svg>
  <use xlink:href="#p3d"/>
  <path id="p3d" d="M0,0 L1,1 Z"/>
</svg>)xxx";
```
Exercises your `pSvg->getElementById` fallback path in `__parseUse` for
forward references (use appears *before* its target in document order —
legal in real SVG, and font-glyph SVGs with `<use>` for composite/skin-tone
glyphs may rely on this).

**Caveat**: `getElementById` requires either a DTD/schema declaring the `id`
attribute as type ID, or Xerces configured to treat `id` specially — plain
well-formed-only parsing (no DTD) often makes `getElementById` return
`nullptr` even when the attribute is literally named `id`. Worth testing
this explicitly since it's a likely silent failure point independent of
your own logic being correct.

### 3e. use with no matching id anywhere (should throw)
Tags: `[use][throws]`
```cpp
auto svg3e = R"xxx(<svg><use xlink:href="#nonexistent"/></svg>)xxx";
```

### 3f. use with missing xlink:href (should throw)
Tags: `[use][throws]`
```cpp
auto svg3f = R"xxx(<svg><use/></svg>)xxx";
```

### 3g. defs containing a group without an id (should throw)
Tags: `[use][structural][throws]`
```cpp
auto svg3g = R"xxx(
<svg><defs><g><path id="p1" d="M0,0 L1,1 Z"/></g></defs></svg>)xxx";
```

### 3h. defs containing gradient without id
Tags: `[gradient][structural][throws]`
```cpp
auto svg3h = R"xxx(
<svg>
  <defs>
    <linearGradient x1="0" y1="0" x2="1" y2="0">
      <stop offset="0" stop-color="#000000"/>
      <stop offset="1" stop-color="#ffffff"/>
    </linearGradient>
  </defs>
</svg>)xxx";
```

This is a degenerate test - this is a valid svg, but it's not accessible without
an id.

---

## 4. Colors

### 4a. Hex 6-digit
Tags: `[colour]`
```cpp
auto svg4a = R"xxx(<svg><path id="p4a" d="M0,0 L1,1 Z" fill="#a1b2c3"/></svg>)xxx";
```

### 4b. Hex 8-digit (with alpha)
Tags: `[colour]`
```cpp
auto svg4b = R"xxx(<svg><path id="p4b" d="M0,0 L1,1 Z" fill="#a1b2c3ff"/></svg>)xxx";
```

### 4c. Hex uppercase
Tags: `[colour]`
```cpp
auto svg4c = R"xxx(<svg><path id="p4c" d="M0,0 L1,1 Z" fill="#A1B2C3"/></svg>)xxx";
```

### 4d. Named standard color
Tags: `[colour]`
```cpp
auto svg4d = R"xxx(<svg><path id="p4d" d="M0,0 L1,1 Z" fill="red"/></svg>)xxx";
```
Assumes `red` exists in your `StandardColours::Colours` map — swap for
whatever's actually populated there.

### 4e. currentColor
Tags: `[colour]`
```cpp
auto svg4e = R"xxx(<svg><path id="p4e" d="M0,0 L1,1 Z" fill="currentColor"/></svg>)xxx";
```

### 4f. url(#id) referencing a gradient
Tags: `[colour][gradient]`
```cpp
auto svg4f = R"xxx(
<svg>
  <defs>
    <linearGradient id="lg4f" x1="0" y1="0" x2="1" y2="0">
      <stop offset="0" stop-color="#000000"/>
      <stop offset="1" stop-color="#ffffff"/>
    </linearGradient>
  </defs>
  <path id="p4f" d="M0,0 L1,1 Z" fill="url(#lg4f)"/>
</svg>)xxx";
```

### 4g. url(#id) referencing nonexistent gradient (should throw)
Tags: `[colour][gradient][throws]`
```cpp
auto svg4g = R"xxx(<svg><path id="p4g" d="M0,0 L1,1 Z" fill="url(#ghost)"/></svg>)xxx";
```

### 4h. var(--colorN, fallback) form
Tags: `[colour]`
```cpp
auto svg4h = R"xxx(<svg><path id="p4h" d="M0,0 L1,1 Z" fill="var(--color3, #ff0000)"/></svg>)xxx";
```
Exercises your `--color` / `Cpal` index path plus the fallback-color
mechanism (`setFallback`). Double check your `colourRegex` in
`__parseColour`:
```cpp
std::regex colourRegex { R"xxx(^var\s*\(\s*([, ]+)\s*,\s*([^,\s\)]+)\s*\))xxx" };
```
`([, ]+)` as the first capture group looks like it will **only ever match
commas and spaces**, never the actual `--colorN` token — this looks like a
bug (probably meant something like `([^,\s]+)`). This test case will likely
surface it immediately.

### 4i. stop-color as rgb()
Tags: `[colour][gradient]`
```cpp
auto svg4i = R"xxx(
<svg><defs><linearGradient id="lg4i" x1="0" y1="0" x2="1" y2="0">
  <stop offset="0" stop-color="rgb(255,0,0)"/>
  <stop offset="1" stop-color="rgb(0,255,0)"/>
</linearGradient></defs>
<path id="p4i" d="M0,0 L1,1 Z" fill="url(#lg4i)"/></svg>)xxx";
```

### 4j. stop-color as rgb() with percentages
Tags: `[colour][gradient]`
```cpp
auto svg4j = R"xxx(
<svg><defs><linearGradient id="lg4j" x1="0" y1="0" x2="1" y2="0">
  <stop offset="0" stop-color="rgb(100%,0%,0%)"/>
  <stop offset="1" stop-color="rgb(0%,100%,0%)"/>
</linearGradient></defs>
<path id="p4j" d="M0,0 L1,1 Z" fill="url(#lg4j)"/></svg>)xxx";
```

### 4k. stop-color as rgba()
Tags: `[colour][gradient]`
```cpp
auto svg4k = R"xxx(
<svg><defs><linearGradient id="lg4k" x1="0" y1="0" x2="1" y2="0">
  <stop offset="0" stop-color="rgba(255,0,0,0.5)"/>
  <stop offset="1" stop-color="rgba(0,255,0,1)"/>
</linearGradient></defs>
<path id="p4k" d="M0,0 L1,1 Z" fill="url(#lg4k)"/></svg>)xxx";
```

### 4l. Unrecognized color string (should throw)
Tags: `[colour][throws]`
```cpp
auto svg4l = R"xxx(<svg><path id="p4l" d="M0,0 L1,1 Z" fill="notacolor"/></svg>)xxx";
```

---

## 5. Linear Gradients

### 5a. Minimal valid
Tags: `[gradient]`
```cpp
auto svg5a = R"xxx(
<svg><defs><linearGradient id="lg5a" x1="0" y1="0" x2="1" y2="0">
  <stop offset="0" stop-color="#000000"/>
  <stop offset="1" stop-color="#ffffff"/>
</linearGradient></defs></svg>)xxx";
```

### 5b. With gradientUnits + gradientTransform
Tags: `[gradient]`
```cpp
auto svg5b = R"xxx(
<svg><defs><linearGradient id="lg5b" x1="0" y1="0" x2="1" y2="0"
  gradientUnits="userSpaceOnUse" gradientTransform="matrix(1,0,0,1,5,5)">
  <stop offset="0" stop-color="#000000"/>
  <stop offset="1" stop-color="#ffffff"/>
</linearGradient></defs></svg>)xxx";
```

### 5c. Missing x1 (should throw)
Tags: `[gradient][throws]`
```cpp
auto svg5c = R"xxx(
<svg><defs><linearGradient id="lg5c" y1="0" x2="1" y2="0">
  <stop offset="0" stop-color="#000000"/>
</linearGradient></defs></svg>)xxx";
```

### 5d. Unrecognized attribute (should throw)
Tags: `[gradient][throws]`
```cpp
auto svg5d = R"xxx(
<svg><defs><linearGradient id="lg5d" x1="0" y1="0" x2="1" y2="0" spreadMethod="pad" xr="16.34">
  <stop offset="0" stop-color="#000000"/>
  <stop offset="1" stop-color="#ffffff"/>
</linearGradient></defs></svg>)xxx";
```
`spreadMethod` is a legitimate, commonly-used SVG gradient attribute
(pad/reflect/repeat) that your parser doesn't recognize at all — will throw
`"Unrecognized linear gradient attribute"`. Real gap: worth deciding whether
to support it or explicitly document it as unsupported (font tools may or
may not emit it).

### 5e. Stop offset out of range (should throw)
Tags: `[gradient][throws]`
```cpp
auto svg5e = R"xxx(
<svg><defs><linearGradient id="lg5e" x1="0" y1="0" x2="1" y2="0">
  <stop offset="1.5" stop-color="#000000"/>
</linearGradient></defs></svg>)xxx";
```

---

## 6. Radial Gradients

### 6a. Minimal valid (cx, cy, r only)
Tags: `[gradient]`
```cpp
auto svg6a = R"xxx(
<svg><defs><radialGradient id="rg6a" cx="0.5" cy="0.5" r="0.5">
  <stop offset="0" stop-color="#000000"/>
  <stop offset="1" stop-color="#ffffff"/>
</radialGradient></defs></svg>)xxx";
```

### 6b. With focal point overrides (fx, fy, fr)
Tags: `[gradient]`
```cpp
auto svg6b = R"xxx(
<svg><defs><radialGradient id="rg6b" cx="0.5" cy="0.5" r="0.5" fx="0.4" fy="0.4" fr="0.1">
  <stop offset="0" stop-color="#000000"/>
  <stop offset="1" stop-color="#ffffff"/>
</radialGradient></defs></svg>)xxx";
```

### 6c. Missing r (should throw)
Tags: `[gradient][throws]`
```cpp
auto svg6c = R"xxx(
<svg><defs><radialGradient id="rg6c" cx="0.5" cy="0.5">
  <stop offset="0" stop-color="#000000"/>
</radialGradient></defs></svg>)xxx";
```

---

## 7. Transforms (the fresh work)

### 7a. Single translate, one arg (ty defaults to 0)
Tags: `[transform]`
```cpp
auto svg7a = R"xxx(<svg><g id="g7a" transform="translate(10)"><path id="p7a" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 7b. translate, both args
Tags: `[transform]`
```cpp
auto svg7b = R"xxx(<svg><g id="g7b" transform="translate(10,20)"><path id="p7b" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 7c. scale, one arg (sy = sx)
Tags: `[transform]`
```cpp
auto svg7c = R"xxx(<svg><g id="g7c" transform="scale(2)"><path id="p7c" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 7d. scale, both args
Tags: `[transform]`
```cpp
auto svg7d = R"xxx(<svg><g id="g7d" transform="scale(2,3)"><path id="p7d" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 7e. rotate, angle only (around origin)
Tags: `[transform]`
```cpp
auto svg7e = R"xxx(<svg><g id="g7e" transform="rotate(45)"><path id="p7e" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 7f. rotate, angle + cx + cy
Tags: `[transform]`
```cpp
auto svg7f = R"xxx(<svg><g id="g7f" transform="rotate(45,10,10)"><path id="p7f" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 7g. rotate with only 2 args (malformed — should throw per spec)
Tags: `[transform][throws]`
```cpp
auto svg7g = R"xxx(<svg><g id="g7g" transform="rotate(45,10)"><path id="p7g" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 7h. skewX
Tags: `[transform]`
```cpp
auto svg7h = R"xxx(<svg><g id="g7h" transform="skewX(30)"><path id="p7h" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 7i. skewY
Tags: `[transform]`
```cpp
auto svg7i = R"xxx(<svg><g id="g7i" transform="skewY(30)"><path id="p7i" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 7j. matrix, explicit 6-value
Tags: `[transform]`
```cpp
auto svg7j = R"xxx(<svg><g id="g7j" transform="matrix(1,0,0,1,10,20)"><path id="p7j" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 7k. Transform LIST
Tags: `[transform]`
```cpp
auto svg7k = R"xxx(<svg><g id="g7k" transform="translate(0 -6.75) translate(0,-1638.4) scale(56.8888888889)"><path id="p7k" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```
This is (approximately) the actual failing case from your font glyph SVG —
good regression test to pin exactly this shape down permanently.

### 7l. Transform list mixing all function types
Tags: `[transform]`
```cpp
auto svg7l = R"xxx(<svg><g id="g7l" transform="translate(5,5) rotate(30) scale(2) skewX(10) matrix(1,0,0,1,0,0)"><path id="p7l" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

### 7m. Transform list with mixed comma/space separators between args
Tags: `[transform]`
```cpp
auto svg7m = R"xxx(<svg><g id="g7m" transform="translate(1, 2)  scale( 3 , 4 )"><path id="p7m" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```
Deliberately sloppy whitespace — tests tokenizer robustness against
irregular spacing, which real-world tool-generated SVG absolutely produces.

### 7n. Scientific notation in transform values
Tags: `[transform]`
```cpp
auto svg7n = R"xxx(<svg><g id="g7n" transform="scale(1.5e-2)"><path id="p7n" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```
Confirms `Core::ParseFloat`/`from_chars` handles `e`/`E` notation correctly
inside a real transform context, not just as an isolated unit test of the
float parser.

### 7o. Unknown transform function (should throw)
Tags: `[transform][throws]`
```cpp
auto svg7o = R"xxx(<svg><g id="g7o" transform="shear(10)"><path id="p7o" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```
`shear` isn't a real SVG function (common typo/confusion with skewX/Y) —
good negative test.

### 7p. Malformed transform — unclosed paren (should throw)
Tags: `[transform][throws]`
```cpp
auto svg7p = R"xxx(<svg><g id="g7p" transform="translate(10,20"><path id="p7p" d="M0,0 L1,1 Z"/></g></svg>)xxx";
```

---

## 8. Structural / whole-document edge cases

### 8a. Unrecognized top-level element (should throw)
Tags: `[structural][throws]`
```cpp
auto svg8a = R"xxx(<svg><circle cx="5" cy="5" r="3"/></svg>)xxx";
```
Exercises your `Parse()` top-level dispatch's `else throw
SvgException("Unrecognized element {}")` branch.

### 8b. Empty svg root (no children) — should this throw or no-op?
Tags: `[structural]`
```cpp
auto svg8b = R"xxx(<svg></svg>)xxx";
```
Worth deciding expected behavior explicitly — currently your loop just
won't execute, `getUnclaimedGlobals()` will be empty, no exception. Good
"parses successfully but does nothing" baseline test.

### 8c. Duplicate id across different element types (should throw)
Tags: `[structural][throws]`
```cpp
auto svg8c = R"xxx(
<svg>
  <defs>
    <path id="dup" d="M0,0 L1,1 Z"/>
  </defs>
  <g id="dup">
    <path id="p2" d="M0,0 L1,1 Z"/>
  </g>
</svg>)xxx";
```
Exercises `__saveSvgElement`'s duplicate-id check — should throw the
"previously defined as X and now as Y" exception.

### 8d. Whitespace-only text nodes between elements (should be ignored, not crash)
Tags: `[structural]`
```cpp
auto svg8d = R"xxx(<svg>

  <g id="g8d">

    <path id="p8d" d="M0,0 L1,1 Z"/>

  </g>

</svg>)xxx";
```
Confirms your `if (pChildNode->getNodeType() != DOMNode::ELEMENT_NODE)
continue;` guards correctly skip text/whitespace nodes throughout — cheap
but important robustness check, since real-world (pretty-printed) SVG is
never on one line.

### 8e. XML comments interspersed (should be ignored)
Tags: `[structural]`
```cpp
auto svg8e = R"xxx(
<svg>
  <!-- this is a comment -->
  <g id="g8e">
    <!-- another comment -->
    <path id="p8e" d="M0,0 L1,1 Z"/>
  </g>
</svg>)xxx";
```

### 8f. With XML declaration (real FreeType OT-SVG output may or may not include this)
Tags: `[structural]`
```cpp
auto svg8f = R"xxx(<?xml version="1.0" encoding="UTF-8"?>
<svg><path id="p8f" d="M0,0 L1,1 Z"/></svg>)xxx";
```

### 8g. With xmlns declarations on root (very common in real tool output)
Tags: `[structural]`
```cpp
auto svg8g = R"xxx(
<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
  <path id="p8g" d="M0,0 L1,1 Z"/>
</svg>)xxx";
```
Worth confirming your DOM parsing setup (`setDoNamespaces(true)` in
`DomParser`) doesn't do anything surprising to tag-name comparisons once a
default namespace is declared — some DOM configurations return qualified
vs. local names differently once namespaces are "on." This is a real risk
area given `setDoNamespaces(true)` is active in your parser and your code
compares against plain strings like `GROUP_NAME = "g"`.

## 9. Circular references

### 9a. Direct self-reference (use pointing at itself)
Tags: `[self-reference][use]`

```cpp
auto svg9a = R"xxx(<svg><use id="u9a" xlink:href="#u9a"/></svg>)xxx";
```

### 9b. Two-node cycle (A references B, B references A):
Tags: `[self-reference][use]`

```cpp
auto svg9b = R"xxx(
<svg>
  <use id="u9b_1" xlink:href="#u9b_2"/>
  <use id="u9b_2" xlink:href="#u9b_1"/>
</svg>)xxx";
```

### 9c. Three-node cycle, just to confirm it's not accidentally only caught for the 2-node case
Tags: `[self-reference][use]`

```cpp
auto svg9c = R"xxx(
<svg>
  <use id="u9c_1" xlink:href="#u9c_2"/>
  <use id="u9c_2" xlink:href="#u9c_3"/>
  <use id="u9c_3" xlink:href="#u9c_1"/>
</svg>)xxx";
```
### 9d.  Use: reference to enclosing group is broken
Tags: `[self-reference][use]`

```cpp
const char *svg9d =
    R"xxx(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
            <g id="loop" fill="red">
              <path d="M0,0 L10,0 L10,10 Z"/>
              <use href="#loop"/>
            </g>
          </svg>)xxx";
```

### 9e.  use: repeated reference is not a cycle
Tags: `[self-reference][use]`

```cpp
auto svg9e =
        R"xxx(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
                <defs>
                  <path id="leaf" d="M0,0 L10,0 L10,10 Z"/>
                </defs>
                <g id="mid">
                  <use href="#leaf" x="0"  y="0"/>
                  <use href="#leaf" x="20" y="0"/>
                </g>
              </svg>)xxx";
```

### 9f.  use: cycle through nested svg is broken
Tags: `[self-reference][use]`

```cpp
auto svg9f =
    R"xxx(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
            <g id="outer">
              <svg x="10" y="10" width="50" height="50">
                <use href="#outer"/>
              </svg>
            </g>
          </svg>)xxx";
```

### 9g. Two-node cycle (A references B, B references A), one in defs
Tags: `[self-reference][use]`

```cpp
auto svg9g = R"xxx(
<svg>
  <defs>
    <use id="u9g_1" xlink:href="#u9g_2"/>
  </defs>
  <use id="u9g_2" xlink:href="#u9g_1"/>
</svg>)xxx";
```

### 9h use -> defs group -> use -> back to the enclosing group
Tags: `[self-reference][use][throws]`

```cpp
auto svg9h = R"xxx(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
  <defs>
    <g id="d_inner">
      <path d="M0,0 L10,0 L10,10 Z"/>
      <use xlink:href="#d_outer"/>
    </g>
  </defs>
  <g id="d_outer">
    <use xlink:href="#d_inner"/>
  </g>
</svg>)xxx";
```

### 9i cycle buried under several defs levels
Tags: `[self-reference][use][throws]`

```cpp
auto svg9i = R"xxx(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
  <defs>
    <g id="lvl1">
      <g id="lvl2">
        <use xlink:href="#lvl1"/>
      </g>
    </g>
  </defs>
  <use xlink:href="#lvl2"/>
</svg>)xxx";
```

---

## Suggested triage

Given the volume, I'd bucket these into three piles as you work through them:

1. **Should already pass** — 1a/b, 2a-d, 3a-c/e-h, 4a-c/f-g/l, 5a-c/e, 6a-c,
   7a-f/h-j/o-p, 8a-e — these exercise features you clearly already built
   correctly.
2. **Likely to surface real bugs right now** — 4h (the `var()` regex looks
   broken), 1d/1e (glued numbers in path data), 8g (namespace handling).
3. **Spec features you may not support yet, worth a conscious decision
   per-case (implement vs. document as unsupported)** — 1c (implicit
   command repeat), 1h (multi-subpath via repeated M), 5d (spreadMethod),
   any primitive shapes (circle/rect/etc.).
