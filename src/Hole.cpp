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
}


