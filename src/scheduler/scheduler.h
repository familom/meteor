#pragma once

#include "event.h"
#include "job.h"


namespace Meteor {

class IScheduler {
public:
    virtual ~IScheduler() = default;

    virtual Event Schedule(const Job& job) = 0;
};

} // Meteor
