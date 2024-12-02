/**
* Created by Caleb Kugel on 11/15/2024
*/

#include <iostream>
#include "Hole.hpp"


Hole::Hole(const Pose2D& positionOne, const Pose2D& positionTwo) {
    Pose2D o1 = positionOne.clone();
    Pose2D o2 = positionTwo.clone();
    this->registerPointsToHole(o1, o2);
    this->points = std::make_unique<std::vector<Pose2D>>();
}

Hole::Hole(const Pose2D& positionOne, const Pose2D& positionTwo, bool foundHole, const std::vector<Pose2D>& points) {
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
}

Pose2D Hole::getOneSquareCorner() {
    return this->cornerOne;
}

Pose2D Hole::getSecondSquareCorner() {
    return this->cornerTwo;
}

Pose2D Hole::doOperationCopy(const Pose2D& pose) const {
    double x = (pose.getX() + x_translation_one) * cos_phi - (y_translation_one + pose.getY()) * sin_phi + x_translation_two;
    double y = (pose.getX() + x_translation_one) * sin_phi + (y_translation_one + pose.getY()) * cos_phi + y_translation_two;
    return {x, y};
}

bool Hole::isInSquare(Pose2D& position) const {
    // general idea: we use the operations in the object to translate objects for checks
    Pose2D pos = doOperationCopy(position);

    std::cout << *this;


    if (pos.getX() > 0 && pos.getX() < correctedCornerTwo.getX() && pos.getY() > 0 && pos.getY() < correctedCornerTwo.getY()) {
        return true;
    }
    if (pos.getX() - BOT_RADIUS > 0 && pos.getX() - BOT_RADIUS < correctedCornerTwo.getX() && pos.getY() > 0 && pos.getY() < correctedCornerTwo.getY()) {
        return true;
    }
    if (pos.getX() + BOT_RADIUS > 0 && pos.getX() + BOT_RADIUS < correctedCornerTwo.getX() && pos.getY() > 0 && pos.getY() < correctedCornerTwo.getY()) {
        return true;
    }
    if (pos.getX() > 0 && pos.getX() < correctedCornerTwo.getX() && pos.getY() + BOT_RADIUS > 0 && pos.getY() + BOT_RADIUS < correctedCornerTwo.getY()) {
        return true;
    }
    if (pos.getX() > 0 && pos.getX() < correctedCornerTwo.getX() && pos.getY() - BOT_RADIUS > 0 && pos.getY() - BOT_RADIUS < correctedCornerTwo.getY()) {
        return true;
    }
    return false;


    /*
    // heres the idea: we generate the cour points. Translate the four points such that the center is around the origin
    // translate the position by the same offset
    Pose2D cornerOne(this->cornerOne);
    Pose2D cornerTwo(this->cornerTwo);

    // calculate center of rectangle
    Pose2D centerOfRectangle(cornerOne);
    centerOfRectangle.setHeading(centerOfRectangle.angleTo(cornerTwo));
    double halfDistance = cornerOne.distanceTo(cornerTwo);
    centerOfRectangle.translateByMagnitude(halfDistance);
    // now calculate the translation for the corners
    double angleTo = centerOfRectangle.angleTo(Pose2D(0, 0));
    double amount = centerOfRectangle.distanceTo(Pose2D(0, 0));

    cornerOne.setHeading(angleTo);
    cornerTwo.setHeading(angleTo);
    position.setHeading(angleTo);
    cornerOne.translateByMagnitude(amount);
    cornerTwo.translateByMagnitude(amount);
    position.translateByMagnitude(amount);

    if (cornerTwo.getX() < 0) {
        if (cornerTwo.getY() < 0) {
            // - 90 degree rotation
            cornerOne.rotateByAngle(- M_PI / 2);
            cornerTwo.rotateByAngle(-M_PI / 2);
            position.rotateByAngle(-M_PI / 2);
        }
        else {
            // -180 degree rotation
            cornerOne.rotateByAngle(M_PI);
            cornerTwo.rotateByAngle(M_PI);
            position.rotateByAngle(M_PI);
        }
    }
    if (cornerOne.getX() < 0 && cornerOne.getY() >= 0) {
        // user cornerOne
        cornerOne.rotateByAngle(M_PI / 2);
        cornerTwo.rotateByAngle(M_PI / 2);
        position.rotateByAngle(M_PI / 2);
    }

    Pose2D otherCorner(cornerOne);
    otherCorner.rotateByAngle(M_PI / 2); // really thinky
    amount = otherCorner.angleTo(otherCorner) / 2;

    cornerOne.rotateByAngle(amount);
    cornerTwo.rotateByAngle(amount);
    position.rotateByAngle(amount);
    angleTo = cornerOne.angleTo(Pose2D(0, 0));
    cornerOne.setHeading(angleTo);
    cornerTwo.setHeading(angleTo);
    position.setHeading(angleTo);
    cornerOne.translateByMagnitude(halfDistance);
    cornerTwo.translateByMagnitude(halfDistance);
    position.translateByMagnitude(halfDistance);


    // now we have cornerOne and cornerTwo forming a square with the bottom left on the origin
    if ((position.getX() + BOT_RADIUS > 0 && position.getX() + BOT_RADIUS < cornerTwo.getX()) || (position.getX() - BOT_RADIUS > 0 && position.getX() - BOT_RADIUS < cornerTwo.getX()) || (position.getX() > 0 && position.getX() < cornerTwo.getX())) {
        // X coord has collision
        // now check Y below
        if ((position.getY() + BOT_RADIUS > 0 && position.getY() + BOT_RADIUS < cornerTwo.getY()) || (position.getY() - BOT_RADIUS > 0 && position.getY() - BOT_RADIUS < cornerTwo.getY()) || (position.getY() > 0 && position.getY() < cornerTwo.getY())) {
            return true;
        }
    }
    return false;

    // perform a rotation on all the points such that the coordinate are in a stanard rectangle
    // translate the bottom left coordinate to the origin
    // translate all the points by that offset
    // check if the coordinate is in that posiiton
    // create four points aroung the new posiiton and determine if any of those are in the square

*/

    /*
    // Calculate the center of the square
    Pose2D center = {(cornerOne.getX() + cornerTwo.getX()) / 2, (cornerOne.getY() + cornerTwo.getY()) / 2};

    // Calculate the vector representing one side of the square
    const Pose2D sideVector = cornerTwo.subtractBy(cornerOne);
    // const double sideLength = cornerOne.distanceTo(cornerTwo) / std::sqrt(2);

    /*
    if (std::abs(sideLength - HOLE_SIZE) > 5) {
        std::cerr << "Error: Given corners do not form a square of the specified hole length." << std::endl;
        return false;
    }
    */

    /*
    Pose2D sideVectorNormalized = sideVector.normalize();
    Pose2D perpendicularVector = {-sideVectorNormalized.getY(), sideVectorNormalized.getX()};

    // Project the point onto the square's local axes
    Pose2D toPointVector = position.subtractBy(center);
    double projSide = toPointVector.dotProduct(sideVectorNormalized);
    double projPerpendicular = toPointVector.dotProduct(perpendicularVector);

    double halfSize = HOLE_SIZE / 2;

    // Check if the point is within the square
    if (fabs(projSide) <= halfSize && fabs(projPerpendicular) <= halfSize) {
        return true;
    }

    // Check if the point is within RADIUS of the square
    if (fabs(projSide) <= halfSize + BOT_RADIUS && fabs(projPerpendicular) <= halfSize + BOT_RADIUS) {
        Pose2D closestPoint = {
            center.getX() + projSide * sideVectorNormalized.getX() + projPerpendicular * perpendicularVector.getX(),
            center.getY() + projSide * sideVectorNormalized.getY() + projPerpendicular * perpendicularVector.getY()
        };
        return position.distanceTo(closestPoint) <= BOT_RADIUS;
    }

    return false;
    */
}

