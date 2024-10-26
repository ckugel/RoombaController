//
// Created by caleb on 11/9/21.
//

#ifndef ANOTHERGRAPH_GRAPH_H
#define ANOTHERGRAPH_GRAPH_H

#include "Node.cpp"
#include <stack>

#include "vector"
#include "map"


template <typename V>
class Graph {
    protected:
        typedef std::pair<unsigned int, std::vector<Node<V>*>> pair;
        struct CustomCompare {
            bool operator()(const pair& first, const pair& second) {
                return first.first > second.first;
            }
        };
        std::vector<Node<V>*> nodes;
        std::vector<std::vector<unsigned int>> matrix;
        Node<int>* head = nullptr;
        std::map<Node<V>*, unsigned int> nodeMap;


        void resize();
        
        int numVisited(std::vector<bool> listOfBools);

        bool contains(Node<V>* node, std::vector<Node<V>*> listOfNodes);

        void printAdjacent(Node<V>* next);

    public:

        void addNode(Node<V>* newNode);


        std::vector<Node<V>*> Dijkstra(Node<V>* find);

        std::vector<Node<V>*> getAdj(Node<V>* next);

        Graph();

        std::vector<Node<V>*> FrugalKugel(Node<V>* find, unsigned int steps);

};


#endif //ANOTHERGRAPH_GRAPH_H
