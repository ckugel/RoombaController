/**
* Created by Caleb Kugel on 11/15/2024
*/

#include <iostream>
#include "Hole.hpp"


Hole::Hole(const Pose2D& positionOne, const Pose2D& positionTwo, double holeSize) {
    Pose2D o1 = positionOne.clone();
    Pose2D o2 = positionTwo.clone();
    threshold = o1.distanceTo(o2) / sqrt(2);
    this->holeSize = holeSize;
    this->registerPointsToHole(o1, o2);
    this->points = std::make_unique<std::vector<Pose2D>>();
    this->pointHoles = std::make_unique<std::vector<Hole>>();
}

Hole::Hole(const Pose2D& positionOne, const Pose2D& positionTwo, bool foundHole, const std::vector<Pose2D>& points, double holeSize) {
    threshold = positionOne.distanceTo(positionTwo) / sqrt(2);
    if (foundHole) {
        this->registerPointsToHole(positionOne, positionTwo);
    }
    else {
        this->foundHole = false;
    }

    this->points = std::make_unique<std::vector<Pose2D>>();
    for (uint16_t i = 0; i < points.size(); i++) {
        this->points->push_back(points[i]);
    }
    this->pointHoles = std::make_unique<std::vector<Hole>>();
}

Pose2D Hole::getOneSquareCorner() {
    return this->cornerOne;
}

Pose2D Hole::getSecondSquareCorner() {
    return this->cornerTwo;
}

Pose2D Hole::copyDoOperation(const Pose2D& position) const {
    Pose2D center(this->cornerOne);
    center.setHeading(center.angleTo(this->cornerTwo));
    center.translateByMagnitude(center.distanceTo(this->cornerTwo) / 2);

    Pose2D pos(position);
    pos.minus(center);

    pos.rotateByAngle(phi);
    pos.plus(Pose2D(x_translation_two, y_translation_two));
    return pos;
}

std::vector<Pose2D> Hole::getSuggestedNodePlacements() {
    // the nodes that we will generate will be around the hole, One hole length * 1.5 45 degrees offset to each corner from the center of the hole
    std::vector<Pose2D> toReturn;

    if (this->foundHole) {
	Pose2D center(this->cornerOne);
	center.setHeading(center.angleTo(this->cornerTwo));
	Pose2D temp(center);
	double magnitude = HOLE_SIZE * 1.5;
        for (uint8_t i = 0; i < 8; i++) {
            Pose2D toAdd(temp);
            toAdd.translateByMagnitude(magnitude);
            toReturn.push_back(toAdd);
            temp.addAngle(M_PI / 4);
        }
    }
    else {
        for (uint16_t i = 0; i < this->points->size(); i++) {
	    Pose2D suggested(this->points->at(i));
	    suggested.addAngle(M_PI);
	    suggested.translateByMagnitude(BOT_RADIUS);
	    toReturn.push_back(suggested);
        }
    }
    return toReturn;
    }

    bool Hole::isInSquare(Pose2D& position) const {
    // std::cout << "starting position: " << position << std::endl;
    // general idea: we use the operations in the object to translate objects for checks

    Pose2D pos = copyDoOperation(position);

   // std::cout << "translated position: " << pos << std::endl;

    if (pos.getX() > 0 && pos.getX() < threshold && pos.getY() > 0 && pos.getY() < threshold) {
        return true;
    }
    if (pos.getX() - BOT_RADIUS > 0 && pos.getX() - BOT_RADIUS < threshold && pos.getY() > 0 && pos.getY() < threshold) {
        return true;
    }
    if (pos.getX() + BOT_RADIUS > 0 && pos.getX() + BOT_RADIUS < threshold && pos.getY() > 0 && pos.getY() < threshold) {
        return true;
    }
    if (pos.getX() > 0 && pos.getX() < threshold && pos.getY() + BOT_RADIUS > 0 && pos.getY() + BOT_RADIUS < threshold) {
        return true;
    }
    if (pos.getX() > 0 && pos.getX() < threshold && pos.getY() - BOT_RADIUS > 0 && pos.getY() - BOT_RADIUS < threshold) {
        return true;
    }
    return false;

}

Hole::Hole(double X1, double Y1, double X2, double Y2, double holeSize) {
    Pose2D o1(X1, Y1);
    Pose2D o2(X2, Y2);
    this->threshold = o1.distanceTo(o2) / sqrt(2);
    this->holeSize = holeSize;
    registerPointsToHole(o1, o2);

    this->points = std::make_unique<std::vector<Pose2D>>();
    this->pointHoles = std::make_unique<std::vector<Hole>>();
}

