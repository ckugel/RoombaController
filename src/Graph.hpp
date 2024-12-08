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


        void resize();
        
        int numVisited(std::vector<bool> listOfBools);

        bool contains(Node<V>* node, std::vector<Node<V>*> listOfNodes);

        //void printAdjacent(Node<V>* next);

    public:

        void addNode(Node<V>* newNode);

	// void printOut();
	
	void addNode(Node<V>* nextNode, std::vector<Node<V>*> adjacentNodes, double weight);
	void addNode(Node<V> *nextNode, std::vector<Node<V>*> adjacentNodes);
	void addNode(Node<V> *nextNode, Node<V> *nodeITSLATE);

	void addConnection(Node<V> *one, Node<V> *two, double weight);
	void addConnection(Node<V> *one, Node<V> *two);
	void addNode(Node<V> *nextNode, Node<V> *nodeITSLATE, double weight);
	void setHead(int index);

    void removeNode(size_t index);

        std::vector<Node<V>*> Dijkstra(Node<V>* from, Node<V>* find);

	std::vector<std::vector<double>> getAdjacencyList();

        std::vector<Node<V>*> getAdj(Node<V>* next);

	std::vector<Node<V>*> getNodes();

	// void playGround();

        Graph();

        // std::vector<Node<V>*> FrugalKugel(Node<V>* find, unsigned int steps);

	~Graph();

};

template class Graph<Pose2D>;


#endif //ANOTHERGRAPH_GRAPH_H
