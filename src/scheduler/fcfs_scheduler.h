#pragma once

#include "event.h"
#include "job.h"
#include "scheduler.h"

#include "cluster/resources_tracker.h"

#include <map>


namespace Meteor {

class FCFSTracker : public ResourcesTracker {
public:
    FCFSTracker();

    bool FindFreeNode(const Job& job, Node& node);
};

class FCFSScheduler : public IScheduler {
public:
    explicit FCFSScheduler(FCFSTracker& tracker);

    Event Schedule(const Job& job) override;

private:
    void CleanFinished();

private:
    struct ScheduledJob : public Job {
        explicit ScheduledJob(const Job& job, const Node& node)
            : ScheduledOn(node)
        {
            Dur = job.Dur;
            ResQuantity = job.ResQuantity;
        }

        Node ScheduledOn;
    };

    FCFSTracker& Tracker;

    Timestamp CurTime;

    using Timeline = std::map<Timestamp, ScheduledJob>;
    Timeline Jobs;
};

} // Meteor