Hole::Hole() {
    this->holeSize = HOLE_SIZE;
    this->threshold = HOLE_SIZE;
    this->foundHole = false;
    this->points = std::make_unique<std::vector<Pose2D>>();
    this->pointHoles = std::make_unique<std::vector<Hole>>();
}

Hole::Hole(const Pose2D& initialPoint, double holeSize) {
    this->foundHole = false;
    this->points = std::make_unique<std::vector<Pose2D>>();
    this->points->push_back(initialPoint);
    this->pointHoles = std::make_unique<std::vector<Hole>>();
    this->holeSize = holeSize;
    this->threshold = holeSize;
}

bool Hole::pointCouldBeMemberOfHole(const Pose2D& measurment) {
    for (const auto& point : *this->points) {
        if (point.distanceTo(measurment) > holeSize * std::sqrt(2)) {
            return false;
        }
    }
    return true;
}

Hole::Hole(const Hole& hole) {
    this->cornerOne = hole.cornerOne;
    this->cornerTwo = hole.cornerTwo;
    this->foundHole = hole.foundHole;
    this->phi = hole.phi;
    this->threshold = hole.threshold;
    this->x_translation_one = hole.x_translation_one;
    this->y_translation_one = hole.y_translation_one;
    this->x_translation_two = hole.x_translation_two;
    this->y_translation_two = hole.y_translation_two;
    this->holeSize = hole.holeSize;

    this->points = std::make_unique<std::vector<Pose2D>>();
    for (uint16_t i = 0; i < hole.points->size(); i++) {
        this->points->push_back(hole.points->data()[i]);
    }
    this->pointHoles = std::make_unique<std::vector<Hole>>();
    for (uint16_t i = 0; i < hole.pointHoles->size(); i++) {
        this->pointHoles->push_back(hole.pointHoles->data()[i]);
    }

}

