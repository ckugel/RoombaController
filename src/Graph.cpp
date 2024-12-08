//
// Created by caleb on 11/9/21.
//

#include "Graph.hpp"

#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>
#include "limits.h"

template<typename V>
Graph<V>::Graph() {
    std::vector<double> temp;
    temp.push_back(0);
    temp.push_back(0);
    matrix.push_back(temp);
    matrix.push_back(temp);
}

/*
template<typename V>
void Graph<V>::playGround() {
    Node<V> *tempNode = new Node<V>(0);
    addNode(tempNode);
    // srand(time(0));

    addNode(new Node<V>(1), nodes[0], 2);
    addNode(new Node<V>(2), nodes[1], 1);
    addNode(new Node<V>(3), nodes[2], 3);
    addNode(new Node<V>(4), nodes[0], 2);
    addConnection(nodes[4], nodes[2], 3);
    addNode(new Node<V>(5), nodes[1], 2);
    addNode(new Node<V>(6), nodes[1], 1);
    addConnection(nodes[5], nodes[3], 7);
    addNode(new Node<V>(7), nodes[2], 5);
    addNode(new Node<V>(8), nodes[6], 5);
    addConnection(nodes[8], nodes[3], 3);
    addNode(new Node<V>(9), nodes[8], 1);
    addConnection(nodes[9], nodes[7], 8);


    head = nodes[0];


    // printOut();

    printAdjacent(head);

    std::cout << std::endl;

    std::vector<Node<V>*> path = Dijkstra(nodes[7]);

    std::cout << "goal: " << nodes[7]->getData() << std::endl;

    std::cout << "\n\n\n Path: ";
    if (!path.empty()) {
        for (int looper = 0; looper < path.size(); looper++) {
            if (looper != path.size() - 1) {
                std::cout << nodeMap.at(path[looper]) << "->";
            } else {
                std::cout << nodeMap.at(path[looper]);
            }
        }
        std::cout << std::endl;
    }

/*    std::cout << "\n\n\n Vals: " << std::endl;
    for (int i = 0; i < nodes.size(); i++) {
        std::cout << i << " -> " << nodes[i] << std::endl;
    }*/


// }


template<typename V>
void Graph<V>::resize() {
    matrix.resize(nodes.size());
    for (std::vector<double>& looper : matrix) {
        looper.resize(nodes.size());
    }
}

template<typename V>
bool Graph<V>::contains(Node<V>* node, std::vector<Node<V>*> listOfNodes) {
    for (int looper = 0; looper < listOfNodes.size(); looper++) {
        if (node == listOfNodes[looper]) {
            return true;
        }
    }
    return false;
}

template<typename V>
std::vector<Node<V>*> Graph<V>::getNodes() {
    return this->nodes;
}

template<typename V>
void Graph<V>::addNode(Node<V>* newNode) {
    if (contains(newNode, nodes)) {return;}
    head = newNode;
    nodes.push_back(newNode);
    nodeMap[newNode] = nodes.size() - 1;
    resize();
}

template<typename V>
void Graph<V>::addNode(Node<V>* nextNode, std::vector<Node<V>*> adjacentNodes, double weight) {
    addNode(nextNode);
    for (int looper = 0; looper < adjacentNodes.size(); looper++) {
        if (!contains(adjacentNodes[looper], nodes)) {
            nodes.push_back(adjacentNodes[looper]);
            nodeMap[adjacentNodes[looper]] = nodes.size() - 1;
        }
        resize();
        int currNodeNum = nodeMap.at(nextNode);
        int nextNodeNum = nodeMap.at(adjacentNodes[looper]);
        matrix[currNodeNum][nextNodeNum] = weight;
        matrix[nextNodeNum][currNodeNum] = weight;
    }
}

template<typename V>
void Graph<V>::addNode(Node<V> *nextNode, std::vector<Node<V>*> adjacentNodes) {
    addNode(nextNode, adjacentNodes, 1);
}

template<typename V>
void Graph<V>::addNode(Node<V> *nextNode, Node<V> *nodeITSLATE) {
    std::vector<Node<V>*> adj;
    adj.push_back(nodeITSLATE);
    addNode(nextNode, adj);
}

