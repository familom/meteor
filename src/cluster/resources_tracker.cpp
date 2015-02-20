#include "resources_tracker.h"

#include <cassert>
#include <stdexcept>
#include <string>


namespace Meteor {

ResourcesTracker::ResourcesTracker(size_t numResources)
    : NumResources(numResources)
{
}

/// @throw runtime_error
void ResourcesTracker::ValidateResource(const Resource& resource) {
    if (resource >= NumResources) {
        throw std::runtime_error("Invalid resource id " + std::to_string(resource));
    }
}

void ResourcesTracker::Add(const Node& node, const Resource& resource,
                           const ResourceQuantity& quantity) {
    ValidateResource(resource);

    auto it = State.find(node);
    if (it != State.end()) {
        NodeState& nodeState = it->second;
        nodeState.All[resource] += quantity;
        nodeState.Current[resource] += quantity;
    } else {
        NodeState nodeState(NumResources);
        nodeState.All[resource] = quantity;
        nodeState.Current[resource] = quantity;
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

bool ResourcesTracker::Release(const Node& node, const Resource& resource,
                               const ResourceQuantity& quantity) {
    ValidateResource(resource);

    auto it = State.find(node);
    if (it == State.end()) {
        return false;
    }

    NodeState& nodeState = it->second;
    assert(nodeState.Current.size() == NumResources);

    if (nodeState.All[resource] - nodeState.Current[resource] <= quantity) {
        nodeState.Current[resource] += quantity;
        return true;
    }

    return false;
}

} // Meteor
