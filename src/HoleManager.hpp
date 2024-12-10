/**
* Created by Caleb Kugel 11/15/2024
*/

#ifndef HOLE_MANAGER_H
#define HOLE_MANAGER_H

#include "Hole.hpp"
#include "util.hpp"

#define sideLength 0.6096 // 2 by 2 squares in feet. meters
#define MEASUREMENT_WIDTH 0.05 // meters

/**
 * A hole manager handles holes that and the measurments that are associated with them.
 * Wrapped class for lists of holes
 */
class HoleManager {
    private:
	/**
	 * The holes that we have found. Calculated.
	 */
	std::unique_ptr<std::vector<Hole>> holes;

	/**
	 * The raw hole measurements that we have read in
	 */
	std::unique_ptr<std::vector<Pose2D>> holeMeasurements;
    public:
		/**
		 * Creats a new Hole manager
		 */
		HoleManager();

		/**
		* Adds a point that we collided on the hole with
		* @param position the position of the collision
		*/
		void addPoint(const Pose2D& position);

		/**
		 * get a hole from registered holes
		 * @param index of the hole in the holes list
		 * @return the hole that has been registered
		 */
		Hole getHole(uint8_t index);

		/**
		 * Adds a hole to the holes
		 * @param hole to add
		 */
		void addHole(const Hole& hole);

		/**
		 * Adds a hole to the hole manager
		 * @param cornerOne the first Corner
		 * @param cornerTwo the second Corner
		 */
		void addHole(const Pose2D& cornerOne, const Pose2D& cornerTwo);

		/**
		* get the holes that we for sure know about
		*/
		std::vector<Hole> getHoles();

		/**
		 * get the points along unconfirmed holes
		 */
		std::vector<Pose2D> getPointsOnHoles();

		/**
		* returns a list of likely accessible nodes
		*/
		std::vector<Pose2D> getSuggestedNodePlacements();

		/**
		* Whether a node collides with the estimated hole or not
		* @param position the position of the node
		* @return whether that node is valid from a hole perspective
		*/
		bool nodeCollides(Pose2D position);

		/**
		* Determines whether a line is valid for all the hole measurements
		*/
		bool lineIntersectsAnyHoleMeasurement(const Pose2D& positionOne, const Pose2D& positionTwo);

		/**
		 * provides the offset to all holes and their values
		 */
		void offsetAll(const Pose2D& offset) const;

};


#endif //HOLE_MANAGER_H
