/**
* Created by Caleb Kugel 11/21/2024
*/

#ifndef UTIL_H
#define UTIL_H

#include <cmath>


/*
 * calculates the orientation of the ordered triplet (p, q, r)
* @return
    * 0 -> p, q, r are collinear
    * 1 -> Clockwise
    * 2 -> Counterclockwise
*/
double orientation(double px2, double py, double qx, double qy, double rx, double ry);


/*
 * checks if point q lies on the line segment 'pr'
*/
bool onSegment(double px, double py, double qx, double qy, double rx, double ry);


/*
 * determines if line segments (p1, q1) and (p2, q2) intersect
*/
bool doIntersect(double p1x, double p1y, double q1x, double q1y, double p2x, double p2y, double q2x, double q2y);

/*
* function to check if the line intersects a given rectangle
* @param cx1 the x position of the first bound of the line
* @param cy1 the y position of the first bound of the line
* @param cx2 the x position of the second bound of the line
* @param cy2 the y position of the second bound of the line
* @param rx1 the x position of the first rectangular X coordinate
* @param ry1 the y position of the first rectangular Y coordinate
* @param rx2 the x position of the second rectangular X coordinate
* @param ry2 the y position of the second rectangular Y coordinate
* @param rx3 the x position of the third rectangular X coordinate
* @param ry3 the y position of the third rectangular Y coordinate
* @param rx4 the x position of the fourth rectangular X coordinate
* @param ry4 the y position of the fourth rectangular Y coordinate
* @return whether the line intersects the position
*/
bool lineIntersectsRectangle(double cx1, double cy1, double cx2, double cy2, double rx1, double ry1, double rx2, double ry2, double rx3, double ry3, double rx4, double ry4);



#endif // UTIL_H

