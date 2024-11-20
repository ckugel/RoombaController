/**
* Created by Caleb Kugel on 11/15/2024
*/

#include "Hole.hpp"


Hole::Hole(Pose2D positionOne, Pose2D positionTwo) {
    this->cornerOne = positionOne;
    this->cornerTwo = positionTwo;
    this->foundHole = false;
    this->points = std::make_unique<std::vector<Pose2D>>();
}

Hole::Hole(Pose2D positionOne, Pose2D positionTwo, bool foundHole, std::vector<Pose2D> points) {
    this->cornerOne = positionOne;
    this->cornerTwo = positionTwo;
    this->foundHole = foundHole;

    this->points = std::make_unique<std::vector<Pose2D>>();
    for (uint16_t i = 0; i < points.size(); i++) {
	this->points->push_back(points[i]);
    }
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
    this->foundHole = false;
}

Hole::Hole() {
    this->foundHole = false;
    this->points = std::make_unique<std::vector<Pose2D>>();
}

Hole::Hole(Pose2D initialPoint) {
    this->foundHole = false;
    this->points = std::make_unique<std::vector<Pose2D>>();
    this->points->push_back(initialPoint);
}

bool Hole::pointCouldBeMemberOfHole(Pose2D measurment) {
    bool viable = true;
    for (uint16_t i = 0; i < this->points->size(); i++) {
	if (this->points->data()[i].distanceTo(measurment) > HOLE_SIZE * std::sqrt(2)) {
	    viable = false;
	}
    }
    return viable;
}

Hole::Hole(const Hole& hole) {
    this->cornerOne = hole.cornerOne;
    this->cornerTwo = hole.cornerTwo;
    this->foundHole = hole.foundHole;

    this->points = std::make_unique<std::vector<Pose2D>>();
    for (uint16_t i = 0; i < hole.points->size(); i++) {
	this->points->push_back(hole.points->data()[i]);

    }
}

void Hole::addPoint(Pose2D position) {
    this->points->push_back(position);
    //TODO: add logic to check if we can morph into found
    //General flow:
    if (this->points->size() > 2) {
	// should be 3 elements
	// pick 2
	Pose2D positionOne = this->points->data()[0];
	Pose2D positionTwo = this->points->data()[1];
	Pose2D positionThree = this->points->data()[2];

	uint16_t pivot = 0; // start with 0 for our guess of a pivot
	
	// First case our third measurment is in a line
	if (positionOne.isOnLine(positionTwo, positionThree)) {
	    this->points->at(0) = positionThree;
	    this->points->pop_back();
	    return ;

	}
	else if (positionTwo.isOnLine(positionOne, positionThree)) {
	    this->points->at(1) = positionThree;
	    this->points->pop_back();
	    return ;
	}
	else if (positionThree.isOnLine(positionOne, positionTwo)) {
	    this->points->pop_back();
	    return ;
	}

	// Case 2: we pick a point, then we have to see if the either other points make a 90 ish degree angle with the other point(s?)
	
	// pick positionOne first
	// find the line that is parallel
	Pose2D perpendicular = positionThree;
	
    }
}

