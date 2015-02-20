#pragma once

#include "time.h"
#include "cluster/resource.h"


namespace Meteor {

struct Job {
    Job() = default;

    Job(Duration duration, ResourceQuantity resource)
        : Dur(duration)
        , ResQuantity(resource)
    {
    }

    Duration Dur;
    ResourceQuantity ResQuantity;
};

} // Meteor
