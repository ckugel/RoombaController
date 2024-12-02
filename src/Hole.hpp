/**
* Made by Caleb Kugel on 11/15/2024
*/

#ifndef HOLE_H
#define HOLE_H

#include "vector"
#include <memory>
#include "Pose2D.hpp"
#include <cmath>
#include <ostream>

#define HOLE_SIZE 69.06 // centimeters

/**
 * A Hole is either a complete square where we know the critical points (two vertices of the square).
 * A Hole could also represent a bunch of points that are all within Square Length * Root(2) of eachother
 */

static const double halfHoleDiagonal = HOLE_SIZE * sqrt(2) / 2;
class Hole {
    private:
	Pose2D cornerOne;
	Pose2D cornerTwo;
    Pose2D correctedCornerTwo;
	double cos_phi;
	double sin_phi;
	double x_translation_one;
	double y_translation_one;
	double x_translation_two;
	double y_translation_two;

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
	Hole(const Pose2D& positionOne, const Pose2D& positionTwo, bool foundHole, const std::vector<Pose2D>& points);

	/**
	 * Creates a new Hole object
	 */
	Hole(const Pose2D& positionOne, const Pose2D& positionTwo);

	/**
	 *  Register the corners of a hole
	 * @param positionOne corner One
	 * @param positionTwo Corner Two
	 */
	void registerPointsToHole(const Pose2D& positionOne, const Pose2D& positionTwo);

    friend std::ostream &operator<<(std::ostream &os, const Hole &hole);

    /**
     * do the whole operation and copy it into a new object for object collision
     * @param pose the Pose2D to apply the big formula
     * @return the result of the operations
     */
	[[nodiscard]] Pose2D doOperationCopy(const Pose2D& pose) const;
    
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
	bool pointCouldBeMemberOfHole(const Pose2D& measurment);

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
	explicit Hole(const Pose2D& initialPoint);

	/**
	 * Add a point to this hole/chunk. 
	 * Morphs this object into a found hole if possible
	 * @param position the position of the measurment
	 */
	void addPoint(const Pose2D& position);

	/**
	* Whether the passed in position is in the square
	* @param position the position to check
	* @return whether that position is in the square
	*/
	bool isInSquare(Pose2D& position) const;
};

#endif // HOLE_H
