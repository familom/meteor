#include "stat_scheduler.h"

#include <boost/optional.hpp>

#include <cmath>


namespace {

// 10%
const long double DEFAULT_TOLERANCE = 0.1;

long double CalcMean(long double curMean, size_t numItems, long double newItem) {
    long double newMean = (curMean * numItems + newItem) / (numItems + 1);
    return newMean;
}

bool IsDiffAllowed(long double diff, long double mean,
                   long double tolerance = DEFAULT_TOLERANCE) {
    // 10% burst is allowed
    if (fabs(diff / mean) <= tolerance) {
        return true;
    }

    return false;
}

} // ns

namespace Meteor {

StatTracker::StatTracker()
    : ResourcesTracker(1)
{
}

bool StatTracker::FindFreeNode(const Job& job, Node& node) {
    ResourceQuantity bestDiff = std::numeric_limits<ResourceQuantity>::max();
    boost::optional<Node> bestNode;
    for (const auto& it : State) {
        ResourceQuantity resAvailable = it.second.Current[0];
        if (job.ResQuantity <= resAvailable) {
            ResourceQuantity diff = resAvailable - job.ResQuantity;
            if (diff <= bestDiff) {
                bestDiff = diff;
                bestNode = it.first;
            }
        }
    }

    if (bestNode) {
        node = *bestNode;
        Acquire(node, 0, job.ResQuantity);
        return true;
    }

    return false;
}

StatScheduler::StatScheduler(StatTracker& tracker)
    : Tracker(tracker)
    , DurationMean(0.0)
    , ResourceMean(0.0)
    , NumScheduled(0)
{
}

EventList StatScheduler::Schedule(const Job& job) {
    EventList events = ClearFinished(Tracker);

    bool diffIsAllowed = true;
    if (NumScheduled > 0) {
        long double durationDiff = job.Dur - DurationMean;
        long double resourceDiff = job.ResQuantity - ResourceMean;

        diffIsAllowed = (IsDiffAllowed(durationDiff, DurationMean)
                         && IsDiffAllowed(resourceDiff, ResourceMean));
    }

    Node node;
    if (diffIsAllowed && Tracker.FindFreeNode(job, node)) {
        // Resource is consumed here
        ScheduledJob scheduledJob(job, node);
        Jobs.insert(Timeline::value_type(CurTime + job.Dur, scheduledJob));
        events.push_back(Event(EventType::Scheduled, CurTime, node));

        DurationMean = CalcMean(DurationMean, NumScheduled, job.Dur);
        ResourceMean = CalcMean(ResourceMean, NumScheduled, job.ResQuantity);

        ++NumScheduled;
    } else {
        events.push_back(Event(EventType::Rejected, CurTime, Node()));
    }

    ++CurTime;

    return events;
}

} // Meteor
