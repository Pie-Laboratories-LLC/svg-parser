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

