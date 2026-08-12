#!/bin/bash

mkdir -p src
mkdir -p src/core
mkdir -p src/xml
mkdir -p src/svg
cp ../c-libraries/draw2d/svg/* src/svg
cp ../c-libraries/core/String.[ch]pp src/core
cp ../c-libraries/core/Exception.[ch]pp src/core
cp ../c-libraries/core/{Slice,Splice}.hpp src/core
cp ../c-libraries/xml/* src/xml
mkdir -p tests
mkdir -p tests/svg
cp ../c-libraries/tests/draw2d/svg/* tests/svg