/*
template<typename V>
void Graph<V>::printOut() {
    for (int looper = 0; looper < nodes.size(); looper++) {
        if (looper != nodes.size() - 1) {
            std::cout << nodes[looper]->getData() << ", ";
        }
        else {
            std::cout << nodes[looper]->getData();
        }
    }
    std::cout << std::endl;

    for (int anotherLoop = 0; anotherLoop < nodes.size(); anotherLoop++) {
        std::cout << "-";
    }
    std::cout << std::endl;

    for (int i = 0; i < matrix.size(); i++) {
        for (int ii = 0; ii < matrix[i].size(); ii++) {
            if (ii != matrix.size() - 1) {
                std::cout << matrix[i][ii] << ", ";
            }
            else {
                std::cout << matrix[i][ii];
            }
        }
        std::cout << std::endl;
    }
    std::cout << "\n\n\n" << std::endl;
}
*/

template<typename V>
std::vector<Node<V>*> Graph<V>::getAdj(Node<V>* next) {
    std::vector<Node<V>*> adjacent;
    int index = nodeMap.at(next);

    for (int looper = 0; looper < matrix.size(); looper++) {
        if (matrix[index][looper] != 0) {
            adjacent.push_back(nodes[looper]);
        }
    }
    return adjacent;
}

template<typename V>
void Graph<V>::setHead(int index) {
    head = getNodes()[index];
}


template<typename V>
std::vector<Node<V>*> Graph<V>::Dijkstra(Node<V>* from, Node<V>* find) {
    std::vector<Node<V>*> thing;
    if (from == find) {
	// std::cout << "was head" << std::endl;
        return thing;
    }
    std::vector<bool> visited;
    visited.resize(nodes.size());
    double distances[nodes.size()];

    for(int filler = 0; filler < nodes.size(); filler++) {
        visited[filler] = 0;
        distances[filler] = UINT_MAX;
    }
    distances[0] = 0;

    std::priority_queue<pair, std::vector<pair>, CustomCompare> queue;
    std::vector<Node<V>*> temp;
    temp.push_back(from);
    queue.push(std::pair<double, std::vector<Node<V>*>>(0, temp));
    uint64_t iterations = 0;

    while (!queue.empty()) {
        iterations++;

        std::vector<Node<V>*> path = queue.top().second;        
        Node<V> *curr = path[path.size() - 1];
        double currentDistance = queue.top().first;
        queue.pop();

        visited[nodeMap.at(curr)] = 1;

        if (find == curr) {
            // std::cout << "\n\nDistance: " << currentDistance << std::endl;
            std::cout << iterations << std::endl;
            return path;
        }

        std::vector<Node<V>*> adj = getAdj(curr);

        for (int looper = 0; looper < adj.size(); looper++) {
            std::vector<Node<V>*> possiblePath;
            for (int i = 0; i < path.size(); i++) {
                possiblePath.push_back(path[i]);
            }
            // printAdjacent(curr);
            if (!visited[nodeMap.at(adj[looper])] && currentDistance + matrix[nodeMap.at(curr)][nodeMap.at(adj[looper])] < distances[nodeMap.at(adj[looper])]) {
                distances[nodeMap.at(adj[looper])] = currentDistance + matrix[nodeMap.at(curr)][nodeMap.at(adj[looper])];
                possiblePath.push_back(adj[looper]);
                queue.push(std::pair<double, std::vector<Node<V>*>>(currentDistance + matrix[nodeMap.at(curr)][nodeMap.at(adj[looper])], possiblePath));
            }
        }
        thing = path;
    }

    // std::cout << "ATTEMPT TO RETURN SIZE: " << thing.size() << std::endl;
   //  delete queue;
    // delete[] distances;
    std::cout << iterations << std::endl;
    return thing;
}

template<typename V>
Graph<V>::~Graph() {
    // delete all connections and nodes
    for (Node<V>* node : nodes) {
	delete node;
    }
}

template<typename V>
std::vector<std::vector<double>> Graph<V>::getAdjacencyList() {
    return matrix;
}

