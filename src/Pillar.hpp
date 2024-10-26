//
// Created by caleb on 10/24/2024
//

#ifndef PILLAR_H
#define PILLAR_H

#include "Pose2D.hpp"

class Pillar {
  protected:
    Pose2D* position;
    double radius;
  public:
    Pillar();
    Pillar(Pose2D position, double radius);
    Pillar(double x, double y, double radius);
    ~Pillar();
    Pose2D* getPose();
    double getX();
    double getY();
    double getRadius();
};

#endif //PILLAR_H
