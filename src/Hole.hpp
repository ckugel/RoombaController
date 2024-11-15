/**
* Made by Caleb Kugel on 11/15/2024
*/

#ifndef HOLE_H
#define HOLE_H

#include "vector"
#include <memory>
#include "Pose2D.hpp"

#define sideLength 20.0 // FIND THIS

class Hole {
    private:
	std::unique_ptr<std::vector<Pose2D>> points;
	Pose2D cornerOne;
	Pose2D cornerTwo;
    public:
	/**
	 * Creates a new Hole object
	 */
	Hole();

	/**
	* Adds a point that we collided on the hole with
	* @param position the position of the collision
	*/	 
	void addPoint(Pose2D position);

	/**
	* Have we found enough points in order to determine if this is a square
	* @return whether this hole is known
	*/
	bool IsFoundSquare();

	
	Pose2D getOneSquareCorner();
	Pose2D getSecondSquareCorner();
	bool isInsideSquare(Pose2D position);
};

#endif // HOLE_H
