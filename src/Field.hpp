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

#define MAX_Y 425
#define MAX_X 250

enum Cardinality {
    N = 'N',
    S = 'S',
    E = 'E',
    W = 'W',
};

class Field {
  protected:
    std::unique_ptr<std::vector<Pillar>> pillars;
    HoleManager holeManager;
    Pose2D offset;
    Pose2D desiredDestination;
    Pillar botPose;
    Pose2D runningError;
    Graph<Pose2D> graph;
    int32_t desiredIndex = -1;
    std::vector<Pillar> newPillars;
    
  public:
    Field(const std::vector<Pillar>& pillars, const Pose2D& desiredDestination, const Pillar& botPose);
    Field(const std::vector<Pillar>& pillars, const Pose2D& desiredDestination);
    Field();

    Field(const std::unique_ptr<std::vector<Pillar>> &pillars, const Pose2D &desired_destination,
        const Pillar &bot_pose, const Graph<Pose2D> &graph);

    void addEdgeMeasurement(double rawPosition, Cardinality cardinality);

    HoleManager& getManager();

    void applyOffsetToEdge(double x, double y);

    [[nodiscard]] int32_t getDesiredIndex() const;

    void clearField();

    Graph<Pose2D>& getGraph();

    Pillar getBotPose();

    static bool outOfBounds(const Pose2D& location);

    /**
     * Util function that returns whether a given line intersects a circle
     */
    static bool lineIntersectsCircle(double cx, double cy, double r, double x1, double y1, double x2, double y2);

    static bool lineIntersectsCircle(Pillar p1, const Pose2D& one, const Pose2D& two) {return lineIntersectsCircle(p1.getX(), p1.getY(), p1.getRadius() + BOT_RADIUS, one.getX(), one.getY(), two.getX(), two.getY()); }

    void discretizeGraph();
    void weightGraph();
    std::vector<Pose2D> makePath();

    void addPillar(Pillar& newPillar);
    void updateBotPose(const Pose2D& updatedPosition);
    std::unique_ptr<std::vector<Pillar>> getPillars();
    void setPillars(std::unique_ptr<std::vector<Pillar>> pillars);
    std::vector<Pillar> getCopyPillars();

    bool validLocationForNode(const Pose2D& location);

    Pose2D getDesiredDestination();

    /**
     * 
     * @param other
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
