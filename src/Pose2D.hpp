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
    Pose2D();
    Pose2D(const Pose2D& position);
    // Pose2D(Pose2D* position);
    double angleTo(Pose2D other);
    double distanceTo(Pose2D other);
    double squareOfDistanceTo(Pose2D other);
    static Pose2D fromPolar(double magnitude, double angle);
    static double degreesToRadians(double degrees);
    static double radiansToDegrees(double radians);
    Pose2D clone();
    void rotateByPose(Pose2D rotation);
    void rotateByAngle(double angle);
    void translateByPose(Pose2D translation);
    void translateByMagnitude(double magnitude);
    void transformPose(Pose2D modifier);
    double getX();
    double getY();
    double getHeading();
    void plusCoord(Pose2D other);
    void plus(Pose2D other);
    void minus(Pose2D other);
    void addAngle(double angle);

    /**
     * Makes a slope intercept object from two pose2Ds
     * @param one the first position
     * @param two the second position
     * @return Slope intercept object that runs through two points
     */
    // static SlopeIntercept getSlopeIntercept(Pose2D one, Pose2D two);
    

    /**
     * Determines whether a pose is on a given bounded line
     * @param LineEnd1 one endpoint of the line
     * @param LineEnd2 the other endpoint of the line
     */
    bool isOnLine(Pose2D LineEnd1, Pose2D LineEnd2);

    /**
    * gets the angle between points in radians. 
    * @param corner 
    * @param end1 
    * @param end2 
    * @return the angle between all of these points
    */
    static double getAngleBetweenPoints(Pose2D corner, Pose2D end1, Pose2D end2);
};



#endif //POSE2D_H

