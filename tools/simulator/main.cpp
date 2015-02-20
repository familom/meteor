#include "cluster/resources_config_reader.h"
#include "cluster/resources_tracker.h"
#include "scheduler/job.h"
#include "scheduler/scheduler.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace {

template <typename Tracker>
Tracker BuildTracker() {
    using namespace Meteor;

    const std::string CONFIG =
        "nodeA 1\n"
        "nodeB 1\n"
        "nodeC 1\n"
        ;

    std::istringstream configStream(CONFIG);

    Tracker tracker;
    StreamResourcesConfigReader reader(configStream, tracker);
    reader.Run();

    return tracker;
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

    FCFSTracker fcfsTracker = BuildTracker<FCFSTracker>();
    FCFSScheduler fcfsScheduler(fcfsTracker);
    size_t fcfsRejected = 0;
    for (const auto& job : JOBS) {
        Event event = fcfsScheduler.Schedule(job);
        if (event.Type == EventType::Rejected) {
            ++fcfsRejected;
        }
    }

    std::cout << "Rejected by default scheduler: " << fcfsRejected << std::endl;

    return 0;
}
