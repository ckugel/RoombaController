//
// Created by caleb on 11/9/21.
//

#ifndef ANOTHERGRAPH_NODE_H
#define ANOTHERGRAPH_NODE_H


#include "Pose2D.hpp"

template <typename V>
class Node {
    private:
        V data;
    public:
        Node(V data);
        V getData();
        void SetData(V newData);
};

#include "Node.cpp"

#endif //ANOTHERGRAPH_NODE_H
