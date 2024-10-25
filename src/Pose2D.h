//
// Created by caleb on 10/24/2024
//


#ifndef POSE2D_H
#define POSE2D_H

#include <cmath>

class Pose2D {
  protected:
    double x, y, heading;

  public:
    Pose2D(double x, double y, double heading);
    Pose2D(double x, double y);
    double angleTo(Pose2D other);
    double distanceTo(Pose2D other);
    double squareOfDistanceTo(Pose2D other);
    static Pose2D createPoseFromAngleMagnitude(double angle, double magnitude);
    static double degreesToRadians(double degrees);
    static double radiansToDegrees(double radians);
    Pose2D clone();
    void rotateByPose(Pose2D rotation);
    void rotateByAngle(double angle);
    void translateByPose(Pose2D translation);
    void translateByMagnitude(double magnitude);
    void transformPose(Pose2D modifier);

};



#endif //POSE2D_H

