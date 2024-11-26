/**
* Created by Caleb Kugel 11/21/2024
*/

#include "util.hpp"

double orientation(double px, double py, double qx, double qy, double rx, double ry) {
    double val = (qy - py) * (rx - qx) - (qx - px) * (ry - qy);
    if (fabs(val) < 0.1) return 0; // collinear
    return (val > 0) ? 1 : 2; // clockwise or counterclockwise
}

bool onSegment(double px, double py, double qx, double qy, double rx, double ry) {
    return qx <= (px > rx ? px : rx) && qx >= (px < rx ? px : rx) &&
           qy <= (py > ry ? py : ry) && qy >= (py < ry ? py : ry);
}

bool doIntersect(double p1x, double p1y, double q1x, double q1y, double p2x, double p2y, double q2x, double q2y) {
    double o1 = orientation(p1x, p1y, q1x, q1y, p2x, p2y);
    double o2 = orientation(p1x, p1y, q1x, q1y, q2x, q2y);
    double o3 = orientation(p2x, p2y, q2x, q2y, p1x, p1y);
    double o4 = orientation(p2x, p2y, q2x, q2y, q1x, q1y);

    // standard case
    if (o1 != o2 && o3 != o4) return true;

    // Edge cases
    // p1, q1, p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1x, p1y, p2x, p2y, q1x, q1y)) return true;

    // p1, q1, q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1x, p1y, q2x, q2y, q1x, q1y)) return true;

    // p2, q2, p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2x, p2y, p1x, p1y, q2x, q2y)) return true;

    // p2, q2, q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2x, p2y, q1x, q1y, q2x, q2y)) return true;

    return false; // No doubleersection
}


bool lineIntersectsCircle(double cx, double cy, double r, double x1, double y1, double x2, double y2) {
    // Calculate the line direction vector
    double dx = x2 - x1;
    double dy = y2 - y1;

    // Calculate the projection of the circle center onto the line
    double t = ((cx - x1) * dx + (cy - y1) * dy) / (dx * dx + dy * dy);

    // Find the closest point on the line to the circle center
    double closestX = x1 + t * dx;
    double closestY = y1 + t * dy;

    // Calculate the distance from the circle center to the closest point
    double distX = closestX - cx;
    double distY = closestY - cy;
    double distanceToLine = std::sqrt(distX * distX + distY * distY);

    // Check if the distance is less than or equal to the radius
    return distanceToLine <= r;
}

bool lineIntersectsRectangle(double cx1, double cy1, double cx2, double cy2, double rx1, double ry1, double rx2, double ry2, double rx3, double ry3, double rx4, double ry4) {
    // Check if the line intersects any of the rectangle's edges
    return doIntersect(cx1, cy1, cx2, cy2, rx1, ry1, rx2, ry2) || doIntersect(cx1, cy1, cx2, cy2, rx2, ry2, rx3, ry3) || doIntersect(cx1, cy1, cx2, cy2, rx3, ry3, rx4, ry4) || doIntersect(cx1, cy1, cx2, cy2, rx4, ry4, rx1, ry1);
}

