//
// Created by caleb on 10/25/2024
//

#include "Field.hpp"

Field::Field(const std::vector<Pillar>& pillars, const Pose2D& desiredDestination, const Pillar& botPose) {
  this->pillars = std::make_unique<std::vector<Pillar>>(pillars);
    this->desiredDestination = desiredDestination;
    this->holeManager = HoleManager();
    this->botPose = botPose;
}

Field::Field(const std::vector<Pillar>& pillars, const Pose2D& desiredDestination) {
    this->pillars = std::make_unique<std::vector<Pillar>>(pillars);
    this->holeManager = HoleManager();
    this->desiredDestination = desiredDestination;
    this->botPose = Pillar(0, 0, 0, BOT_RADIUS);
}

Field::Field() {
  this-> pillars = std::make_unique<std::vector<Pillar>>();
  this->desiredDestination = Pose2D(0, 0, 0);
  this->botPose = Pillar(0, 0, 0, BOT_RADIUS);
  this->holeManager = HoleManager();
}

Field::Field(const std::unique_ptr<std::vector<Pillar>> &pillars, const Pose2D &desired_destination,
    const Pillar &bot_pose, const Graph<Pose2D> &graph) {
}

void Field::discretizeGraph() {
    graph.addNode(new Node<Pose2D>(botPose.getPose()));
    // std::vector<Node<Pillar>*> nodes;
    for (Pillar & pillar : *pillars) {
        double magnitude = pillar.getRadius() + BOT_RADIUS;
        for (double i = 1.0; i < 5.0; i += 0.75) {
            for (uint16_t angle = 0; angle < 361; angle += 25) {
                const double radian = angle * M_PI / 180.0;
                Pose2D attemptAdd = Pose2D::fromPolar(magnitude * i, radian);

                attemptAdd.plus(pillar.getPose());

                if (validLocationForNode(attemptAdd)) {
                    // add to list
                    Node<Pose2D>* toAdd = new Node<Pose2D>(attemptAdd);

                    graph.addNode(toAdd);
                    // nodes = graph.getNodes();
                }
            }
        }
    }

    graph.addNode(new Node<Pose2D>(desiredDestination));
    desiredIndex = graph.getNodes().size() - 1;
}

void Field::weightGraph() {
    std::vector<Node<Pose2D>*> nodes = graph.getNodes();
    for (uint16_t nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++) {
        for (uint16_t nodeIndexTwo = 0; nodeIndexTwo < nodes.size(); nodeIndexTwo++) {
            if (nodeIndex != nodeIndexTwo) {
                Pose2D positionOne = nodes[nodeIndex]->getData();
                Pose2D positionTwo = nodes[nodeIndexTwo]->getData();
                double length = positionOne.distanceTo(positionTwo);
                double dy = (positionTwo.getY() - positionOne.getY());
                double dx = (positionTwo.getX() - positionTwo.getY());
                bool gotThrough = true;

                for (uint8_t pillarIndex = 0; pillarIndex < pillars->size(); pillarIndex++) {
                    if (lineIntersectsCircle(pillars->at(pillarIndex), positionOne, positionTwo)) {
                        // uh oh we hit the circle
                        gotThrough = false;
                    }
                }

                if (holeManager.lineIntersectsAnyHoleMeasurement(positionOne, positionTwo)) {
                    gotThrough = false;
                }

                if (gotThrough) {
                    // add a weight between nodes[nodeIndex] and nodes[nodeIndexTwo]
                    graph.addConnection(nodes[nodeIndex], nodes[nodeIndexTwo], length);
                    // print out attempt to add
                }
            }
        }
    }
}

std::vector<Pose2D> Field::makePath() {
    std::vector<Node<Pose2D>*> path = graph.Dijkstra(graph.getNodes()[0], graph.getNodes()[desiredIndex]);
    std::vector<Pose2D> toReturn;
    for (uint16_t i = 0; i < path.size(); i++) {
        toReturn.push_back(path[i]->getData());
    }
    return toReturn;
}


bool Field::validLocationForNode(const Pose2D& location) {
    if (outOfBounds(location)) {
        return false;
    }
	for (uint8_t i = 0; i < pillars->size(); i++) {
		if (pillars->at(i).getPose().distanceTo(location) < pillars->at(i).getRadius() + BOT_RADIUS) {
			return false;
		}
    }
    if (holeManager.nodeCollides(location)) {
		return false;
    }

    return true;
}


HoleManager& Field::getManager() {
    return holeManager;
}

void Field::clearField() {
    this->pillars->clear();
}

std::unique_ptr<std::vector<Pillar>> Field::getPillars() {
    return std::move(pillars); 
}

void Field::setDesiredIndex(int32_t index) {
    this->desiredIndex = index;
}


void Field::setPillars(std::unique_ptr<std::vector<Pillar>> pillars) {
    this->pillars = std::move(pillars);
} 

Pillar Field::getBotPose() {
    return botPose;
}

void Field::addPillar(Pillar& newPillar) {
    newPillar.setRadius(roundRadius(newPillar.getRadius()));
    this->pillars->push_back(newPillar);
}

void Field::updateBotPose(const Pose2D& updatedPosition) {
    this->botPose.setPosition(updatedPosition);
}

std::vector<Pillar> Field::getCopyPillars() {
    std::vector<Pillar> copy;
    for (const auto & i : *pillars) {
        copy.push_back(i);
    }
    return copy;
}

bool Field::outOfBounds(const Pose2D& location) {
    return (location.getX() < 0 || location.getY() < 0 || location.getX() > MAX_X || location.getY() > MAX_Y);
}

void Field::addEdgeMeasurement(const double rawPosition, const Cardinality cardinality) {
    switch (cardinality) {
        case N:
            {
                offset.setY(rawPosition);
            }
            break;
        case S:
            {
                offset.setY(-rawPosition);
            }
            break;
        case E:
            {
                offset.setX(rawPosition);
            }
            break;
        case W:
            {
                offset.setX(-rawPosition);
            }
            break;
    }
}

void Field::applyOffsetToEdge(double x, double y) {
    Pose2D offset = Pose2D(x, y);
    for (uint16_t i = 0; i < this->pillars->size(); i++) {
        this->pillars->at(i).getPose().plus(offset);
        this->holeManager.offsetAll(offset);
    }
}

bool Field::lineIntersectsCircle(double cx, double cy, double r, double x1, double y1, double x2, double y2) {
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

int32_t Field::getDesiredIndex() const {
    return desiredIndex;
}

Graph<Pose2D> &Field::getGraph() {
    return graph;
}
