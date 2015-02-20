#include "cluster/resources_config_reader.h"
#include "cluster/resources_tracker.h"
#include "scheduler/job.h"
#include "scheduler/fcfs_scheduler.h"
#include "scheduler/stat_scheduler.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace {

template <typename TrackerT>
TrackerT BuildTracker() {
    using namespace Meteor;

    const std::string CONFIG =
        "nodeA 1\n"
        "nodeB 1\n"
        "nodeC 1\n"
        ;

    std::istringstream configStream(CONFIG);

    TrackerT tracker;
    StreamResourcesConfigReader reader(configStream, tracker);
    reader.Run();

    return tracker;
}

template <typename TrackerT, typename SchedulerT, typename JobsT>
void TestScheduler(const JobsT& jobs, const std::string& schedulerName) {
    using namespace Meteor;

    std::cout << "Starting " << schedulerName << " Test...\n";

    TrackerT tracker = BuildTracker<TrackerT>();
    SchedulerT scheduler(tracker);
    size_t numRejected = 0;
    for (const auto& job : jobs) {
        EventList events = scheduler.Schedule(job);

        if (!events.empty()) {
            std::cout << "\t Events at " << events.front().HappenedAt << ", "
                      << "scheduling job {"
                      << "duration = " << job.Dur << ", "
                      << "resource = " << job.ResQuantity
                      << "}:\n";

            for (const auto& event : events) {
                switch (event.Type) {
                case EventType::Rejected: {
                    ++numRejected;
                    std::cout << "\t\tjob REJECTED";
                    break;
                }

                case EventType::Scheduled: {
                    std::cout << "\t\tjob SCHEDULED on " << event.HappenedOn;
                    break;
                }

                case EventType::Finished: {
                    std::cout << "\t\tjob FINISHED on " << event.HappenedOn;
                    break;
                }

                default:
                    assert(false);
                }

                std::cout << "\n";
            }
        }
    }

    std::cout << "Finished " << schedulerName << " Test\n";
    std::cout << "Stats: \n"
              << "\tRejected: " << numRejected << std::endl;
}

} // ns

int main(int /* argc */, char* /*argv*/[]) {
    using namespace Meteor;

    // (duration, resource requirements)
    const std::vector<Job> JOBS = {
        {1, 1},
        {1, 1},
        {2, 1},
        {10, 1},
        {10, 1},
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 1},
        {1, 1},
    };

    TestScheduler<FCFSTracker, FCFSScheduler>(JOBS, "First-Come, First-Server Scheduler");

    std::cout << std::endl;

    TestScheduler<StatTracker, StatScheduler>(JOBS, "Statistical Scheduler");

    return 0;
}
