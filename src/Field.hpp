//
// Created by caleb on 10/25/2024
//

#ifndef FIELD_H
#define FIELD_H

#include "Pillar.hpp"
#include "vector"
#include <cstdint> 
#include <memory>

#include "Graph.hpp"
#include "HoleManager.hpp"
#include "Pose2D.hpp"

/**
 * THe max x position on the field
 */
#define MAX_X 426.72

/**
 * The max y position on the field
 */
#define MAX_Y 242.57

/**
 * Norht, South, East, West
 */
enum Cardinality {
    N = 'N',
    S = 'S',
    E = 'E',
    W = 'W',
};

/**
 * A class that represents the objects that can be and are on the field
 */
class Field {
  protected:
 /**
  * the pillars on the field.
  * The physical 3D cylinders get stored here
  */
 std::unique_ptr<std::vector<Pillar>> pillars;

 /**
  * A hole maanger to manage and check collision with holes
  */
 HoleManager holeManager;

 /**
  * The amount to offset everything by after reading edges.
  * X value stores the X (N, S) offset, Y value stores the Y (E, W) offset
  */
 Pose2D offset;

 /**
  * the desired destination for the robot to get to
  */
 Pose2D desiredDestination;

 /**
  * THe position of the robot on the field
  */
 Pillar botPose;

 /**
  * the running error that we have accumulated over time in our measurements.
  * This is just an approixmation based on read in error from the IPC
  */
 Pose2D runningError;

 /**
  * The graph object which is used to generate trajectorys from nodes.
  * Nodes represent places we can be on the field.
  * The connections between nodes represents the possible paths we can take.
  * The values of the connections represent the cost of moving between those nodes
  * which is measured as the distance between nodes.
  */
 Graph<Pose2D> graph;

 /**
  * The index of the desired position in the graph
  */
 int32_t desiredIndex = -1;

 /**
  * A queue for new pillars that we add to the field.
  * This is used to speed up discretization and weighting of the graph.
  */
 std::vector<Pillar> newPillars;
    
  public:
    /**
     * constructs a new field object
     * @param pillars the pillars that the field stores
     * @param desiredDestination the desired destination for the app to generate a path to
     * @param botPose 
     */
    Field(const std::vector<Pillar>& pillars, const Pose2D& desiredDestination, const Pillar& botPose);

    /**
     * constructs a new field object
     * @param pillars the pillars that the field stores
     * @param desiredDestination the desired destination for the app to generate a path to
     */
    Field(const std::vector<Pillar>& pillars, const Pose2D& desiredDestination);

    /**
     * Constructs a deafult field object
     */
    Field();

    /**
     * Copys the majority of the important parts of the field
     * @param pillars the pillars to store
     * @param desired_destination the desired destination to travel to
     * @param bot_pose the bot position
     * @param graph the graph object
     */
    Field(const std::unique_ptr<std::vector<Pillar>> &pillars, const Pose2D &desired_destination,
          const Pillar &bot_pose, const Graph<Pose2D> &graph);

 /**
  * Add an edge measurement to the field.
  * @param rawPosition the raw position (X or Y) of the measurmenet
  * @param cardinality the direction of the measurement
  */
 void addEdgeMeasurement(double rawPosition, Cardinality cardinality);

 /**
  * Gets a reference to the hole manager
  * @return a reference to the hole manager
  */
 HoleManager& getManager();

 /**
  * Applys some offset from getting the edge of the field
  * @param x component of the offset
  * @param y component of the offset
  */
 void applyOffsetToEdge(double x, double y);

 /**
  * @return the index of the desired pillar in the graph
  */
 [[nodiscard]] int32_t getDesiredIndex() const;

 /**
  * Clears the entire field and cooresponding managers and graphs
  */
 void clearField();

 /**
  * Gets a reference to the graph object on the field
  * @return a reference to the graph object
  */
 Graph<Pose2D>& getGraph();

 /**
  * @return the robot pose on the field as a pillar. The radius is the radius of the bot
  */
 Pillar getBotPose();

 /**
  * A static check to determine if a given location in the field coordinate system is out of bounds
  * @param location the position to check
  * @return whether that postiion is out of bounds on the field.
  */
 static bool outOfBounds(const Pose2D& location);

    /**
     * Util function that returns whether a given line intersects a circle
     */
    static bool lineIntersectsCircle(double cx, double cy, double r, double x1, double y1, double x2, double y2);

 /**
  * Util function that returns whether a given line intersects a circle
  */
 static bool lineIntersectsCircle(Pillar p1, const Pose2D& one, const Pose2D& two) {return lineIntersectsCircle(p1.getX(), p1.getY(), p1.getRadius() + BOT_RADIUS, one.getX(), one.getY(), two.getX(), two.getY()); }

 /**
  * add nodes to the graph that are valid places the bot can be at
  */
 void discretizeGraph();

 /**
  * Weight the graph in a way that the bot does not intersect and objects on the field
  */
 void weightGraph();

 /**
  * Generates a path from the graph using Dijkstra's algorithm
  * @return a path to the desired destination
  */
 std::vector<Pose2D> makePath();

 /**
  * Add a pillar measurement to the graph
  * @param newPillar to add
  */
 void addPillar(Pillar& newPillar);

    /**
     *
     * @param updatedPosition
     */
    void updateBotPose(const Pose2D& updatedPosition);

    /**
     * a unique pointer for the pillars on the field
     * @return the pillars on the field
     */
    std::unique_ptr<std::vector<Pillar>> getPillars();

    /**
     * sets the pillars on the field
     * @param pillars the pillars that the field stores
     */
    void setPillars(std::unique_ptr<std::vector<Pillar>> pillars);

    /**
     * @return a copy of the pillars on the field
     */
    std::vector<Pillar> getCopyPillars();

    /**
     * Check whether a given position is a valid place for a node in the graph with respect to "this" field
     * @param location the location of the node
     * @return whether it collides with anything
     */
    bool validLocationForNode(const Pose2D& location);

    /**
     * get the desired destination to travel to
     * @return the position to get to
     */
    Pose2D getDesiredDestination();

    /**
     * @param other the new desired position
     */
    void updateDesired(const Pose2D& other);

    /**
     * set the index of the desired position to travel to IN THE GRAPH
     * @param index the index IN the graph
     */
    void setDesiredIndex(int32_t index);

    /**
     * round a given radius to possible radii
     * @param radius the radius to round
     * @return the rounded radius
     */
    static double roundRadius(double radius) {
        const double possibleRadii[] = {5.1, 7.62, 10.16, 12.7};
        double closest = possibleRadii[0];
        double minDiff = std::abs(radius - possibleRadii[0]);

        for (double r : possibleRadii) {
            double diff = std::abs(radius - r);
            if (diff < minDiff) {
                minDiff = diff;
                closest = r;
            }
        }
        return closest;
    }

    };

#endif //FIELD_H
