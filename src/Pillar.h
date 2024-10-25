//
// Created by caleb on 10/24/2024
//

#ifndef PILLAR_H
#define PILLAR_H

#include "Pose2D.h"

class Pillar {
  protected:
    Pose2D position;
    double radius;
  public:
    Pillar(Pose2D position, double radius);
    Pillar(double x, double y, double radius);
    ~Pillar();
  
  

};

#endif //PILLAR_H
