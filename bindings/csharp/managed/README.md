# Tests

The tests run by virtue of being in a collection; the collection ensures
that SvgParser::init() is called exactly once.  This happens by adding
the collection attribute to the class containing tests:

```c#
[Collection(SvgParserTestCollection.Name)]
```

Omitting this results in xUnit attempting to parallelise tests and each
one calls SvgParser::init(), which tries to initialise Xerces.
