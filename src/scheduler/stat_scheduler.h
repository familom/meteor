#pragma once

#include "event.h"
#include "job.h"
#include "scheduler.h"

#include "cluster/resources_tracker.h"


namespace Meteor {

class StatTracker : public ResourcesTracker {
public:
    StatTracker();

    bool FindFreeNode(const Job& job, Node& node);
};

/// This scheduler analyzes historical workload.
class StatScheduler : public Scheduler {
public:
    explicit StatScheduler(StatTracker& tracker);

    EventList Schedule(const Job& job) override;

private:
    StatTracker& Tracker;

    long double DurationMean;
    long double ResourceMean;
    size_t NumScheduled;
};

} // Meteor