template<typename V>
void Graph<V>::addConnection(Node<V> *one, Node<V> *two, double weight) {
    int oneIndex = nodeMap.at(one);
    int twoIndex = nodeMap.at(two);

    matrix[oneIndex][twoIndex] = weight;
    matrix[twoIndex][oneIndex] = weight;

}

template<typename V>
void Graph<V>::addConnection(Node<V> *one, Node<V> *two) {
    addConnection(one, two, 1);
}

template<typename V>
void Graph<V>::addNode(Node<V> *nextNode, Node<V> *nodeITSLATE, double weight) {
    std::vector<Node<V>*> adj;
    adj.push_back(nodeITSLATE);
    addNode(nextNode, adj, weight);
}

template<typename V>
int Graph<V>::numVisited(std::vector<bool> listOfBools) {
    int counter = 0;
    for (int i = 0; i < listOfBools.size(); i++) {
        if (listOfBools[i]) {
            counter++;
        }
    }
    return counter;
}

/*
template<typename V>
void Graph<V>::printAdjacent(Node<V>* next) {
    std::vector<Node<V>*> list = getAdj(next);
    std::cout << "current: " << next->getData() << std::endl;

    std::cout << "Adjacent nodes: ";
    for (int looper = 0; looper < list.size(); looper++) {
        std::cout << nodeMap.at(list[looper]);
        if (looper != list.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}
*/
/*
template<typename V> 
std::vector<Node<V>*> Graph<V>::FrugalKugel(Node<V>* find, unsigned int steps) {
    std::vector<Node<V>*> toReturn;
    if (head == find) {
        return toReturn;
    }
    std::vector<bool> visited;
    visited.resize(nodes.size());
    unsigned int distances[nodes.size()];

    // set distances except for the first one to infinity
    for (int filler = 1; filler < nodes.size(); filler++) {
        distances[filler] = UINT_MAX;
    }
    distances[0] = 0;

    // setup a prioirty queue
    std::priority_queue<pair, std::vector<pair>, CustomCompare> queue;
    std::vector<Node<V>*> temp;
    temp.push_back(head);
    queue.push(std::pair<unsigned int, std::vector<Node<V>*>>(0, temp));

    while (!queue.empty()) {
        // get the path from the top of the queue
        std::vector<Node<V>*> path = queue.top().second;
        // get the most recent item added to the path
        Node<V>* curr = path[path.size() - 1];
        unsigned int currentDistance = queue.top().first;
        // remove the current path from the PQ
        queue.pop();

        visited[nodeMap.at(curr)] = 1;

        // found the goal node
        if (find == curr) {
            std::cout << "\nDistance: " << currentDistance << std::endl;
            return path;
        }

        std::vector<Node<V>*>& adj = nodes[nodeMap.at(curr)];

        for (int looper = 0; looper <  adj.size(); looper++) {
            if (adj[looper] != 0) {
                std::vector<Node<V>*> possiblePath;
                // copy over every element from path
                for (int i = 0; i < path.size(); i++) {
                    possiblePath.push_back(path[i]);
                }
                // print the adjacent nodes
                printAdjacent(curr);
                // cache the index
                int index = nodeMap.at(adj[looper]);
                // the weight between the current node and the node we ate checking
                double tempDistance  = ((path.size() < 2) ? INT_MAX : matrix[nodeMap.at(curr)][index]);
                double sumDistance = tempDistance + currentDistance;
                // checks if we have visited the node we are inspecting, the distance is less than the cirrent distance, and
                if (!visited[index] && sumDistance < distances[index]) {
                    distances[index] = sumDistance;
                    possiblePath.push_back(adj[looper]);
                    queue.push(std::pair<unsigned int, std::vector<Node<V>*>>(sumDistance, possiblePath));
                }
            }
        }
        toReturn = path;
    }
    delete queue;
    delete[] distances;
    return toReturn;
}

*/


template<typename V>
void Graph<V>::removeNode(size_t index) {
    if (index >= nodes.size()) {
        return;
    }
    delete nodes.at(index);
    nodes.erase(nodes.begin() + index);
    resize();
}