void Hole::addPoint(const Pose2D& position) {
    this->points->push_back(position);
    //General flow:
    if (this->points->size() > 2) {
        // should be 3 elements
        // pick 2 with close headings

        // find index of the pivot

        Pose2D perpendicularPosition;
        Pose2D parallelOne;
        Pose2D parallelTwo;

        // positions one and two
        if (fabs(this->points->at(0).getHeading() - this->points->at(1).getHeading()) < M_PI / 2) {
            // yay we have our parallel par of one and two
            perpendicularPosition = this->points->at(2);
            parallelOne = this->points->at(0);
            parallelTwo = this->points->at(1);
        }
        // check positions two and three
        else if (fabs(this->points->at(1).getHeading() - this->points->at(2).getHeading()) < M_PI / 2) {
            // yay we know our parallel pair are two and three
            perpendicularPosition = this->points->at(0);
            parallelOne = this->points->at(1);
            parallelTwo = this->points->at(2);
        }

        // check positions one and three
        else if (fabs(this->points->at(0).getHeading() - this->points->at(2).getHeading()) < M_PI / 2) {
            // we have our parallel pair of 1 and three
            perpendicularPosition = this->points->at(1);
            parallelOne = this->points->at(0);
            parallelTwo = this->points->at(2);
        }

        if (parallelOne.getX() > parallelTwo.getX()) {
            const Pose2D temp = parallelOne;
            parallelOne = parallelTwo;
            parallelTwo = temp;
        }

        // none of the angles are similiar so we know that we have three measurmenets from all different edges of the square
        else {
            // get the two similiar angles when turned around
            // pose 1 and 2 are out of phase
            if (fabs(this->points->at(0).getHeading() + this->points->at(1).getHeading()) < 0.1 || fabs(fabs(this->points->at(0).getHeading()) - fabs(this->points->at(1).getHeading())) < 0.1) {
                parallelOne = this->points->at(0);
                parallelOne.translateByMagnitude(holeSize);
                parallelTwo = this->points->at(1);
                perpendicularPosition = this->points->at(2);
            }
            // pose 2 and three are out of phase
            else if (fabs(this->points->at(2).getHeading() + this->points->at(1).getHeading()) < 0.1 || fabs(fabs(this->points->at(2).getHeading()) - fabs(this->points->at(1).getHeading())) < 0.1) {
                parallelOne = this->points->at(2);
                parallelOne.translateByMagnitude(holeSize);
                parallelTwo = this->points->at(1);
                perpendicularPosition = this->points->at(0);
            }
            // pose 3 and 1 are out of phase
            else if (fabs(this->points->at(0).getHeading() + this->points->at(2).getHeading()) < 0.1 || fabs(fabs(this->points->at(0).getHeading()) - fabs(this->points->at(2).getHeading())) < 0.1) {
                parallelOne = this->points->at(0);
                parallelOne.translateByMagnitude(holeSize);
                parallelTwo = this->points->at(2);
                perpendicularPosition = this->points->at(1);
            }
            else {
                // i'm not sure how we got here and I' m just gonna clear the entire list
                this->points->clear();
            }

        }

        Pose2D cornerOne;
        Pose2D oppositeCorner;

        // now we have to make the cirtical points from the two parallel lines and the perpendicular line
        double dx = (parallelTwo.getX() - parallelOne.getX());
        double dy = (parallelTwo.getY() - parallelOne.getY());
        if (fabs(dx) < 0.01) {
            // vertical line
            double xCornerOne = parallelOne.getX();
            double yCornerOne = perpendicularPosition.getY();
            cornerOne = Pose2D(xCornerOne, yCornerOne);
            oppositeCorner = Pose2D(cornerOne);
            oppositeCorner.setHeading((cornerOne.angleTo(parallelOne) + cornerOne.angleTo(parallelTwo)) / 2);
            oppositeCorner.translateByMagnitude(holeSize * sqrt(2));
        }
        // horizontal line
        else if (fabs(dy) < 0.01) {
            double xCornerOne = perpendicularPosition.getX();
            double yCornerOne = parallelOne.getY();
            cornerOne = Pose2D(xCornerOne, yCornerOne);
            oppositeCorner = Pose2D(cornerOne);
            oppositeCorner.setHeading((parallelOne.getX() + perpendicularPosition.getX()) / 2);
            oppositeCorner.translateByMagnitude(holeSize * sqrt(2));
        }
        else {
            double m1 = (parallelTwo.getY() - parallelOne.getY()) / dx;
            double b1 = - m1 * parallelOne.getX() + parallelOne.getY();
            double m2 = - 1 / m1;
            double b2 = -m2 * perpendicularPosition.getX() + perpendicularPosition.getY();

            double xIntersection = (1 / m1 + m1) / (b2 - b1);
            double yIntersection = xIntersection * m1 + b1;
            cornerOne = Pose2D(xIntersection, yIntersection);

            // favorite very dumb method
            double angleToOther = (cornerOne.angleTo(parallelOne) + cornerOne.angleTo(perpendicularPosition)) / 2;
            oppositeCorner = Pose2D(cornerOne);
            oppositeCorner.setHeading(angleToOther);
            oppositeCorner.translateByMagnitude(holeSize * sqrt(2));
        }

        this->cornerOne = cornerOne;
        this->cornerTwo = oppositeCorner;
        this->foundHole = true;
    }

    if (!foundHole) {
        Pose2D cornerOneNew(position);
        cornerOneNew.rotateByAngle(M_PI / 4);
        cornerOneNew.translateByMagnitude(2);
        Pose2D cornerTwoNew(position);
        // an astute reader will notice that this is really stupid and that there is a relatively simple
        // math to accomplish this goal in one operation
        // however I am stupid
        cornerTwoNew.rotateByAngle(-M_PI / 4);
        cornerTwoNew.translateByMagnitude(2);
        cornerTwoNew.rotateByAngle(M_PI / 2);
        cornerTwoNew.translateByMagnitude(4);
        Hole hole(cornerOneNew, cornerTwoNew, 4);
        this->pointHoles->push_back(hole);
    }
}

void Hole::registerPointsToHole(const Pose2D& positionOne, const Pose2D& positionTwo) {
    // calculate the first translation
    cornerOne = Pose2D(positionOne);
    cornerTwo = Pose2D(positionTwo);
    Pose2D D(positionOne);
    foundHole = true;

    // wrong should be center x and y
    Pose2D center(positionOne);
    center.vecAdd(center.angleTo(positionTwo), center.distanceTo(positionTwo) / 2);
    x_translation_one = -center.getX();
    y_translation_one = -center.getY();

    double phi;
    D.plus(Pose2D(x_translation_one, y_translation_one));
    switch (D.getQuadrant()) {
        case 0:
            if (fabs(D.getX()) < 0.01) {
                // on y axis
                if (D.getY() < 0) {
                    phi = - M_PI / 3; // just a nudge
                }
                else {
                    phi = M_PI / 4 * 3;
                }
            }
            else {
                // on x axis
                if (D.getX() > 0) {
                    phi = -M_PI / 4 * 3;
                }
                else {
                    phi = M_PI / 3;
                }
            }
            break;
        case 1:
            phi = M_PI;
        break;
        case 2:
            phi = M_PI / 2;
        break;
        case 4:
            phi = -M_PI / 2;
        break;
        default:
            phi = 0;
        break;
    }

    D.rotateByAngle(phi);

    double newAngle = M_PI / 4 - atan2(-D.getY(), -D.getX()); // see desmos graph
    D.rotateByAngle(newAngle);
    phi = newAngle;
    this->phi = phi;
    // calculate position one into
    x_translation_two = -D.getX();
    y_translation_two = -D.getY();

    threshold = cornerOne.distanceTo(cornerTwo) / sqrt(2);

    /*std::cout << "x translation: " << x_translation_one << std::endl << "y translation: " << y_translation_one << std::endl
    << "angleTurn: " << phi << std::endl << "x translation 2: " << x_translation_two << std::endl << "y_translation: " << y_translation_two << std::endl
    << "threshold: " << threshold << std::endl;*/
}

