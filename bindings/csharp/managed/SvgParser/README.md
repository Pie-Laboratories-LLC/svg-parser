# INTRODUCTION

This is a simple svg parser which parses the subset of svg primarily used by
OT-SVG fonts, particularly aemoji fonts.  The project can be found
[On Github](https://github.com/Pie-Laboratories-LLC/svg-parser/tree/csharp-bindings).
It's part of a larger project whose progress can be tracked
[On Youtube](https://www.youtube.com/@PieLaboratoriesLLC).

The particular svg elements covered consist of:

* `<path>`, `<rect>`, `<group>`, `<use>`, `<ellipse>`, `<circle>`, `<defs>`
* `<linearGradient>`, `<radialGradient>`
* fill and stroke operations and "currentColor"
* limited support for the CSS `var()` primitive in colour attributes, e.g.
  `fill="var(--currentColor,#deadbeef)"`
* optional built-in support for Xerces XML parsing
* support for XML namespaces (optional)
* support for DTD validation (optional)

# PARSING

The following is an example that excercises all the functionality of the parser:

```c#
```
