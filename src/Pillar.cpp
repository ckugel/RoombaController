// 
// Created By Caleb on 10/24/2024
//

#include "Pillar.hpp"

Pillar::Pillar() {
   this->position = Pose2D(0, 0, 0);
    this->radius = 0;
}

Pillar::Pillar(const double x, const double y, const double heading, const double radius) {
    this->radius = radius;
    Pose2D pose(x, y, heading);
    this->position = pose;
}

Pillar::Pillar(const Pose2D& position, const double radius) {
    this->position = position;
    this->radius = radius;
}

Pillar::Pillar(const double x, const double y, const double radius) {
    Pose2D pose(x, y);
    this->position = pose;
  this->radius = radius;
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

double Pillar::getRadius() const {
    return this->radius;
}

void Pillar::setRadius(double newRadius) {
    this->radius = newRadius;
}


Pillar Pillar::parseFromStream(std::istringstream& stream) {
    double x, y, heading, radius;
	if (stream >> x >> y >> heading >> radius) {
	    return Pillar(x, y, heading, radius);
	}
	return Pillar(0, 0, 0);
}

void Pillar::setPosition(const Pose2D& newPosition) {
    this->position = newPosition;
}

