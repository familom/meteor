#pragma once

#include "event.h"
#include "job.h"
#include "scheduler.h"

#include "cluster/resources_tracker.h"


namespace Meteor {

class FCFSTracker : public ResourcesTracker {
public:
    FCFSTracker();

    bool FindFreeNode(const Job& job, Node& node);
};

class FCFSScheduler : public Scheduler {
public:
    explicit FCFSScheduler(FCFSTracker& tracker);

    EventList Schedule(const Job& job) override;

private:
    FCFSTracker& Tracker;
};

} // Meteor
