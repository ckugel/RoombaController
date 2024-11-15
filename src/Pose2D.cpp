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

