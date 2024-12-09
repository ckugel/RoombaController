//
// Created by caleb on 10/24/2024
//

#ifndef PILLAR_H
#define PILLAR_H

#include "Pose2D.hpp"

class Pillar {
  protected:
    Pose2D position;
    double radius;
  public:
    Pillar();
    Pillar(const Pose2D& position, double radius);
    Pillar(double x, double y, double heading, double radius);
    Pillar(double x, double y, double radius);
    //~Pillar();
    Pose2D& getPose();
    double getX();
    double getY();
    [[nodiscard]] double getRadius() const;
    void setRadius(double newRadius);

    void setPosition(const Pose2D& newPosition);

    static Pillar parseFromStream(std::istringstream& stream);
};

#endif //PILLAR_H
