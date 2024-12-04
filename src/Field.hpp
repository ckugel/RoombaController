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

    void addPillar(const Pillar& newPillar);
    void updateBotPose(const Pose2D& updatedPosition);
    std::unique_ptr<std::vector<Pillar>> getPillars();
    void setPillars(std::unique_ptr<std::vector<Pillar>> pillars);
    std::vector<Pillar> getCopyPillars();

};

#endif //FIELD_H
