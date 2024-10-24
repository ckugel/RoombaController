//
// Created by caleb on 11/9/21.
//

#ifndef ANOTHERGRAPH_NODE_H
#define ANOTHERGRAPH_NODE_H


template <typename V>
class Node {
    private:
        V data;
    public:
        explicit Node(V data);
        V getData();
        void SetData(V newData);
};


#endif //ANOTHERGRAPH_NODE_H
