/**
* Made by Caleb Kugel on 11/15/2024
*/

#ifndef HOLE_H
#define HOLE_H

#include "vector"
#include <memory>
#include "Pose2D.hpp"

#define HOLE_SIZE 20 // TODO: find

/**
 * A Hole is either a complete square where we know the critical points (two vertices of the square).
 * A Hole could also represent a bunch of points that are all within Square Length * Root(2) of eachother
 */
class Hole {
    private:
	Pose2D cornerOne;
	Pose2D cornerTwo;

	bool foundHole;

	std::unique_ptr<std::vector<Pose2D>> points;
    public:
    
	/**
	 * Make a new hole with the given coordinates
	 * @param X1 one corner's X position
	 * @param Y1 one corner's Y position
	 * @param X2 the other corner's X position
	 * @param Y2 the other corner's Y position
	 */
	Hole(double X1, double Y1, double X2, double Y2);	

	/**
	 * Copy  constructor for hole
	 */
	Hole(Pose2D positionOne, Pose2D positionTwo, bool foundHole, std::vector<Pose2D> points);

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
	 * returns whether a new point could be within the range of this hole.
	 * @param measurment the position of the point that may or may not be close to this hole
	 */
	bool pointCouldBeMemberOfHole(Pose2D measurment);

	/**
	 * The deafult constructor for a hole.
	 */
	Hole();

	/**
	 * Copy constructor
	 */
	Hole(const Hole& hole);

	/**
	 * Makes a new hole and stores the initial measurment
	 * @param initialPoint our initial measurment
	 */
	Hole(Pose2D initialPoint);

	/**
	 * Add a point to this hole/chunk. 
	 * Morphs this object into a found hole if possible
	 * @param position the position of the measurment
	 */
	void addPoint(Pose2D position);

	/**
	* Whether the passed in position is in the square
	* @param position the position to check
	* @return whether that position is in the square
	*/
	bool isInSquare(Pose2D position);
};

#endif // HOLE_H
