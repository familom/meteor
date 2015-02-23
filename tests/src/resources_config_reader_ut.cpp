#include "cluster/resources_config_reader.h"
#include "cluster/resources_tracker.h"
 
#include <gtest/gtest.h>

#include <sstream>
#include <string>


TEST(ResourcesConfigReaderTest, testValidConfig) {
    const std::string CONFIG = "nodeA 1";
    std::istringstream configStream(CONFIG);

    Meteor::ResourcesTracker tracker(1);
    Meteor::StreamResourcesConfigReader reader(configStream, tracker);

    ASSERT_NO_THROW(reader.Run());
    EXPECT_EQ(1u, tracker.GetNumNodes());
    EXPECT_TRUE(tracker.Acquire("nodeA", 0, 1));
}
