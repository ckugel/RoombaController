// 
// Created By Caleb on 10/24/2024
//

#include "Pillar.hpp"

Pillar::Pillar() {
    Pillar(0, 0, 0);
}

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

double Pillar::getX() {
    return this->position->getX();
}

double Pillar::getY() {
    return this->position->getY();
}

double Pillar::getRadius() {
    return this->radius;
}

