#include "cluster/resources_config_reader.h"
#include "cluster/resources_tracker.h"
#include "scheduler/job.h"
#include "scheduler/fcfs_scheduler.h"

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

template <typename JobsT>
void TestFCFSScheduler(const JobsT& jobs) {
    using namespace Meteor;

    std::cout << "Starting First-Come, First-Served Scheduler test...\n";

    FCFSTracker fcfsTracker = BuildTracker<FCFSTracker>();
    FCFSScheduler fcfsScheduler(fcfsTracker);
    size_t numRejected = 0;
    for (const auto& job : jobs) {
        Event event = fcfsScheduler.Schedule(job);

        std::cout << "\t Time " << event.Moment
                  << "\tJob {"
                  << "duration = " << job.Dur << ", "
                  << "resource = " << job.ResQuantity
                  << "}\t";

        switch (event.Type) {
        case EventType::Rejected: {
            ++numRejected;
            std::cout << "REJECTED";
            break;
        }

        case EventType::Scheduled: {
            std::cout << "SCHEDULED";
            break;
        }

        default:
            assert(false);
        }

        std::cout << std::endl;
    }

    std::cout << "Finished First-Come, First-Served Scheduler test\n";
    std::cout << "Stats: \n"
              << "\tRejected: " << numRejected << std::endl;
}

} // ns

int main(int /* argc */, char* /*argv*/[]) {
    using namespace Meteor;

    const std::vector<Job> JOBS = {
        {1, 1},
        {1, 1},
        {100, 1},
        {1, 1},
        {1, 1},
        {1, 1}
    };

    TestFCFSScheduler(JOBS);

    return 0;
}
