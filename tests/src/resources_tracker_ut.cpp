#include "resources_tracker.h"
 
#include <gtest/gtest.h>


TEST(ResourcesTrackerTest, testNoResources) {
    Meteor::ResourcesTracker tracker(0);
    ASSERT_THROW(tracker.Add("node", 1, 1), std::runtime_error);
    ASSERT_THROW(tracker.Acquire("node", 1, 1), std::runtime_error);
}

TEST(ResourcesTrackerTest, testSingleResource) {
    Meteor::ResourcesTracker tracker(1);
    ASSERT_NO_THROW(tracker.Add("node", 0, 1));
    ASSERT_THROW(tracker.Add("node", 1, 1), std::runtime_error);

    EXPECT_TRUE(tracker.Acquire("node", 0, 1));
    EXPECT_FALSE(tracker.Acquire("node", 0, 1));
}
