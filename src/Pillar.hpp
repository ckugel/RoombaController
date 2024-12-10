//
// Created by caleb on 10/24/2024
//

#ifndef PILLAR_H
#define PILLAR_H

#include "Pose2D.hpp"

/**
 * A Pillar is a cyclinder on the field.
 * It has both a position and a radius.
 */
class Pillar {
  protected:
    /**
     * Pose to handle position and related math
     */
    Pose2D position;

    /**
     * The radius of the pillar
     */
    double radius;

  public:
    /**
     * Constructs a new pillar object
     */
    Pillar();

    /**
     * Makes a new pillar with given position and radius
     * @param position the position of the pillar on the field
     * @param radius the radius of the pillar on the field
     */
    Pillar(const Pose2D& position, double radius);

    /**
     * Constructs a new pillar from components
     * @param x component of position
     * @param y component of position
     * @param heading component of position
     * @param radius of the pillar
     */
    Pillar(double x, double y, double heading, double radius);

    /**
     * Constructs a new pillar from components. ignores heading
     * @param x the x component of the position
     * @param y the y component of the position
     * @param radius the radius of the pillar
     */
    Pillar(double x, double y, double radius);
    //~Pillar();
    /**
     * Gets a reference to the position of the pillar.
     * If Pose2D functions are called on that reference it will effect this pillars position
     * @return the position of the pillar
     */
    Pose2D& getPose();

    /**
     * @return The X compoenent of the position
     */
    double getX();

    /**
     * @return The y component of the positoon
     */
    double getY();

    /**
     * @return the radius of the pillar
     */
    [[nodiscard]] double getRadius() const;

    /**
     * Sets the radius of the pillar
     * @param newRadius the radius to set to
     */
    void setRadius(double newRadius);

   /**
     * Sets the position of the pillar
     * @param newPosition the new position to set to
     */
    void setPosition(const Pose2D& newPosition);

    /**
     * Makes a pillar from a given string stream.
     * Formatted as: X Y theta R
     * @param stream the stream in which to parse the pillar from
     * @return the pillar from the data in the stream
     */
    static Pillar parseFromStream(std::istringstream& stream);
};

#endif //PILLAR_H
