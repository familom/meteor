#pragma once

#include "job.h"
#include "time.h"

#include "cluster/node.h"

#include <vector>


namespace Meteor {

enum class EventType {
    Scheduled,
    Rejected,
    Finished
};

struct Event {
    Event(EventType type, Timestamp timestamp, const Node& node)
        : Type(type)
        , HappenedAt(timestamp)
        , HappenedOn(node)
    {
    }

    EventType Type;

    Timestamp HappenedAt;
    Node HappenedOn;
};

using EventList = std::vector<Event>;

} // Meteor
