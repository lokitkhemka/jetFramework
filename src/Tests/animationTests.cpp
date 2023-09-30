
#include<Animation/animation.h>
#include<gtest/gtest.h>

using namespace jet;

TEST(Frame, Constructors) {
    Frame frame;
    EXPECT_EQ(0u, frame.Index);
    EXPECT_DOUBLE_EQ(1.0 / 60.0, frame.TimeIntervalInSeconds);
}

TEST(Frame, TimeInSeconds) {
    Frame frame;

    frame.Index = 180;

    EXPECT_DOUBLE_EQ(3.0, frame.TimeInSeconds());
}

TEST(Frame, Advance) {
    Frame frame;

    frame.Index = 45;

    for (int i = 0; i < 9; ++i) {
        frame.Advance();
    }

    EXPECT_EQ(54u, frame.Index);

    frame.Advance(23);

    EXPECT_EQ(77u, frame.Index);

    EXPECT_EQ(78u, (++frame).Index);

    EXPECT_EQ(78u, (frame++).Index);

    EXPECT_EQ(79u, frame.Index);
}