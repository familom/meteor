#pragma once

#include "event.h"
#include "job.h"

#include <map>


namespace Meteor {

class IResourcesTracker;

class Scheduler {
public:
    Scheduler();
    virtual ~Scheduler() = default;

    virtual EventList Schedule(const Job& job) = 0;

protected:
    EventList ClearFinished(IResourcesTracker& tracker);

protected:
    struct ScheduledJob : public Job {
        explicit ScheduledJob(const Job& job, const Node& node)
            : ScheduledOn(node)
        {
            Dur = job.Dur;
            ResQuantity = job.ResQuantity;
        }

        Node ScheduledOn;
    };

    Timestamp CurTime;

    using Timeline = std::map<Timestamp, ScheduledJob>;
    Timeline Jobs;
};

} // Meteor