std::ostream &operator<<(std::ostream &os, const Hole &hole) {
    os << "cornerOne: " << hole.cornerOne << " cornerTwo: " << hole.cornerTwo << " threshold: " << hole.threshold << " phi: " << hole.phi
    << " x_translation_one: " << hole.x_translation_one << " y_translation_one: " << hole.y_translation_one
       << " x_translation_two: " << hole.x_translation_two << " y_translation_two: " << hole.y_translation_two
       << " foundHole: " << hole.foundHole << " points: " << hole.points;
    return os;
}

void Hole::offset(const Pose2D& offset) {
    if (foundHole) {
        this->cornerOne.plus(offset);
        this->cornerTwo.plus(offset);
        registerPointsToHole(cornerOne, cornerTwo);
    }
    for (uint16_t i = 0; i < this->points->size(); i++) {
        this->points->at(i).plus(offset);
        this->pointHoles->at(i).offset(offset);
    }
}

bool Hole::lineIntersectsHole(const Pose2D& posOne, const Pose2D& posTwo) const {
    if (foundHole) {
        Pose2D positionOne;
        Pose2D positionTwo;
        if (posOne.getX() < posTwo.getX()) {
            positionOne = copyDoOperation(posOne);
            positionTwo = copyDoOperation(posTwo);
        } else {
            positionOne = copyDoOperation(posTwo);
            positionTwo = copyDoOperation(posTwo);
        }

        // if the line between them, at any point falls between 0 and threshold
        // annoyingly I think this means that we need to do real math (ew)
        if (fabs(positionOne.getX() - positionTwo.getX()) < 0.05) {
            // we have a vertical line
            // if the vertical line starts below 0 and ends after threshold and X is between 0 and threshold then it intersects
            return (((positionOne.getX() > 0 && positionOne.getX() < threshold) ||
                     (positionTwo.getX() > 0 && positionTwo.getX() < threshold)) &&
                    fmax(positionOne.getY(), positionTwo.getY()) > threshold &&
                    fmin(positionOne.getY(), positionTwo.getY()) < 0);

        }
        if (fabs(positionOne.getY() - positionTwo.getY()) < 0.05) {
            // we have a horizontal line
            return (((positionOne.getY() > 0 && positionOne.getY() < threshold) ||
                     (positionTwo.getY() > 0 && positionTwo.getY() < threshold)) &&
                    fmax(positionOne.getX(), positionTwo.getX()) > threshold &&
                    fmin(positionOne.getX(), positionTwo.getX()) < 0);
        }

        // now we gotta make a line, which we can guranteed will exist without a slope of 0
        double m = (positionTwo.getY() - positionOne.getY()) / (positionTwo.getX() - positionOne.getX());
        double b = positionOne.getY() - (positionOne.getX() * m);

        // now that we know the equation of the line we can determine if it intersects the x axis, y axis, or y = threshold or x = threshold
        // yayyyy finding 0's of a function
        double y2Check = m * positionOne.getX() + b;
        if ((b < threshold && b > 0) || (y2Check < threshold) && (y2Check > 0)) {
            return true; // we intersect with the x axis
        }
        double xValueAtYIsZero = (-b / m);
        double xValueATYIsThreshold = (threshold - b) / m;
        return (xValueAtYIsZero > 0 || xValueAtYIsZero < threshold) ||
               (xValueATYIsThreshold > 0 || xValueATYIsThreshold < threshold);
    }
    else {
        for (uint16_t i = 0; i < this->pointHoles->size(); i++) {
            if (this->pointHoles->at(i).lineIntersectsHole(posOne, posTwo)) {
                return true;
            }
        }
    }
    return false;
}

Hole::Hole(double x1, double y1, double x2, double y2) {
    this->cornerOne = Pose2D(x1, y1);
    this->cornerTwo = Pose2D(x2, y2);
    this->points = std::make_unique<std::vector<Pose2D>>();
    this->holeSize = cornerOne.distanceTo(cornerTwo) / sqrt(2);
    registerPointsToHole(cornerOne, cornerTwo);
}
