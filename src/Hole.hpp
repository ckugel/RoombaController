/**
* Made by Caleb Kugel on 11/15/2024
*/

#ifndef HOLE_H
#define HOLE_H

#include "vector"
#include <memory>
#include "Pose2D.hpp"

class Hole {
    private:
	Pose2D cornerOne;
	Pose2D cornerTwo;
    public:
    
	/**
	 * Creates a new Hole object
	 */
	Hole(Pose2D positionOne, Pose2D positionTwo);
    
	/**
	* @return one of the square's corners
	*/
	Pose2D getOneSquareCorner();

	/**
	* @return the second of the square's corners
	*/
	Pose2D getSecondSquareCorner();

	/**
	* Whether the passed in position is in the square
	* @param position the position to check
	* @return whether that position is in the square
	*/
	bool isInSquare(Pose2D position);
};

#endif // HOLE_H
