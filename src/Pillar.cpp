// 
// Created By Caleb on 10/24/2024
//

#include "Pillar.h"

Pillar::Pillar(Pose2D position, double radius) {
  this->position = position;
  this->radius = radius;
}

Pillar::Pillar(double x, double y, double radius) {
  this->position = pose(x, y);
  this->radius = radius;
}

