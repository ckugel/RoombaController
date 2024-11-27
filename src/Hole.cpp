/**
* Created by Caleb Kugel on 11/15/2024
*/

#include "Hole.hpp"


Hole::Hole(const Pose2D& positionOne, const Pose2D& positionTwo) {
    this->cornerOne = positionOne;
    this->cornerTwo = positionTwo;
    this->foundHole = false;
    this->points = std::make_unique<std::vector<Pose2D>>();
}

Hole::Hole(const Pose2D& positionOne, const Pose2D& positionTwo, bool foundHole, std::vector<Pose2D> points) {
    this->cornerOne = positionOne;
    this->cornerTwo = positionTwo;
    this->foundHole = foundHole;

    this->points = std::make_unique<std::vector<Pose2D>>();
    for (uint16_t i = 0; i < points.size(); i++) {
	this->points->push_back(points[i]);
    }
}

Pose2D Hole::getOneSquareCorner() {
    return this->cornerOne;
}

Pose2D Hole::getSecondSquareCorner() {
    return this->cornerTwo;
}

bool Hole::isInSquare(Pose2D& position) {
    //TODO: Implement
    return 1;
}

Hole::Hole(double X1, double Y1, double X2, double Y2) {
   this->cornerOne = Pose2D(X1, Y1);
    this->cornerTwo = Pose2D(X2, Y2);
    this->foundHole = false;

    this->points = std::make_unique<std::vector<Pose2D>>();
}

Hole::Hole() {
    this->foundHole = false;
    this->points = std::make_unique<std::vector<Pose2D>>();
}

Hole::Hole(const Pose2D& initialPoint) {
    this->foundHole = false;
    this->points = std::make_unique<std::vector<Pose2D>>();
    this->points->push_back(initialPoint);
}

bool Hole::pointCouldBeMemberOfHole(const Pose2D& measurment) {
    bool viable = true;
    this->points = std::make_unique<std::vector<Pose2D>>();
    for (uint16_t i = 0; i < this->points->size(); i++) {
	if (this->points->data()[i].distanceTo(measurment) > HOLE_SIZE * std::sqrt(2)) {
	    viable = false;
	}
    }
    return viable;
}

Hole::Hole(const Hole& hole) {
    this->cornerOne = hole.cornerOne;
    this->cornerTwo = hole.cornerTwo;
    this->foundHole = hole.foundHole;

    this->points = std::make_unique<std::vector<Pose2D>>();
    for (uint16_t i = 0; i < hole.points->size(); i++) {
	this->points->push_back(hole.points->data()[i]);

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
                parallelOne.translateByMagnitude(HOLE_SIZE);
                parallelTwo = this->points->at(1);
                perpendicularPosition = this->points->at(2);
            }
            // pose 2 and three are out of phase
            else if (fabs(this->points->at(2).getHeading() + this->points->at(1).getHeading()) < 0.1 || fabs(fabs(this->points->at(2).getHeading()) - fabs(this->points->at(1).getHeading())) < 0.1) {
                parallelOne = this->points->at(2);
                parallelOne.translateByMagnitude(HOLE_SIZE);
                parallelTwo = this->points->at(1);
                perpendicularPosition = this->points->at(0);
            }
            // pose 3 and 1 are out of phase
            else if (fabs(this->points->at(0).getHeading() + this->points->at(2).getHeading()) < 0.1 || fabs(fabs(this->points->at(0).getHeading()) - fabs(this->points->at(2).getHeading())) < 0.1) {
                parallelOne = this->points->at(0);
                parallelOne.translateByMagnitude(HOLE_SIZE);
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
            oppositeCorner.translateByMagnitude(HOLE_SIZE * sqrt(2));
        }
        // horizontal line
        else if (fabs(dy) < 0.01) {
            double xCornerOne = perpendicularPosition.getX();
            double yCornerOne = parallelOne.getY();
            cornerOne = Pose2D(xCornerOne, yCornerOne);
            oppositeCorner = Pose2D(cornerOne);
            oppositeCorner.setHeading((parallelOne.getX() + perpendicularPosition.getX()) / 2);
            oppositeCorner.translateByMagnitude(HOLE_SIZE * sqrt(2));
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
            oppositeCorner.translateByMagnitude(HOLE_SIZE * sqrt(2));
        }

        this->cornerOne = cornerOne;
        this->cornerTwo = oppositeCorner;
        this->foundHole = true;
    }
}

