/**
* Made by Caleb Kugel on 11/15/2024
*/

#ifndef HOLE_H
#define HOLE_H

#include "vector"
#include <memory>
#include "Pose2D.hpp"

#define HOLE_SIZE 20; // TODO: find

class Hole {
    private:
	Pose2D cornerOne;
	Pose2D cornerTwo;
    public:
    
	/**
	 * Make a new hole with the given coordinates
	 * @param X1 one corner's X position
	 * @param Y1 one corner's Y position
	 * @param X2 the other corner's X position
	 * @parram Y2 the other corner's Y position
	 */
	Hole(double X1, double Y1, double X2, double Y2);	

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
