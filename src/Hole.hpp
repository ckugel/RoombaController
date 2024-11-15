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
	std::unique_ptr<std::vector<Pose2D>> points; 
    public:
	Hole();
	~Hole();
};

#endif // HOLE_H
