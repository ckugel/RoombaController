/**
* Created by Caleb Kugel 11/15/2024
*/

#include <iostream>
#include "HoleManager.hpp"

HoleManager::HoleManager() {
    holes = std::make_unique<std::vector<Hole>>();
    holeMeasurements = std::make_unique<std::vector<Pose2D>>();
}

void HoleManager::addHole(const Pose2D& cornerOne, const Pose2D& cornerTwo) {
    this->holes->push_back(Hole(cornerOne, cornerTwo));
}

void HoleManager::addHole(const Hole& hole) {
    this->holes->push_back(hole);
}

Hole HoleManager::getHole(uint8_t index) {
    if (index > holes->size()) {
	return {NAN, NAN, NAN, NAN};
    }
    else {
	return holes->data()[index];
    }
}

std::vector<Hole> HoleManager::getHoles() {
    std::vector<Hole> toCpy;
    for (uint8_t i = 0; i < holes->size(); i++) {
	toCpy.push_back(holes->data()[i]); // Unsafe lol
    }
    return toCpy;
}

void HoleManager::addPoint(const Pose2D& position) {
   this->holeMeasurements->push_back(position); 
    for (uint16_t i = 0; i < this->holes->size(); i++) {
		if (this->holes->data()[i].pointCouldBeMemberOfHole(position)) {
			this->holes->data()[i].addPoint(position);
			break;
		}
    }
}

// deiscretization step
bool HoleManager::nodeCollides(Pose2D position) {
	// shoot out a small line from the position in the dircection of the heading
	for (uint16_t i = 0; i < this->holeMeasurements->size(); i++) {
	    Pose2D initial = this->holeMeasurements->at(i);
	    Pose2D pose2(this->holeMeasurements->at(i)); // should copy
	    pose2.translateByMagnitude(HOLE_SIZE * 0.5);
	    if (position.isOnLine(initial, pose2)) {
            return true;
	    }
	}


	for (uint8_t i = 0; i < this->holes->size(); i++) {
		if (this->holes->at(i).isInSquare(position)) {
            // std::cout << "failed for: " << position.getX() << ", " << position.getY() << std::endl;
			return true;
		}
	}
    return false;
}

std::vector<Pose2D> HoleManager::getSuggestedNodePlacements() {
    // for every hole return a few points around the square
    
    // for all incomplete holes return an estimate for where you can go
    std::vector<Pose2D> toReturn;
   // basically make sure that none of these are on a collision
    for (uint16_t i = 0; i < this->holeMeasurements->size(); i++) {
		Pose2D suggested(this->holeMeasurements->at(i));
		suggested.addAngle(M_PI);
		suggested.translateByMagnitude(BOT_RADIUS);
		toReturn.push_back(suggested);
    }
    return toReturn;

}

// weighting step
bool HoleManager::lineIntersectsAnyHoleMeasurement(const Pose2D& positionOne, const Pose2D& positionTwo) {
    // we have a pose of each measurement
    // have to make the rectangle and the expected line
    
    for (uint16_t i = 0; i < this->holeMeasurements->size(); i++) {
	// for every point make the rectangle
	Rectangle rect = makeRectangleFromLine(positionOne, positionTwo, MEASUREMENT_WIDTH);	
		if (lineIntersectsRectangle(positionOne.getX(), positionOne.getY(), positionTwo.getX(), positionTwo.getY(), rect.r1.getX(), rect.r1.getY(), rect.r2.getX(), rect.r2.getY(), rect.r3.getX(), rect.r3.getY(), rect.r4.getX(), rect.r4.getY())) {
			return true;
		}
    }

    for (uint16_t i = 0; i < this->holes->size(); i++) {
        Pose2D positionOne = this->holes->at(i).getOneSquareCorner();
        Pose2D positionTwo = this->holes->at(i).getSecondSquareCorner();
        Rectangle rect = makeRectangleFromLine(positionOne, positionTwo, positionOne.distanceTo(positionTwo) / sqrt(2));
        if (lineIntersectsRectangle(positionOne.getX(), positionOne.getY(), positionTwo.getX(), positionTwo.getY(), rect.r1.getX(), rect.r1.getY(), rect.r2.getX(), rect.r2.getY(), rect.r3.getX(), rect.r3.getY(), rect.r4.getX(), rect.r4.getY())) {
			return true;
		}
    }


    return false;
}


