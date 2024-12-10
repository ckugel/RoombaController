//
// Created by caleb on 11/9/21.
//

#ifndef ANOTHERGRAPH_GRAPH_H
#define ANOTHERGRAPH_GRAPH_H

#include "Node.hpp"
#include "Pose2D.hpp"
#include <stack>

#include "vector"
#include "map"


template <typename V>
class Graph {
    protected:
        typedef std::pair<double, std::vector<Node<V>*>> pair;
        struct CustomCompare {
            bool operator()(const pair& first, const pair& second) {
                return first.first > second.first;
            }
        };
        std::vector<Node<V>*> nodes;
        std::vector<std::vector<double>> matrix;
        Node<V>* head = nullptr;
        std::map<Node<V>*, unsigned int> nodeMap;


        /**
         * resize the adjacency matrix
         */
        void resize();

        /**
         * get the number of nodes we have visited
         * @param listOfBools list of bools to combine
         * @return the number of nodes visitied
         */
        int numVisited(std::vector<bool> listOfBools);

        /**
         * Returns true if the graph containes a node
         * @param node the node to determine if it's inside
         * @param listOfNodes the list of nodes to check against
         * @return whether node is in the list of nodes
         */
        bool contains(Node<V>* node, std::vector<Node<V>*> listOfNodes);

        //void printAdjacent(Node<V>* next);

    public:
        /**
         * Add a node to the graph
         * @param newNode a new node to add to the graph
         */
        void addNode(Node<V>* newNode);

	// void printOut();

        /**
         * Adds a node with adjacent nodes all with the same weight
         * @param nextNode the node to add
         * @param adjacentNodes the nodes to make a connect for
         * @param weight the weight for all the edges
         */
        void addNode(Node<V>* nextNode, std::vector<Node<V>*> adjacentNodes, double weight);

        /**
         * Add a node
         * @param nextNode
         * @param adjacentNodes
         */
        void addNode(Node<V> *nextNode, std::vector<Node<V>*> adjacentNodes);

        /**
         * Add a node with a connection
         * @param nextNode the node to add
         * @param nodeITSLATE a node to add a connection to
         */
        void addNode(Node<V> *nextNode, Node<V> *nodeITSLATE);

        /**
         * add a weight between nodes
         * @param one the first node
         * @param two the second node
         * @param weight the weight to put between them
         */
        void addConnection(Node<V> *one, Node<V> *two, double weight);

        /**
         * Add a connection between two nodes
         * @param one node one
         * @param two node two
         */
        void addConnection(Node<V> *one, Node<V> *two);

        /**
         * Add a node with a weight to the graph
         * @param nextNode the node to add
         * @param nodeITSLATE the node to connect to
         * @param weight the weight between those vertices
         */
        void addNode(Node<V> *nextNode, Node<V> *nodeITSLATE, double weight);

        /**
         * Set the head of the graph
         * @param index the index to become head
         */
        void setHead(int index);

        /**
         * Remove a node from the graph based on index
         * @param index the index to remove
         */
        void removeNode(size_t index);

        /**
         * Dijkstra's algorithm
         * @param from the node that you start at
         * @param find the node we want to get to
         * @return a path for the nodes
         */
        std::vector<Node<V>*> Dijkstra(Node<V>* from, Node<V>* find);

        /**
         * get the adjaceency matrix for the graph whihc contains the weigthings
         * @return the matrix for the weights
         */
        std::vector<std::vector<double>> getAdjacencyList();

        /**
         * Get the adjacent nodes of a node
         * @param next the node to get its' connected nodes
         * @return the adjacent nodes
         */
        std::vector<Node<V>*> getAdj(Node<V>* next);

        /**
         * @return All the nodes
         */
        std::vector<Node<V>*> getNodes();

	// void playGround();

        Graph();

        // std::vector<Node<V>*> FrugalKugel(Node<V>* find, unsigned int steps);

	~Graph();

};

template class Graph<Pose2D>;


#endif //ANOTHERGRAPH_GRAPH_H
