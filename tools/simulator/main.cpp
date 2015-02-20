#include "cluster/resources_config_reader.h"
#include "cluster/resources_tracker.h"
#include "scheduler/scheduler.h"

#include <sstream>
#include <string>


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

    FCFSTracker fcfsTracker = BuildTracker<FCFSTracker>();

    return 0;
}
