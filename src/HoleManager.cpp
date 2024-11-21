/**
* Created by Caleb Kugel 11/15/2024
*/

#include "HoleManager.hpp"

HoleManager::HoleManager() {
    holes = std::make_unique<std::vector<Hole>>();
    holeMeasurements = std::make_unique<std::vector<Pose2D>>();
}

void HoleManager::addHole(Pose2D cornerOne, Pose2D cornerTwo) {
    holes->push_back(Hole(cornerOne, cornerTwo));
}


void HoleManager::addHole(Hole hole) {
    holes->push_back(hole);
}

Hole HoleManager::getHole(uint8_t index) {
    if (index > holes->size()) {
	return Hole(NAN, NAN, NAN, NAN);
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

void HoleManager::addPoint(Pose2D position) {
   this->holeMeasurements->push_back(position); 
    /*
    for (uint16_t i = 0; i < this->holes->size(); i++) {
	if (this->holes->data()[i].pointCouldBeMemberOfHole(position)) {
	    this->holes->data()[i].addPoint(position);
	    break;
	}
    }
	 */
}

bool HoleManager::nodeCollides(Pose2D position) {
	// shoot out a small line from the position in the dircection of the heading
	
	for (uint16_t i = 0; i < this->holeMeasurements->size(); i++) {
	    Pose2D initial = this->holeMeasurements->at(i);
	    Pose2D pose2(this->holeMeasurements->at(i)); // should copy
	    pose2.translateByMagnitude(HOLE_SIZE * 0.5);
	    if (position.isOnLine(initial, pose2)) {
		return false;
	    }
	}
    return true;
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

bool HoleManager::lineIntersectsAnyHoleMeasurmenent(Pose2D locations) {
   // we have a pose of each measurement

}


