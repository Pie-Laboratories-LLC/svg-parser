/*
 * Copyright 2026 Pie Laboratories
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>

#ifdef SVGPARSER_WITH_XERCES
    #include <xercesc/util/PlatformUtils.hpp>
#endif

int main(int argc, char* argv[]) {
#ifdef SVGPARSER_WITH_XERCES
    // Required before any Xerces-C parsing call; SvgParser has no lazy
    // self-init, so callers of the library carry this same obligation.
    xercesc::XMLPlatformUtils::Initialize();
#endif

    int result = Catch::Session().run(argc, argv);

#ifdef SVGPARSER_WITH_XERCES
    xercesc::XMLPlatformUtils::Terminate();
#endif

    return result;
}
