#pragma once

#include "node.h"
#include "resource.h"

#include <vector>
#include <unordered_map>


namespace Meteor {

/// Base class for tracker of cluster resources.
class IResourcesTracker {
public:
    virtual ~IResourcesTracker() = default;

    virtual size_t GetNumNodes() const = 0;
    virtual size_t GetNumResources() const = 0;

    virtual void Add(const Node& node, const Resource& resource,
                     const ResourceQuantity& quantity) = 0;

    /// @return true if resource was consumed and false otherwise.
    virtual bool Acquire(const Node& node, const Resource& resource,
                         const ResourceQuantity& quantity) = 0;
};

/// Simple thread-unsafe resource tracker.
class ResourcesTracker : public IResourcesTracker {
public:
    ResourcesTracker(size_t numResources);

    size_t GetNumNodes() const override {
        return State.size();
    }

    size_t GetNumResources() const override {
        return NumResources;
    }

    /// Adds resource to the node.
    ///
    /// @throw runtime_error
    void Add(const Node& node, const Resource& resource,
             const ResourceQuantity& quantity) override;

    /// Tries to consume requested resource on the node.
    ///
    /// @throw runtime_error
    ///
    /// @return true if resource was consumed and false otherwise.
    bool Acquire(const Node& node, const Resource& resource,
                 const ResourceQuantity& quantity) override;

private:
    /// @throw runtime_error
    void ValidateResource(const Resource& resource);

protected:
    struct NodeState {
        NodeState(size_t numResources)
            : All(numResources)
            , Current(numResources)
        {
        }

        // Vector of all available resources on node.
        using Resources = std::vector<ResourceQuantity>;
        Resources All; // Represents free node, all resources are available
        Resources Current;
    };

    size_t NumResources;

    using Resources = std::unordered_map<Node, NodeState>;
    Resources State;
};

} // Meteor
