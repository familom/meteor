#include "resources_tracker.h"

#include <cassert>
#include <stdexcept>


namespace Meteor {

ResourcesTracker::ResourcesTracker(size_t numResources)
    : NumResources(numResources)
{
}

/// @throw runtime_error
void ResourcesTracker::ValidateResource(const Resource& resource) {
    if (resource >= NumResources) {
        throw std::runtime_error("Invalid resource id.");
    }
}

void ResourcesTracker::Add(const Node& node, const Resource& resource,
                           const ResourceQuantity& quantity) {
    ValidateResource(resource);

    auto it = State.find(node);
    if (it != State.end()) {
        nodeState.All[resource] += quantity;
        nodeState.Current[resource] += quantity;
    } else {
        NodeState nodeState(NumResources);
        State.insert(Resources::value_type(node, nodeState));
    }
}

bool ResourcesTracker::Acquire(const Node& node, const Resource& resource,
                               const ResourceQuantity& quantity) {
    ValidateResource(resource);

    auto it = State.find(node);
    if (it == State.end()) {
        return false;
    }

    NodeState& nodeState = it->second;
    assert(nodeState.Current.size() == NumResources);

    if (nodeState.Current[resource] >= quantity) {
        nodeState.Current[resource] -= quantity;
        return true;
    }

    return false;
}

} // Meteor
