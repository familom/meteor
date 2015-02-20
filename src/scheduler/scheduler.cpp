#include "scheduler.h"

#include "cluster/resources_tracker.h"


namespace Meteor {

Scheduler::Scheduler()
    : CurTime(0)
{
}

EventList Scheduler::ClearFinished(IResourcesTracker& tracker) {
    EventList events;
    while (!Jobs.empty() && CurTime == Jobs.begin()->first) {
        const ScheduledJob& job = Jobs.begin()->second;

        Event event(EventType::Finished, CurTime, job.ScheduledOn);
        events.push_back(event);

        tracker.Release(job.ScheduledOn, 0, job.ResQuantity);
        Jobs.erase(Jobs.begin());
    }

    return events;
}


} // Meteor
