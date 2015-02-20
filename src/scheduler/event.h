#pragma once

#include "time.h"


namespace Meteor {

enum class EventType {
    Scheduled,
    Rejected
};

struct Event {
    Event(EventType type, Timestamp timestamp)
        : Type(type)
        , Moment(timestamp)
    {
    }

    EventType Type;
    Timestamp Moment;
};

} // Meteor
