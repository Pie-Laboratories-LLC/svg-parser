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
* limited support for swap() for color and color-related properties like fill and stroke
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

The SvgParser creates an abstract syntax tree.  It doesn't strictly look like a
tree, but the returned document has a root element, and the root element
contains children, and so forth.  Hence the tree structure.  Some information is
gleaned at parse-time, such as inherit attributes.  However, much information is
deferred to the rendering layer.

In general I pride myself on my encapsulation.  That's evidenced here by the
general lack of rendering-layer code.  The confluence where that comes together
are the classes in src/draw2d.  draw2d is obviously the name of my rendering
layer.  Gradients exist as first class entities in the renderer layer.  The
reason for all of those files is that, for instance, SpreadMethod is a property
that applies to gradients, and gradients live in the renderer layer.  So
SvgParser has dependencies on that layer.

So in my implementation I use SvgParser to parse the OT-SVG glyphs freetype
returns.  I then convert the SvgEntity-derived hierarchy into equivalents in the
rendering layer, applying semantic meaning to the abstract syntax tree; in
addition to "instancing" gradients there's tracking x, y position and the
transformation matrix.  These are things only knowable at render time.

## ALTERNATIVES TO XERCES

As noted above, the project uses Xerces for XML parsing.  A precocious user can
replace this via the abstraction layer.  There are four classes to implement:

* src/svg/DomParserBuilder.hpp.  Simple builder pattern: if using xerces,
  XercesDomParserBuilder.cpp is added to the list of files to compile.
* src/svg/IDomParser.hpp. Wraps the parsing lobic.  The xerces implementation is
  in src/svg/XercesDomParser.hpp
* src/svg/IDomDocument.hpp.  Wraps the primary dom document entity, i.e,. the
  top level abstraction representing the parsed DOM.  The xerces implementation
  is in src/svg/XercesDomDocument.hpp.
* src/svg/IDomEntity.hpp.  Wraps DOM entities within the document.  The xerces
  implementation is in src/svg/XercesDomEntity.cpp

