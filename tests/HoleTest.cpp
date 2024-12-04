#include <gtest/gtest.h>
#include "../src/Hole.hpp"

TEST(YourTestSuite, TestPositiveScenario) {
    // Arrange
    Pose2D posOne(50, 40);
    Pose2D posTwo(50, 60);
    Hole hole(posOne, posTwo);
    
    Pose2D inSquare[] = {Pose2D(48, 46), Pose2D(55.4, 49.75), Pose2D(51.6, 55.1), Pose2D(46.1, 55.3), Pose2D(50, 44), Pose2D(54, 47.1)};
    
    Pose2D outOfSquare[] = {Pose2D(67.6, 54.6), Pose2D(37, 39.8), Pose2D(61.5, 37.7), Pose2D(39, 62), Pose2D(84.8, 31.3), Pose2D(-43.3, -27.3)};

    for (uint16_t i = 0; i < 6; i++) {
	bool result = hole.isInSquare(inSquare[i]);
	bool result2 = hole.isInSquare(outOfSquare[i]);
	
	// Assert
	EXPECT_EQ(result, true);
	EXPECT_FALSE(result2);
    }
}

/*
TEST(YourTestSuite, TestNegativeScenario) {
    // Similar structure to above test
}
*/

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
