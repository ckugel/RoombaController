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
    Pose2D desiredDestination;
    Pillar botPose;
    
  public:
    Field(std::vector<Pillar> pillars, Pose2D desiredDestination, Pillar botPose);
    Field(std::vector<Pillar> pillars, Pose2D desiredDestination);
    Field();

    void addPillar(Pillar newPillar);
    void updateBotPose(Pose2D updatedPosition);
    std::unique_ptr<std::vector<Pillar>> getPillars();

};

#endif //FIELD_H
