//
// Created by caleb on 10/24/2024
//


#ifndef POSE2D_H
#define POSE2D_H

#include <cmath>
#include <cstdint>
#include <sstream>
#include <ostream>

/**
 * The bot radius in centimeters
 */
#define BOT_RADIUS 6

/**
 * A class representing a 2D position and a heading.
 * Implements common vector functions
 */
class Pose2D {
  protected:
    double x, y, heading;

  public:
    /**
     * Consctructs a new Pose2D object
     * @param x the x component
     * @param y the y component
     * @param heading the heading
     */
    Pose2D(double x, double y, double heading);

    /**
     * Constructs a new Pose2D object
     * @param x component
     * @param y component
     */
    Pose2D(double x, double y);

    /**
     * Makes a new Pose2D with 0's as default
     */
    Pose2D();

    /**
     * Constructs a new Pose2D.
     * Copy constructor.
     * @param position the pose to copy
     */
    Pose2D(const Pose2D& position);
    // Pose2D(Pose2D* position);
    /**
     * Calculates the angle from one node to another
     * @param other angle to get to
     * @return the angle between two nodes
     */
    [[nodiscard]] double angleTo(const Pose2D& other) const;

    /**
     * Calculates the distance between two nodes
     * @param other the other node to calculate the distance to
     * @return the distance between the two nodes
     */
    [[nodiscard]] double distanceTo(const Pose2D& other) const;

    /**
     * calculate the squared distance to another object
     * @param other the object to get the squared distance to
     * @return the squared distance to the other object
     */
    [[nodiscard]] double squareOfDistanceTo(const Pose2D& other) const;

    /**
     * Creates a new Pose2D from polar coordinats
     * @param magnitude the magnitude of the position
     * @param angle the angle of the position
     * @return A Pose2D object representing the polar coordinates
     */
    static Pose2D fromPolar(double magnitude, double angle);

    /**
     * Calculates the radian equivalent of degrees
     * @param degrees the angle in degrees
     * @return the angle in radians
     */
    static double degreesToRadians(double degrees);

    /**
     * Calculates the degree equivalent of radians
     * @param radians the angle in radians
     * @return the angle in degrees
     */
    static double radiansToDegrees(double radians);

    /**
     * Makes a copy of this node
     * @return the copy of this node
     */
    [[nodiscard]] Pose2D clone() const;

    /**
     * Rotate a Pose2D by the a rotation cooresponding to the heading of the other pose
     * @param rotation the pose to rotate by
     */
    void rotateByPose(const Pose2D& rotation);

    /**
     * Uses a rotation matrix to rotate a Pose2D by an angle.
     * Rotates about the origin.
     * @param angle to rotate by.
     */
    void rotateByAngle(double angle);

    /**
     * Translation for a pose by another pose.
     * Vector addition
     * @param translation the pose to add
     */
    void translateByPose(const Pose2D& translation);

    /**
     * This is admitedly a bit of a goofy function.
     * We take the magnitude of the translation and extend the current position by that much in the heading of the pose.
     * I like to think of the Pose2D as a vector, then we take the heading and combine it into a pollar coordinate with the magnitude.
     * Then we do vector addition on the Pose2D.
     * @param magnitude to extend by
     */
    void translateByMagnitude(double magnitude);

    /**
     * transform a position with a rotation and a translation
     * @param modifier the heading gets used as the rotation. The compoenent get used as the translation
     */
    void transformPose(const Pose2D& modifier);

    /**
     * get the x component of the position
     * @return the c component
     */
    [[nodiscard]] double getX() const;

    /**
     * gets the y component of the position
     * @return the y component
     */
    [[nodiscard]] double getY() const;

    /**
     * get the heading of the position
     * @return the heading of the bot
     */
    [[nodiscard]] double getHeading() const;

    /**
     * Sets the heading of the Pose
     * @param angle the angle to set the Pose at
     */
    void setHeading(double angle);

    /**
     * Sum of two coordinates
     * @param other the node to add
     */
    void plusCoord(const Pose2D& other);

    /**
     * Computes a vector addition.
     * Adds components of vectors together
     * @param other the other pose
     */
    void plus(const Pose2D& other);

    /**
     * Subtract this by other.
     * Subtracts the components
     * @param other the other node to subtract by
     */
    void minus(Pose2D other);

    /**
     * Add the angle to heading
     * @param angle gets added to "this" heading
     */
    void addAngle(double angle);

    /**
     * Subtract "this" from other by components and return the result
     * @param other the other node to subtract by
     * @return the result of the subtraction
     */
    [[nodiscard]] Pose2D subtractBy(const Pose2D& other) const;

    /**
     * Scales a position by another. Returs result
     * @param scaler amount to scale this vector as a pose by
     * @return the result of the scaling operation
     */
    [[nodiscard]] Pose2D scaleBy(double scaler) const;

    /**
     * Normalizes a position to a unit vector
     * @return a unit vector representing this position
     */
    [[nodiscard]] Pose2D normalize() const;

    /**
     * Calculate the dot product from the nodes: "this" and other
     * @param other other node to calculate the dot product with
     * @return a double resembling the dot product
     */
    [[nodiscard]] double dotProduct(const Pose2D& other) const;

    /**
     * To string for a pose2D
     * @param os output stream
     * @param d the pose2D to reference
     * @return
     */
    friend std::ostream &operator<<(std::ostream &os, const Pose2D &d);

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
     * If the pose is on the origin or on an intercept it return's 0.
     * otherwise going from pos pos (1) it travels counter clockwise around in a circle
     * @return
         * (0, 0): 0
         * (+, +): 1
         * (-, +): 2
         * (-, -): 3
         * (+, -): 4
     */
    [[nodiscard]] uint8_t getQuadrant() const;

    /**
     * Wraps a heading to - PI -> PI
     */
    void wrapHeading();

    /**
     * transforms a given pose for another's frame
     * @param other the other frame
     */
    void transformForPose(const Pose2D& other);

    /**
     * Parse a pose 2d from a string stream
     * @param stream the stream to parse from
     * @return a pose2D from the data in the stream
     */
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

    /**
     * Multiplys the x and y components by a scalar
     */
    void multiply(double d);
};

/**
 * Struct to resemble the four corners of a rectangle
 */
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
Rectangle makeRectangleFromLine(const Pose2D& L1, const Pose2D& L2, double width);

#endif //POSE2D_H

