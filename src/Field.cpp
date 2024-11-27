//
// Created by caleb on 10/25/2024
//

#include "Field.hpp"

Field::Field(const std::vector<Pillar>& pillars, const Pose2D& desiredDestination, const Pillar& botPose) {
  this->pillars = std::make_unique<std::vector<Pillar>>(pillars);
    this->desiredDestination = desiredDestination;
    this->holeManager = HoleManager();
    this->botPose = botPose;
}

Field::Field(const std::vector<Pillar>& pillars, const Pose2D& desiredDestination) {
    this->pillars = std::make_unique<std::vector<Pillar>>(pillars);
    this->holeManager = HoleManager();
    this->desiredDestination = desiredDestination;
    this->botPose = Pillar(0, 0, 0, BOT_RADIUS);
}

Field::Field() {
  this-> pillars = std::make_unique<std::vector<Pillar>>();
  this->desiredDestination = Pose2D(0, 0, 0);
  this->botPose = Pillar(0, 0, 0, BOT_RADIUS);
    this->holeManager = HoleManager();
}

HoleManager& Field::getManager() {
    return holeManager;
}

void Field::clearField() {
    this->pillars->clear();
}

std::unique_ptr<std::vector<Pillar>> Field::getPillars() {
    return std::move(pillars); 
}

void Field::setPillars(std::unique_ptr<std::vector<Pillar>> pillars) {
    this->pillars = std::move(pillars);
} 

Pillar Field::getBotPose() {
    return botPose;
}

void Field::addPillar(const Pillar& newPillar) {
    this->pillars->push_back(newPillar);
}

void Field::updateBotPose(const Pose2D& updatedPosition) {
    this->botPose.setPosition(updatedPosition);
}

std::vector<Pillar> Field::getCopyPillars() {
    std::vector<Pillar> copy;
    for (const auto & i : *pillars) {
        copy.push_back(i);
    }
    return copy;
}

void Field::addEdgeMeasurement(const double rawPosition, const Cardinality cardinality) {
    switch (cardinality) {
        case N:
            {
                offset.setY(rawPosition);
            }
            break;
        case S:
            {
                offset.setY(-rawPosition);
            }
            break;
        case E:
            {
                offset.setX(rawPosition);
            }
            break;
        case W:
            {
                offset.setX(-rawPosition);
            }
            break;
    }
}

