# INTRODUCTION

This is an SVG Parser written in c++ with c# bindings.  It's part of a much
larger project that includes 2d and 3d rendering engines written from scratch.
It's a lifelong passion project.

The SVG Parser creates an abstract syntax tree from an SVG document.  Parse-time
resolution is performed on fill="currentColor" style attributes as well as
`<use...>` elements.  Semantic information about the SVG is handled at render
time in the rendering layer which is not included in this project.

The SVG Parser supports parsing SVG documents used in fonts.  It is not
a complete implementation of the SVG specification.  In broad strokes, it
supports:

* <path>, <rect>, <group>, <use>, <ellipse>, <circle>, <defs>
* <linearGradient>, <radialGradient>
* fill and stroke operations and "currentColor"
* optional built-in support for Xerces xml parsing
* support for xml namespaces optional
* support for dtd validation optional

I have extensive plans for the project and I intend to update this repository as
I make headway.  Of course, that depends a lot on community interest, personal
circumstances, and the needs of the larger project.  See [#ROADMAP](ROADMAP)
section below for more information.

I have been working on this project over 20 years.  At the time, Xerces was
(arguably) the cadillack of XML Parsers, and I had a lot invested in Xerces as I
had used it extensively.  Now there are better options, but I still have a soft
spot for it and am inherently lazy.  The compromise was to make Xerces support
optional.  See [#IMPLEMENTATION_NOTES](IMPLEMENTATION NOTES) and [#BUILDING]
(BUILDING) below for more information.

# IMPLEMENTATION NOTES

As noted above, the project uses Xerces for XML parsing.  A precocious user can
replace this via the abstraction layer.  

# BUILDING

Use the following commands to build the application.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)
```

To build with xerces support off, pass `-DSVGPARSER_WITH_XERCES=OFF`, to the
configure command, like so:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DSVGPARSER_WITH_XERCES=OFF
```

Tests must be run separately.  Either use:

```bash
ctest --test-dir build --output-on-failure
```

or run the Catch2 executable directly:

```bash
./build/tests/svgparser_tests
```

# EXAMPLES - c++

```c++
using namespace Draw2d::Svg;

std::string xmlDocument = "...";
SvgParser svgParser {};
auto svgDocument = svgParser.parse(xmlDocument);
```

# ROADMAP

These are short-term goals:

* support for `<text...>` elements.  I'm rendering fonts in 3d and this library
  allows me to render OT-SVG fonts, in particular, emojis.  So adding support
  for the `<text...>` element is essentially free.
* support for `<pattern...>` elements.  This is another thing for which there is
  extensive support in the rendering layer.

These are mid-term goals:

* support for SMIL animation.  This has a large surface area but fits neatly
  into what I already have in the rendering engine
* support for css styling.  Currently I grab any style text I find and append it
  into a string in the SvgDocument, and that's as much as I do.  I want to be
  sure to support css styling as commonly occurs in SVG documents.

Longer term goals:

* clipping and masking via `<clipPath..>` and `<mask...>`
* markers via `<marker...>`.  This isn't so challenging to implement, there's
  just no immediate need for this.