Hole::Hole(double X1, double Y1, double X2, double Y2) {
    Pose2D o1(X1, Y1);
    Pose2D o2(X2, Y2);
    registerPointsToHole(o1, o2);

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
    this->cos_phi = hole.cos_phi;
    this->sin_phi = hole.sin_phi;
    this->x_translation_one = hole.x_translation_one;
    this->y_translation_one = hole.y_translation_one;
    this->x_translation_two = hole.x_translation_two;
    this->y_translation_two = hole.y_translation_two;

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

void Hole::registerPointsToHole(const Pose2D& positionOne, const Pose2D& positionTwo) {
    // calculate the first translation
    cornerOne = Pose2D(positionOne);
    cornerTwo = Pose2D(positionTwo);
    foundHole = true;

    // wrong should be center x and y
    Pose2D center(positionOne);
    center.vecAdd(center.angleTo(positionTwo), center.distanceTo(positionTwo) / 2);
    x_translation_one = -center.getX();
    y_translation_one = -center.getY();
    center = Pose2D(cornerOne);

    double phi;
    center.plus(Pose2D(x_translation_one, y_translation_two));
    switch (center.getQuadrant()) {
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

    center.rotateByAngle(phi);

    Pose2D cpy(center);
    cpy.rotateByAngle(M_PI / 2); // rotate this object into quadrant one
    double newAngle = -center.angleTo(cpy) / 2;
    center.rotateByAngle(newAngle);
    phi += newAngle;
    cos_phi = cos(phi);
    sin_phi = sin(phi);
    // calculate position one into
    x_translation_two = -center.getX();
    y_translation_two = center.getY();

    correctedCornerTwo = doOperationCopy(cornerTwo);

    std::cout << "x translation: " << x_translation_one << std::endl << "y translation: " << y_translation_one << std::endl
    << "angleTurn: " << phi << std::endl << "x translation 2: " << x_translation_two << std::endl << "y_translation: " << y_translation_two << std::endl
    << "corrected corner: " << correctedCornerTwo.getX() << ", " << correctedCornerTwo.getY() << std::endl;
}

std::ostream &operator<<(std::ostream &os, const Hole &hole) {
    os << "cornerOne: " << hole.cornerOne << " cornerTwo: " << hole.cornerTwo << " correctedCornerTwo: "
       << hole.correctedCornerTwo << " cos_phi: " << hole.cos_phi << " sin_phi: " << hole.sin_phi
       << " x_translation_one: " << hole.x_translation_one << " y_translation_one: " << hole.y_translation_one
       << " x_translation_two: " << hole.x_translation_two << " y_translation_two: " << hole.y_translation_two
       << " foundHole: " << hole.foundHole << " points: " << hole.points;
    return os;
}

