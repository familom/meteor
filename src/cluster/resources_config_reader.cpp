#include "resources_config_reader.h"

#include "resources_tracker.h"

#include <boost/regex.hpp>

#include <stdexcept>
#include <string>


namespace {

std::string BuildNodeRegex(size_t numResources) {
    std::string s = "(\\w+)";
    for (size_t i = 0; i < numResources; ++i) {
        s += " (\\d+)";
    }

    return s;
}

} // ns

namespace Meteor {

StreamResourcesConfigReader::StreamResourcesConfigReader(std::istream& input,
                                                         IResourcesTracker& tracker)
    : Input(input)
    , Tracker(tracker)
{
}

void StreamResourcesConfigReader::Run() {
    std::string nodeResourcesStr;

    std::string regExpStr = BuildNodeRegex(Tracker.GetNumResources());
    boost::regex regExp(regExpStr);
    while (std::getline(Input, nodeResourcesStr)) {
        boost::smatch items;
        if (!boost::regex_match(nodeResourcesStr, items, regExp)) {
            throw std::runtime_error("Invalid node resources info: " + nodeResourcesStr);
        }

        Node nodeName(items[1]);
        Resource resourceId(0);
        while (resourceId < Tracker.GetNumResources()) {
            std::cout << items[2 + resourceId] << std::endl;
            ResourceQuantity quantity = std::stoull(items[2 + resourceId]);
            Tracker.Add(nodeName, resourceId, quantity);
            ++resourceId;
        }
    }
}

} // Meteor
