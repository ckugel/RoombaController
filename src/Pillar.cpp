// 
// Created By Caleb on 10/24/2024
//

#include "Pillar.hpp"

Pillar::Pillar() {
    Pose2D pose(0, 0, 0);
   this->position = pose;
    this->radius = 0;
}

Pillar::Pillar(double x, double y, double heading, double radius) {
    this->radius = radius;
    Pose2D pose(x, y, heading);
    this->position = pose;
}

Pillar::Pillar(Pose2D position, double radius) {
this->position = position; 
  this->radius = radius;
}

Pillar::Pillar(double x, double y, double radius) {
    Pose2D pose(x, y);
    this->position = pose;
  this->radius = radius;
}

Pillar::~Pillar() {
  // delete this->position;
}

Pose2D Pillar::getPose() {
  return this->position;
}

double Pillar::getX() {
    return this->position.getX();
}

double Pillar::getY() {
    return this->position.getY();
}

double Pillar::getRadius() {
    return this->radius;
}


Pillar Pillar::parseFromStream(std::istringstream& stream) {
	double x, y, heading, radius;
	if (stream >> x >> y >> heading >> radius) {
	return Pillar(x, y, heading, radius);
	}
	return Pillar(0, 0, 0);
}

