#include "resources_tracker.h"
 
#include <gtest/gtest.h>


TEST(ResourcesTrackerTest, testNoResources) {
    Meteor::ResourcesTracker tracker(0);

    EXPECT_EQ(0, tracker.GetNumNodes());
    EXPECT_EQ(0, tracker.GetNumResources());

    ASSERT_THROW(tracker.Add("node", 1, 1), std::runtime_error);
    ASSERT_THROW(tracker.Acquire("node", 1, 1), std::runtime_error);
}

TEST(ResourcesTrackerTest, testSingleResource) {
    Meteor::ResourcesTracker tracker(1);
    ASSERT_NO_THROW(tracker.Add("node", 0, 1));
    ASSERT_THROW(tracker.Add("node", 1, 1), std::runtime_error);

    EXPECT_EQ(1, tracker.GetNumNodes());
    EXPECT_EQ(1, tracker.GetNumResources());

    EXPECT_TRUE(tracker.Acquire("node", 0, 1));
    EXPECT_FALSE(tracker.Acquire("node", 0, 1));
}

TEST(ResourcesTrackerTest, testUnknownNode) {
    Meteor::ResourcesTracker tracker(1);
    ASSERT_NO_THROW(tracker.Add("nodeA", 0, 1));
    EXPECT_FALSE(tracker.Acquire("node", 0, 1));
}

TEST(ResourcesTrackerTest, testMultiNode) {
    Meteor::ResourcesTracker tracker(1);
    ASSERT_NO_THROW(tracker.Add("nodeA", 0, 1));
    ASSERT_NO_THROW(tracker.Add("nodeB", 0, 1));

    EXPECT_EQ(2, tracker.GetNumNodes());
    EXPECT_EQ(1, tracker.GetNumResources());

    EXPECT_TRUE(tracker.Acquire("nodeA", 0, 1));
    EXPECT_FALSE(tracker.Acquire("nodeA", 0, 1));

    EXPECT_TRUE(tracker.Acquire("nodeB", 0, 1));
    EXPECT_FALSE(tracker.Acquire("nodeB", 0, 1));
}
