#include "scheduler.h"


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
    , CurTime(0)
{
}

void FCFSScheduler::CleanFinished() {
    while (!Jobs.empty() && CurTime == Jobs.begin()->first) {
        const ScheduledJob& job = Jobs.begin()->second;
        Tracker.Release(job.ScheduledOn, 0, job.ResQuantity);
        Jobs.erase(Jobs.begin());
    }
}

Event FCFSScheduler::Schedule(const Job& job) {
    CleanFinished();

    Node node;
    if (Tracker.FindFreeNode(job, node)) {
        // Resource is consumed here
        Jobs.insert(Timeline::value_type(CurTime + job.Dur, ScheduledJob(job, node)));
        return Event(EventType::Scheduled, CurTime++);
    } else {
        return Event(EventType::Rejected, CurTime++);
    }
}

} // Meteor
