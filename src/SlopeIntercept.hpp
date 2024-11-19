/**
* A slope intercept object is basically a line.
*
* @Author ckugel
*/

#ifndef SLOPE_INTERCEPT_H
#define SLOPE_INTERCEPT_H

#include "Pose2D.hpp"

class SlopeIntercept {
    public: 
	SlopeIntercept();
	SlopeIntercept(Pose2D one, Pose2D two);
	SlopeIntercept(double X1, double Y1, double X2, double Y2);
    private:
	// from y = mx + b
	double m, b;
};

#endif // SLOPE_INTERCEPT_H

