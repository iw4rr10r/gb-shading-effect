#include "Node.h"

// the constructor initializes the attributes of the node
Node::Node(Renderable* renderable) : renderable(renderable), next(NULL) {};

// the destroyer "forgets" the objects he was referencing
Node::~Node() {
    this->renderable = NULL;
    this->next = NULL;
}

// allows access to the encapsulated `Renderable` object
Renderable* Node::getRenderable() {
    return this->renderable;
}

// allows access to the next node in the list
Node* Node::getNext() {
    return this->next;
}

// searches for a particular `Renderable` object
// and returns a pointer to the node holding this object
Node* Node::search(Renderable* renderable) {
    if (this->renderable == renderable) {
        return this;
    }

    // if we get to the end of the list, it's because
    // the `Renderable` object searched for was not found
    if (this->next == NULL) {
        return NULL;
    }

    // if the current node is not the holder,
    // then we perform the search in the next node,
    // and so on....
    return this->next->search(renderable);
}

// adds a node at THE END of the list
void Node::add(Renderable* renderable) {
    if (this->next == NULL) {
        this->next = new Node(renderable);
    } else {
        this->next->add(renderable);
    }
}

// deletes the node holding the `Renderable` object
// that we're trying to identify....
void Node::del(Renderable* renderable) {
    if (this->next != NULL) {
        if (this->next->getRenderable() == renderable) {
            Node* toDelete = this->next;
            this->next = toDelete->getNext();
            delete toDelete;
        } else {
            this->next->del(renderable);
        }
    }
}

// the renderer engine notifies that it's time to render!
void Node::draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) {
    // the observer draws itself
    this->renderable->draw(sliceY, sliceHeight, buffer);
    // and we pass the relay to the next node if it exists
    if (this->next != NULL) {
        this->next->draw(sliceY, sliceHeight, buffer);
    }
}