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

double Pose2D::angleTo(const Pose2D& other) const {
  return atan2(other.y - this->y, other.x - this->x);  
}

void Pose2D::addAngle(double angle) {
    this->heading += angle;
}

double Pose2D::distanceTo(const Pose2D& other) const {
  return hypot(this->x - other.x, this->y - other.y);
}

double Pose2D::squareOfDistanceTo(const Pose2D& other) const {
  return pow(other.x -this->x, 2) + pow(other.y - this->y, 2);
}

void Pose2D::transformForPose(const Pose2D& other) {
    rotateByPose(other);
    this->x += other.x;
    this->y += other.y;
}

void Pose2D::plus(const Pose2D& other) {
    this->x += other.x;
    this->y += other.y;
    this->heading += other.heading;
}

Pose2D Pose2D::clone() const {
  return {this->x, this->y, this->heading};
}

void Pose2D::vecAdd(const double angle, const double magnitude) {
    this->x += magnitude * cos(angle);
    this->y += magnitude * sin(angle);
}


void Pose2D::rotateByAngle(double angle) {
  double newX = this->x * cos(angle) - this->y * sin(angle);
  this->y = this->x * sin(angle) + this->y * cos(angle);
  this->x = newX;
  this->heading += angle;
}

void Pose2D::plusCoord(const Pose2D& other) {
    this->x += other.x;
    this->y += other.y;
}

void Pose2D::rotateByPose(const Pose2D& rotation) {
  rotateByAngle(rotation.heading);
}

void Pose2D::translateByPose(const Pose2D& translation) {
  this->x += translation.x;
  this->y += translation.y;
} 

void Pose2D::translateByMagnitude(double magnitude) {
  this->x += magnitude * cos(this->heading);
  this->y += magnitude * sin(this->heading);
}

void Pose2D::transformPose(const Pose2D& modifier) {
  translateByPose(modifier);
  rotateByPose(modifier);
}

double Pose2D::getX() const {
  return x;
}

double Pose2D::getY() const {
  return y;
}

double Pose2D::getHeading() const {
  return heading;
}

void Pose2D::setHeading(double angle) {
    this->heading = angle;
}

void Pose2D::setX(double x) {
    this->x = x;
}

void Pose2D::setY(double y) {
    this->y = y;
}

uint8_t Pose2D::getQuadrant() const {
    if (x == 0 || y == 0) {
        return 0;
    }
    if (x < 0) {
        if (y < 0) {
            return 3;
        }
        return 2;
    }
    if (y > 0) {
        return 1;
    }
    return 4;
}

void Pose2D::minus(const Pose2D other) {
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

Pose2D Pose2D::parseFromStream(std::istringstream& stream) {
    double x, y, heading;
    	if (stream >> x >> y >> heading) {
	    return {x, y, heading};
	}

    return {0, 0};
}

Pose2D Pose2D::subtractBy(const Pose2D& other) const {
    return {this->x - other.x, this->y - other.y};
}

Pose2D Pose2D::scaleBy(double scalar) const {
    return {this->x * scalar, this->y * scalar};
}

Pose2D Pose2D::normalize() const {
    double squared = sqrt(pow(this->x, 2) + pow(this->y, 2));
    return {this->x / squared, this->y / squared};
}

double Pose2D::dotProduct(const Pose2D& other) const {
    return this->x * other.x + this->y * other.y;
}

std::ostream &operator<<(std::ostream &os, const Pose2D &d) {
    os << "x: " << d.x << " y: " << d.y << " heading: " << d.heading;
    return os;
}

void Pose2D::wrapHeading() {
    heading = fmod(heading, 2 * M_PI);

    if (heading > M_PI) {
        heading -= 2 * M_PI;
    }

    if (heading < - M_PI) {
        heading += 2 * M_PI;
    }
}


Rectangle makeRectangleFromLine(const Pose2D& L1, const Pose2D& L2, double width) {
    double angleBetweenPoints = L1.angleTo(L2);
    double lengthBetween = L1.distanceTo(L2);
    Pose2D head(L1);
    head.setHeading(angleBetweenPoints - M_PI);
    head.translateByMagnitude(width);
    Pose2D r1(head);
    head.addAngle(M_PI);
    head.translateByMagnitude(lengthBetween);
    Pose2D r2(head);
    head.addAngle(M_PI);
    head.translateByMagnitude(width * 2);
    Pose2D r3(head);
    head.addAngle(M_PI);
    head.translateByMagnitude(lengthBetween);
    
    return (Rectangle) {r1, r2, r3, head};
}

void Pose2D::multiply(double d) {
    this->x *= d;
    this->y *= d;
}
