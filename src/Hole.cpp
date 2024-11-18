/**
* Created by Caleb Kugel on 11/15/2024
*/

#include "Hole.hpp"


Hole::Hole(Pose2D positionOne, Pose2D positionTwo) {
    this->cornerOne = positionOne;
    this->cornerTwo = positionTwo;
}

Pose2D Hole::getOneSquareCorner() {
    return this->cornerOne;
}

Pose2D Hole::getSecondSquareCorner() {
    return this->cornerTwo;
}

bool Hole::isInSquare(Pose2D position) {
    //TODO: Implement
    return 1;
}

Hole::Hole(double X1, double Y1, double X2, double Y2) {
   this->cornerOne = Pose2D(X1, Y1);
    this->cornerTwo = Pose2D(X2, Y2);
}