The xerces version of these files are conditionally added to CMakeLists.txt
depending on the value of the `SVGPARSER_WITH_XERCES` cmake flag (see
[#BUILDING](#BUILDING) below.

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

The best source for examples is SvgParser-tests.cpp.  Here is the content of example.cpp with the compilation
command below:

```c++
#include <iostream>
#include <xercesc/util/PlatformUtils.hpp>
#include "svg/SvgParser.hpp"
#include "svg/Group.hpp"
#include "core/Logger.hpp"
#include "core/String.hpp"

using namespace Draw2d::Svg;
XERCES_CPP_NAMESPACE_USE

int main() {
    try {
        XMLPlatformUtils::Initialize();
    } catch (const XMLException& e) {
        char* msg = XMLString::transcode(e.getMessage());
        std::cerr << "Xerces init error: " << msg << std::endl;
        XMLString::release(&msg);
        return 1;
    }
    Core::Logger::Instance(std::unordered_set<const char *,CStrHash,CStrEq> { }, Core::Info);

    auto svgText = R"xxx(
    <svg>
      <g id="g2a">
        <path id="p2a_1" d="M0,0 L10,10 Z"/>
        <path id="p2a_2" d="M5,5 L15,15 Z"/>
      </g>
    </svg>)xxx";
    SvgParser svgParser {};
    auto svgDocument = svgParser.parse(svgText);
    auto group = svgDocument.get()->lookupSvgEntity("g2a");
    std::cout << "Found a group with id " << group->getId() << std::endl;
    // alternatively
    auto svg = svgDocument.get()->getRootSvg();
    for(const auto &child : svg->enumerateChildren()) {
        std::cout << "have a child with id " << child->getId() << " of type " << child->getType() << std::endl;
        if(const Draw2d::Svg::Group *cpGroup = dynamic_cast<const Draw2d::Svg::Group *>(child); cpGroup) {
            for(const auto &groupChild : cpGroup->enumerateChildren()) {
                std::cout << "Group has a child with id " << groupChild->getId() << " of type " << groupChild->getType() << std::endl;
            }
        }
    }
    return 0;
}
```

```bash
g++ -o example.exe -std=c++23 -fcoroutines -I src example.cpp -lxerces-c -lstdc++exp  build/src/libsvgparser.a
./example.exe
```

# ROADMAP

These are short-term goals:

* make the xml abstraction layer less xerces-shaped.  The abstraction layer was
  built from xerces rather than the other way around.  Implementing for SAX2
  would be a real challenge (see planned SAX2 support below).
* c# bindings.  The original genesis for this project was that at the time
  there was a paucity of freely available svg parsers for c#.  It must be
  possible to use c#-native XML parsing with the svg parser in c#.
* support for `<text...>` elements.  I'm rendering fonts in 3d and this library
  allows me to render OT-SVG fonts, in particular, emojis.  So adding support
  for the `<text...>` element is essentially free.
* support for `<pattern...>` elements.  This is another thing for which there is
  extensive support in the rendering layer.

These are mid-term goals:

* dependency injection.  it stinks having to use a separate logger.  The larger
  application will ultimately use dynamic configuration (e.g., via HashiCorp
  consul) and that will be the point I use dependency injection.  So far I
  haven't had the need for it.
* support for SMIL animation.  This has a large surface area but fits neatly
  into what I already have in the rendering engine
* support for css styling.  Currently I grab any style text I find and append it
  into a string in the SvgDocument, and that's as much as I do.  I want to be
  sure to support css styling as commonly occurs in SVG documents.

Longer term goals:

* clipping and masking via `<clipPath..>` and `<mask...>`
* markers via `<marker...>`.  This isn't so challenging to implement, there's
  just no immediate need for this.
* a SAX2 implementation parser to complement the DOM.
* install target.  I just don't have use for it.

# HISTORY

This is part of a lifelong passion project.  I started my career as a c++
desktop application developer, first using Borland C++ then switching to Visual
c++ and Microsoft Foundation Classes.  I got my first job in industry the way a
lot of us do - I was tasked to do a potentially career-ending chore using a 3d
visualisation tool called AVS/Express.  With that position I attended SIGGRAPH
and interest in 3d programming has fueled my personal interests ever since.

One problem that's really dogged me my whole life has been the pace of change of
3d graphics programming.  Original open gl implementations were software
renderers.  I had a 3dfx Voodoo graphics card which had a lot of proprietary 3d
graphics rendering features.  Once I had mastered this, the next version of GL
incorporated shaders, which radically revised how 3d programmin worked, so I sat
the project aside.

I came back to the project and learned shaders, and relearned open gl.  By that
point I had become a c# developer against better judgement.  So I worked a lot
in c#.  Microoft's introduction of Direct3d was a hurdle because of the
Microsoft way of pouring all its resources into its proprietary apis while
supporting open source apis in ways that feel petty: strict adherance to
standards that other vendors are lax on and so forth.

One of the problems with 3d programming is rendering fonts.  The way it's mostly
done is via bitmap fonts that are created at development time and then build
into the application.  So you use freetype to render to buffers and then blit
those buffers into 3d.  Solvable, well known, easy to do, or easier just to grab
some framework.  That was never satisfying, though.

The years intervened and I worked on other projects in my spare time,
particularly focused around learning c# and ultimately gitops practices.  When I
came back, open gl had been shelved for Vulkan.  So I laboured through the
Vulkan official tutorial; I'm not sure I've ever completed a tutorial to the end
except for vulkan.  Vulkan isn't just something you casually pick up.

Personal drama intervened and it was about a year and a half before I could get
back to the project.  By that time I was more determined than ever to have a
bespoke font rendering engine, so I built it.  Initially I built it in c#, from
scratch, an SvgParser and renderer.  Hence where I've wound up now.

Most of what's being released here was originally written in c# and then ported
to c++ to utilise Vulkan.  30 years is a long time away from a language and c++
has seen many seismic shifts since then.  So I'm currently working my way
through Scott Meyers' excellent _Effective Modern C++_ and Bjarne
Stroustrup's _A Tour of C++_.  It's a slow-going process.  Openly, some of the
things I'm working hard on at the time of this writing:

* rule of five vs rule of zero and universal references.  I did a short stint
  with Rust, and coming from c# and python I didn't care for it.  c# was like
  smelly old running shoes that had given over to my lawnmowing/miserable job
  shoes.  I will never really _like_ python since I spent so much time as a perl
  developer, but I basically _like_ it as a scripting language goes.  Rust's
  concept of ownership was daunting.  c++' concept of ownership is still a
  moving target for me

* stl usage wizardry.  I'm finally finding my feet around compiler generated
  error messages and parsing through 30 lines that are 256+ characters
  enumerating compile-time assertion failures.  I'm getting comfortable
  implementing hash and equality operators for maps and sets

* templating in general.  concepts and assertions are really new and strange to
  me.  I've had claude generate Slice and Splice - you can tell my provenance as
  a developer from those alone.  I want to master that; mingw's c++ compiler at
  the time of this writing didn't include a Generator for coroutines, so claude
  generated one for me.  I want to master these ideas but I'm yet at the
  grasping stage.

The project has grand scope and a million different things to work on and no
particular concret goals.  Progress will be tracked on YouTube, and I may
release other portions as open source projects.
