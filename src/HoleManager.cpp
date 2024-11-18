/**
* Created by Caleb Kugel 11/15/2024
*/

#include "HoleManager.hpp"

HoleManager::HoleManager() {
    points = std::make_unique<std::vector<Pose2D>>();
    holes = std::make_unique<std::vector<Hole>>();
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

