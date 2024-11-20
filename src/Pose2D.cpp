//
// Created by caleb on 10/24/2024
//

#include "Pose2D.hpp"

Pose2D::Pose2D(double x, double y, double heading) {
  this->x = x;
  this->y = y;
  this->heading = heading;
}

Pose2D::Pose2D(double x, double y) {
  this->x = x;
  this->y = y;
}

Pose2D::Pose2D() {
    this->x = 0;
    this->y = 0;
    this->heading = 0;
}

bool Pose2D::isOnLine(Pose2D LineEnd1, Pose2D LineEnd2) {
    double y1 = LineEnd1.getY();
    double x1 = LineEnd1.getX();

    double y2 = LineEnd2.getY();
    double x2 = LineEnd2.getX();

    if (x1 > x2) {
	// need to swap
	double holder = x1;
	x1 = x2;
	x2 = holder;
	holder = y1;
	y1 = y2;
	y2 = holder;
    }

    // base case of the line just being y = 
    if (std::abs(y1 - y2) < 0.1) {
	// straight y = line
	return (this->getX() < x2 && this->getX() > x1);
    }

    // case where the line is just x =
    else if (std::abs(x1 - x2) < 0.1) {
	return (this->getY() < std::fmax(y1, y2) && this->getY() > std::fmin(y1, y2));
    }

    // y = mx + b line can be formed
    else {
	double m, b;	
	// find the equation of the line and plugin this coordinates into it to see if it is close to on the line
	m = (y2 - y1) / (x2 - x1);
	b = y1 - m * x1; // B?
	    
	double wouldBeY = m * this->getX() + b;
	return fabs(wouldBeY - this->getY()) < 0.1;
    }
}

Pose2D::Pose2D(const Pose2D& position) {
    this->x = position.x;
    this->y = position.y;
    this->heading = position.heading;
}

/*
Pose2D::Pose2D(Pose2D* position) {
  this->x = position->x;
  this->y = position->y;
  this->heading = position->heading;
}
*/

double Pose2D::angleTo(Pose2D other) {
  return atan2(other.y - this->y, other.x - this->x);  
}

void Pose2D::addAngle(double angle) {
    this->heading += angle;
}

double Pose2D::distanceTo(Pose2D other) {
  return hypot(this->x - other.x, this->y - other.y);
}

double Pose2D::squareOfDistanceTo(Pose2D other) {
  return pow(other.x -this->x, 2) + pow(other.y - this->y, 2);
}

void Pose2D::plus(Pose2D other) {
    this->x += other.x;
    this->y += other.y;
    this->heading += other.heading;
}

Pose2D Pose2D::clone() {
  return Pose2D(this->x, this->y, this->heading);
}

void Pose2D::rotateByAngle(double angle) {
  double newX = this->x * cos(angle) - this->y * sin(angle);
  this->y = this->x * sin(angle) - this->y * cos(angle);
  this->x = newX;
  this->heading += angle;
}

void Pose2D::plusCoord(Pose2D other) {
    this->x += other.x;
    this->y += other.y;
}

void Pose2D::rotateByPose(Pose2D rotation) {
  rotateByAngle(rotation.heading);
}

void Pose2D::translateByPose(Pose2D translation) {
  this->x += translation.x;
  this->y += translation.y;
} 

void Pose2D::translateByMagnitude(double magnitude) {
  this->x += magnitude * cos(this->heading);
  this->y += magnitude * sin(this->heading);
}

void Pose2D::transformPose(Pose2D modifier) {
  translateByPose(modifier);
  rotateByPose(modifier);
}

double Pose2D::getX() {
  return x;
}
double Pose2D::getY() {
  return y;
}
double Pose2D::getHeading() {
  return heading;
}

void Pose2D::minus(Pose2D other) {
    this->x -= other.x;
    this->y -= other.y;
    this->heading -= other.heading;
}

Pose2D Pose2D::fromPolar(double magnitude, double angle) {
    double x = magnitude * cos(angle);
    double y = magnitude * sin(angle);
    Pose2D pose(x, y, 0);
    return pose;
}

double Pose2D::degreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

Pose2D parseFromStream(std::istringstream& stream) {
    double x, y, heading;
    	if (stream >> x >> y >> heading) {
	    return Pose2D(x, y, heading);
	}

    return Pose2D(0, 0);
}

