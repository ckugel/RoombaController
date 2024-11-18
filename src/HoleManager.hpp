/**
* Created by Caleb Kugel 11/15/2024
*/

#ifndef HOLE_MANAGER_H
#define HOLE_MANAGER_H

#include "Hole.hpp"

#define sideLength 20.0 // FIND THIS

class HoleManager {
    private:
	std::unique_ptr<std::vector<Pose2D>> points;
	std::unique_ptr<std::vector<Hole>> holes;
    public:
	/**
	 * Creats a new Hole manager
	 */
	HoleManager();

	/**
	* Adds a point that we collided on the hole with
	* @param position the position of the collision
	*/	 
	void addPoint(Pose2D position);

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

};


#endif //HOLE_MANAGER_H
