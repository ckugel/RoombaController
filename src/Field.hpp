//
// Created by caleb on 10/25/2024
//

#ifndef FIELD_H
#define FIELD_H

#include "Pillar.hpp"
#include "vector"
#include <cstdint> 
#include <memory>

#define FIELD_LENGTH 0
#define FIELD_WIDTH  0

#include "HoleManager.hpp"
#include "Pose2D.hpp"
#include "Pillar.hpp"

#define MAX_X 426.72
#define MAX_Y 242.57

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
    
  public:
    Field(const std::vector<Pillar>& pillars, const Pose2D& desiredDestination, const Pillar& botPose);
    Field(const std::vector<Pillar>& pillars, const Pose2D& desiredDestination);
    Field();

    void addEdgeMeasurement(double rawPosition, Cardinality cardinality);

    HoleManager& getManager();

    void applyOffsetToEdge(double x, double y);

    void clearField();

    Pillar getBotPose();

    static bool outOfBounds(const Pose2D& location);

    void addPillar(Pillar& newPillar);
    void updateBotPose(const Pose2D& updatedPosition);
    std::unique_ptr<std::vector<Pillar>> getPillars();
    void setPillars(std::unique_ptr<std::vector<Pillar>> pillars);
    std::vector<Pillar> getCopyPillars();

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
