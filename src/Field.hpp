//
// Created by caleb on 10/25/2024
//

#ifndef FIELD_H
#define FIELD_H

#include "Pillar.hpp"
#include "vector"
#include <cstdint> 
#include <memory>

class Field {
  protected:
    std::unique_ptr<std::vector<Pillar>> pillars;
    uint8_t desiredDesitnationPillar;
    Pillar botPose;
    
  public:
    Field(std::vector<Pillar> pillars, uint8_t desiredDestinationPillar, Pillar botPose);
    Field(std::vector<Pillar> pillars, Pillar desiredPillar);
    Field();
};

#endif //FIELD_H
