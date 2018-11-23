#ifndef SHADING_EFFECT_NODE
#define SHADING_EFFECT_NODE

#include "Renderable.h"

class Node
{
    private :

        // pointer to the `Renderable` object
        Renderable* renderable;
        // pointer to the next node in the list
        Node* next;

    public:

        // node constructor
        Node(Renderable* renderable);
        // node destructor
        ~Node();

        // access method to the encapsulated `Renderable` object
        Renderable* getRenderable();
        // access method to the next node in the list
        Node* getNext();

        // method of searching for a particular `Renderable` object
        Node* search(Renderable* renderable);
        // method to add a new node to the list
        void add(Renderable* renderable);
        // method to remove a node from the list
        void del(Renderable* renderable);

        // relays the notification from the `Renderer`
        // that it's time to draw
        void draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer);
};

#endif