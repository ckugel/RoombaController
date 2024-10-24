//
// Created by caleb on 11/9/21.
//

#include "Node.h"


template<typename V>
Node<V>::Node(V data) {
    this->data = data;
}

template<typename V>
V Node<V>::getData() {
    return data;
}

template<typename V>
void Node<V>::SetData(V newData) {
    data = newData;
}
