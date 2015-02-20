#pragma once

#include "event.h"
#include "job.h"

#include "cluster/resources_tracker.h"

#include <map>


namespace Meteor {

class IScheduler {
public:
    virtual ~IScheduler() = default;

    virtual Event Schedule(const Job& job) = 0;
};

class FIFSTracker : public ResourcesTracker {
public:
    FIFSTracker() = default;

    bool FindFreeNode(const Job& job, Node& node);
};

class FIFSScheduler : public IScheduler {
public:
    explicit FIFSScheduler(FIFSTracker& tracker);

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

    FIFSTracker& Tracker;

    Timestamp CurTime;

    using Timeline = std::map<Timestamp, ScheduledJob>;
    Timeline Jobs;
};

} // Meteor
