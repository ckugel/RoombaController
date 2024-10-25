// 
// Created By Caleb on 10/24/2024
//

#include "Pillar.h"

Pillar::Pillar(Pose2D position, double radius) {
  this->position = new Pose2D(&position);
  this->radius = radius;
}

Pillar::Pillar(double x, double y, double radius) {
  this->position = new Pose2D(x, y);
  this->radius = radius;
}

Pillar::~Pillar() {
  delete this->position;
}

Pose2D* Pillar::getPose() {
  return this->position;
}

