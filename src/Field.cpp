//
// Created by caleb on 10/25/2024
//

#include "Field.hpp"

Field::Field(std::vector<Pillar> pillars, uint8_t desiredDestinationPillar, Pillar botPose) {
  this->pillars = make_unique<std::vector<Pillar>>(pillars);
}

Field::Field(std::vector<Pillar> pillars, Pillar desiredPillar) {
    this->pillars = make_unique<std::vector<Pillar>>(pillars);
  
    // search for desired pillar
    this->desiredDesitnationPillar = 0xFF;
    this->botPose = Pillar(0, 0, 0);
}

Field::Field() {
  pillars(new std::vector<Pillar>());
  this->desiredDesitnationPillar = 0xFF;
  this->botPose = Pillar(0, 0, 0);
}
