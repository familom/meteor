#pragma once

#include <istream>


namespace Meteor {

class IResourcesTracker;

/// Base class for resources config reading / updating.
///
/// Implementation can be e.g. a daemon listening for configuration changes.
class IResourcesConfigReader {
public:
    virtual ~IResourcesConfigReader() = default;
    virtual void Run() = 0;
};

/// Reads resources config represented in the following format:
///
/// <node_name> <resource0_quantity> <resource1_quantity> ... <resourceN_quantity>
/// <node_name> <resource0_quantity> <resource1_quantity> ... <resourceN_quantity>
/// ...
/// node_name should contains only alphanumeric characters and [_.-].
class StreamResourcesConfigReader : public IResourcesConfigReader {
public:
    StreamResourcesConfigReader(std::istream& input, IResourcesTracker& tracker);

    /// @throw runtime_error (e.g. if format is invalid)
    void Run() override;

private:
    std::istream& Input;
    IResourcesTracker& Tracker;
};

} // Meteor
