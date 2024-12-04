#include <gtest/gtest.h>
#include "../src/Hole.hpp"
#include "../src/Field.hpp"

TEST(testSuite, TestPositiveScenario) {
	// Arrange
	Pose2D posOne(50, 40);
	Pose2D posTwo(50, 60);
	Hole hole(posOne, posTwo);

	Pose2D inSquare[] = {Pose2D(48, 46), Pose2D(55.4, 49.75), Pose2D(51.6, 55.1), Pose2D(46.1, 55.3), Pose2D(50, 44), Pose2D(54, 47.1), Pose2D(50.4, 48.9)};

	Pose2D outOfSquare[] = {Pose2D(67.6, 54.6), Pose2D(37, 39.8), Pose2D(61.5, 37.7), Pose2D(39, 62), Pose2D(84.8, 31.3), Pose2D(-43.3, -27.3), Pose2D(29.4, 41.8)};

	for (uint16_t i = 0; i < 7; i++) {
		std::cout << "should be true" << std::endl;
		bool result = hole.isInSquare(inSquare[i]);
		std::cout << "should not be true" << std::endl;
		bool result2 = hole.isInSquare(outOfSquare[i]);

		std::cout << std::endl;
		// Assert
		EXPECT_EQ(result, true);
		EXPECT_FALSE(result2);
	}
}

TEST(testSuite, TestBoundaries) {

    // Arrange
    Pose2D posOneOne(50, 40);
    Pose2D posTwoOne(50, 60);
    Pose2D posThree(-100, 0);
    Pose2D posFour(-2, -2);
    Pose2D posFive(300, 300);
    Pose2D posSix(500, 500);
    Pose2D posSeven(-1, -1);

    Field field;

    EXPECT_FALSE(field.outOfBounds(posOneOne));
    EXPECT_FALSE(field.outOfBounds(posTwoOne));
    EXPECT_TRUE(field.outOfBounds(posThree));
    EXPECT_TRUE(field.outOfBounds(posFour));
    EXPECT_TRUE(field.outOfBounds(posFive));
    EXPECT_TRUE(field.outOfBounds(posSix));
    EXPECT_TRUE(field.outOfBounds(posSeven));
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
