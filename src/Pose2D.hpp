//
// Created by caleb on 10/24/2024
//


#ifndef POSE2D_H
#define POSE2D_H

#include <cmath>
#include <cstdint>
#include <sstream>

#define BOT_RADIUS 6

class Pose2D {
  protected:
    double x, y, heading;

  public:
    Pose2D(double x, double y, double heading);
    Pose2D(double x, double y);
    Pose2D();
    Pose2D(const Pose2D& position);
    // Pose2D(Pose2D* position);
    double angleTo(const Pose2D& other) const;
    double distanceTo(const Pose2D& other) const;
    double squareOfDistanceTo(const Pose2D& other) const;
    static Pose2D fromPolar(double magnitude, double angle);
    static double degreesToRadians(double degrees);
    static double radiansToDegrees(double radians);
    Pose2D clone() const;
    void rotateByPose(const Pose2D& rotation);
    void rotateByAngle(double angle);
    void translateByPose(const Pose2D& translation);
    void translateByMagnitude(double magnitude);
    void transformPose(const Pose2D& modifier);
    [[nodiscard]] double getX() const;
    [[nodiscard]] double getY() const;
    [[nodiscard]] double getHeading() const;
    void setHeading(double angle);
    void plusCoord(const Pose2D& other);


    /**
     * Computes a vector addition.
     * Adds components of vectors together
     * @param other the other pose
     */
    void plus(const Pose2D& other);
    void minus(const Pose2D other);
    void addAngle(double angle);

    [[nodiscard]] Pose2D subtractBy(const Pose2D& other) const;
    [[nodiscard]] Pose2D scaleBy(double scaler) const;
    [[nodiscard]] Pose2D normalize() const;
    [[nodiscard]] double dotProduct(const Pose2D& other) const;

    /**
     * Setter for x
     * @param x the new x value
     */
    void setX(double x);

    /**
     * Computes a vector addition from an angle and quantity
     * @param angle the angle component of the vector
     * @param magnitude the quantity component of the vector
     */
    void vecAdd(double angle, double magnitude);

    /**
     * Setter for y
     * @param y the new y value
     */
    void setY(double y);

    /**
     * Gets the quadrant that a Pose is in.
     * If the pose is on the origin it return's 0.
     * otherwise going from pos pos (1) it travels counter clockwise around in a circle
     * @return
         * (0, 0): 0
         * (+, +): 1
         * (-, +): 2
         * (-, -): 3
         * (+, -): 4
     */
    [[nodiscard]] uint8_t getQuadrant() const;

    static Pose2D parseFromStream(std::istringstream& stream);

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
     * Determines whether a position is perpendicular to a given line
     */
    bool isPerpendicularToLine(double m);

    /**
    * gets the angle between points in radians. 
    * @param corner 
    * @param end1 
    * @param end2 
    * @return the angle between all of these points
    */
    static double getAngleBetweenPoints(Pose2D corner, Pose2D end1, Pose2D end2);
};

struct Rectangle {
    Pose2D r1, r2, r3, r4;
};

/**
* Makes a rectangle from a line
* @param L1 One bound of the line
* @param L2 Another bound of the line
* @param width the width of the rectangle
* @return a rectangle with the the line in the middle and a width of the width passed in
*/
Rectangle makeRectangleFromLine(Pose2D L1, const Pose2D& L2, double width);

#endif //POSE2D_H

