//
// Created by caleb on 10/25/2024
//

#include "Field.hpp"

Field::Field(std::vector<Pillar> pillars, Pose2D desiredDestination, Pillar botPose) {
  this->pillars = std::make_unique<std::vector<Pillar>>(pillars);
    this->desiredDestination = desiredDestination;
    this->botPose = botPose;
}

Field::Field(std::vector<Pillar> pillars, Pose2D desiredDestination) {
    this->pillars = std::make_unique<std::vector<Pillar>>(pillars);
  
    this->desiredDestination = desiredDestination;
    this->botPose = Pillar(0, 0, 0);
}

Field::Field() {
  this-> pillars = std::make_unique<std::vector<Pillar>>();
  this->desiredDestination = Pose2D(0, 0, 0);
  this->botPose = Pillar(0, 0, 0);
}

