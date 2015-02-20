#include "fcfs_scheduler.h"


namespace Meteor {

FCFSTracker::FCFSTracker()
    : ResourcesTracker(1)
{
}

bool FCFSTracker::FindFreeNode(const Job& job, Node& node) {
    for (const auto& it : State) {
        if (Acquire(it.first, 0, job.ResQuantity)) {
            node = it.first;
            return true;
        }
    }

    return false;
}

FCFSScheduler::FCFSScheduler(FCFSTracker& tracker)
    : Tracker(tracker)
{
}

EventList FCFSScheduler::Schedule(const Job& job) {
    EventList events = ClearFinished(Tracker);

    Node node;
    if (Tracker.FindFreeNode(job, node)) {
        // Resource is consumed here
        ScheduledJob scheduledJob(job, node);
        Jobs.insert(Timeline::value_type(CurTime + job.Dur, scheduledJob));
        events.push_back(Event(EventType::Scheduled, CurTime, node));
    } else {
        events.push_back(Event(EventType::Rejected, CurTime, Node()));
    }

    ++CurTime;

    return events;
}

} // Meteor
