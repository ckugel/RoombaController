//
// Created by caleb on 11/9/21.
//

#ifndef ANOTHERGRAPH_NODE_H
#define ANOTHERGRAPH_NODE_H


#include "Pose2D.hpp"

/**
 * Represents a node in a graph.
 * Really just a wrapped for V
 * @param V The data that the node encapsulates
 */
template <typename V>
class Node {
    private:
    /**
     * The data we store
     */
    V data;

    public:
    /**
     * Constructs a node from given data
     * @param data to construct the node from
     */
    Node(V data);

    /**
     * @return the data stored in the node
     */
    V getData();

    /**
     * sets the data we store in the node
     * @param newData new data to replace current data with
     */
    void SetData(V newData);
};

#include "Node.cpp"

#endif //ANOTHERGRAPH_NODE_H
