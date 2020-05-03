---
rank: 5
layout: default
title: Observer Pattern
permalink: /en/observer-pattern/
lang: en
lang-ref: observer
---

# The Observer Design Pattern

The **Observer** *Design Pattern* is used to send a signal to objects that act as *observers*. In the event of notification, observers then take the appropriate action based on the information received from the objects they are observing ("observables"). The notions of observer and observable make it possible to limit the coupling between objects to the phenomena to be observed. They also allow a simplified management of multiple observers on the same observable object.

In this model, the observable subject is assigned a collection of observers that he notifies when states change. Each observer is responsible for making appropriate updates based on the notified changes. Thus, the observed is not responsible for the changes it impacts on the observers.

We will apply this pattern as part of our application. The observable will obviously be the `Renderer` rendering engine, and the observers will simply be the objects that can be displayed on the game scene: the ball and the tiling on which the ball moves.

As mentioned above, the observable is assigned a collection of observers. We will therefore need a data structure to handle this collection. And to keep a certain genericity, i.e. to keep a relative independence of the observers' data types, we will define an **interface** to qualify our observers in a generic way. An interface is a kind of contract to which all objects that subscribe to it must comply.

We could thus define a `Renderable` interface that guarantees that any object having subscribed to this contract is able to draw itself (and therefore to render its graphic appearance). In this way, the rendering engine would just have to handle a collection of `Renderable` objects, without really needing to know the exact nature of the observers. Indeed, he just needs to make sure that they are able to draw themselves.


# Definition of the Renderable interface

This interface can be defined very simply:

<div class="filename">Renderable.h</div>
```c++
#ifndef SHADING_EFFECT_RENDERABLE
#define SHADING_EFFECT_RENDERABLE

#include <Gamebuino-Meta.h>

class Renderable
{
    public:

        // pure virtual desctructor
        // it will have to be *extended* in derived classes
        virtual ~Renderable() = 0;

        // pure virtual method
        virtual void draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) = 0;
};

#endif
```

The notion of interface does not exist in the strict sense of the term in C++, as it can be found in Java or Ada for example. But there is a possibility to emulate it by relying on the notion of abstract class and *pure* virtual method. A pure virtual method is a method that is declared but not defined in a class. It is defined in one of the derived classes. To declare a pure virtual method in a class, simply follow its declaration with "`= 0`". The method must also be declared with the keyword `virtual`.

There is a small exception for pure virtual destructors. On the one hand, an abstract class must **necessarily** declare a virtual destructor (pure or not), but it must also **define it**... even if it is pure... and even if it does nothing particular.

It is indeed necessary to make the destructor of an abstract class **virtual** because when the object instantiated from one of its derived classes is destroyed, you will be in the typical case of a polymorphism. Let's take the following example:

```c++
class X
{
    public:
        ~X();
};

class Y : X // Y inherits from X
{
    private:
        char* a;
    public:
        Y() { a = new char[10]; }
        ~Y() { delete[] a; }
};

// and now suppose that we do the following:

X* pX = new Y(); // correct: a Y is an X
delete pX;       // which destructor is called here?
```

`pX` is considered as an `X` when it is actually a `Y`. In other words, when it is destroyed, it is the `~X()` destructor that will be invoked and not the one of the class `Y`! This introduces a potential insidious risk of memory leaks. Indeed, in the case described above, the memory allocated to contain the `a` array will never be released since the `~Y()` destructor is never invoked! More generally, and depending on what the `~Y()` destructor was supposed to do, the resulting behavior can be disastrous...

This problem can be solved by making the `X` destructor **virtual**:

```c++
class X
{
    public:
        virtual ~X();
};
```

By doing this, it is explicitly stated that the `X` class was designed to be derived. And that it is then fundamental to consider providing a specific definition of the destructor in its derived classes. But above all: the presence of a virtual destructor in `X` ensures that the operation `delete pX` invokes the destructor of the object actually pointed by `pX`, regardless of its type. Here, it is therefore the `~Y()` destructor that will be invoked, before going up the chain of inheritance and invoking the `~X()` destructor last. What is important to remember here is that a virtual destructor is not *overloaded*, but **extended**!!!!

Returning to the case of our `Renderable` interface, the virtual destructor will be defined by a default empty implementation. It will thus be the responsibility of the classes that will subscribe to the `Renderable` contract to specifically define their own destructor:

<div class="filename">Renderable.cpp</div>
```c++
#include "Renderable.h"

// a pure virtual destructor must be defined in an abstract class
// and in addition, it must be empty if she emulates an interface
Renderable::~Renderable() = default;
```

So, each object responding to the `Renderable` *interface* will have to implement not only its own destructor, but also the `draw()` method to draw itself when it receives a notification from the `Renderer` that it is its turn to draw itself. And remember what we mentioned in the definition of the `Renderer` class:

<div class="filename">Renderer.cpp</div>
```c++
void Renderer::draw() {
    // ...
    // ---------------------------------------------------------------------
    // here we will perform the rendering calculations of our graphic assets
    // from the following 3 parameters:
    // - sliceY : ordinate of the current slice (upper fringe)
    // - SLICE_HEIGHT: height of the slices
    // - buffer : pointer to the current buffer
    // ---------------------------------------------------------------------
    // ...
}
```

We therefore find these 3 parameters as arguments of the `draw()` function.

Well, now that the *contract* is established, we need to add to our rendering engine the ability to handle a collection of `Renderable' observers. We could handle this with a simple table... the problem is that in C++ a table must necessarily be sized so that the memory space necessary to contain it is allocated. However, in a game, the number of objects present on the game scene and to be displayed can be very variable. It is therefore dangerous or limiting to define the size of such a table *a priori*. There may even be a tendency to want to allocate more space than necessary to avoid reaching the limits of the number of objects that can be referenced in this table. So it's definitely a very bad idea!

In this case, it is preferable to handle a dynamic data structure, the size of which may vary according to the needs during the execution of the program. One of the simplest well-known dynamic data structures in algorithmics is the *linked list*.


# Dynamic collection of observers

We will therefore assign a list of observers to our rendering engine. This list will be referred to as the static variable `listeners` (observers are also referred to as *listeners*). So, should we declare this variable this way?

```c++
static Renderable* listeners;
```

**NO!** It's a little more complicated than that <i class="far fa-smile-wink"></i>.

Observers will have to be stored in boxes that we will link together with pointers. These links will constitute the *nodes* (`Node`) of the list. And here is how the structure of this linked list will look like:

![linked-list](../../assets/figures/observer/linked-list.png){: width="504" height="186" }

Each `Node` consists of 2 pointers:

1. `renderable` which is a pointer to an object of type `Renderable`
2. `next` which is a pointer to an object of type `Node`

The `renderable` pointer therefore allows access to the `renderable` object that is encapsulated in the node, while the `next` pointer allows access to the next node in the list.

The correct declaration of our static variable `listeners` is therefore:

```c++
static Node* listeners;
```

And the last node in the list will have its `next` pointer pointing to `NULL` (nothing).

Let's now see how to implement this linked list. For this we will need to declare the class `Node`:

<div class="filename">Node.h</div>
```c++
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
};

#endif
```

Here is the definition:

<div class="filename">Node.cpp</div>
```c++
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
```

You can see that in the case of deleting a node, it is assumed here that the current node does not encapsulate the `Renderable` object that is being searched. Indeed, the search is carried out from the next node. This is normal, don't worry, the removal of the current node cannot be done here. Indeed, if we delete the current node... then the previous node would lose its reference to its next one (which is precisely the current node)... and at the same time lose the whole list! Therefore, we cannot deal with this case here. We will process it in the `Renderer` class which holds the reference at the top of the list.


# Registration of observers

Remember that ultimately, the handling of this linked list should allow us to handle the `Renderer` observers. And so, adding or removing an observer from this list is equivalent, from the `Renderer`'s point of view, to allowing an object of the `Renderable` type to subscribe or unsubscribe to the `Renderer`'s notifications. So this is how to complete the declaration of the class `Renderer`:

<div class="filename">Renderer.h</div>
```c++
#ifndef SHADING_EFFECT_RENDERER
#define SHADING_EFFECT_RENDERER

#include <Gamebuino-Meta.h>
#include "Node.h"
#include "Renderable.h"
#include "constants.h"

// definition of the slices height
#define SLICE_HEIGHT 8

class Renderer
{
    private:
    
        // pointer to the list of observers
        static Node* listeners;
    
    public:

        // observer subscription
        static void subscribe(Renderable* renderable);
        // unsubscribing an observer
        static void unsubscribe(Renderable* renderable);
        // allows to know if an object of type `Renderable` is already subscribed
        static bool hasSubscribed(Renderable* renderable);

    // all other declarations remain unchanged
};

#endif
```

This is how to define these new methods:

<div class="filename">Renderer.cpp</div>
```c++
// the list is empty by default
Node* Renderer::listeners = NULL;

// observer subscription
void Renderer::subscribe(Renderable* renderable) {
  // if the list is empty, initialize it with the new observer ;-)
  if (listeners == NULL) {
    listeners = new Node(renderable);
  } else {
    // otherwise the addition is delegated to the following node
    // since the addition must be done at THE END of the list
    listeners->add(renderable);
  }
}

// unsubscribing an observer
void Renderer::unsubscribe(Renderable* renderable) {
  // if the list is empty, there's nothing you can do!
  // on the other hand, if it is not....
  if (listeners != NULL) {
    // if the heading node of the list is the one that
    // encapsulates the observer we're trying to identify
    if (listeners->getRenderable() == renderable) {
      // we get the reference to the following node
      // which will become the new head of the list
      Node* next = listeners->getNext();
      // and so we can permanently delete the first node
      delete listeners;
      // we hang up the new head of the list
      listeners = next;
    } else {
      // otherwise, the deletion is delegated
      // to the next node in the list....
      listeners->del(renderable);
    }
  }
}

// to know if an object of type `Renderable` is already subscribed
bool Renderer::hasSubscribed(Renderable* renderable) {
  // it is enough to identify the node that is the holder of it
  return listeners->search(renderable) != NULL;
}
```

You can see that our node removal problem is now solved <i class="far fa-smile-wink"></i>.


# Sending notifications

There's one last thing left to do here! Set up the notification mechanism for all observers. And yes, now we can finally ask all our observers to take turns drawing themselves in the order of the display stack, that is, precisely in the order in which they are placed in the subscription list. The heading node is drawn, then the next node, and so on, until the end of the list:

<div class="filename">Renderer.cpp</div>
```c++
void Renderer::draw() {
  if (listeners != NULL) {
    uint8_t slices = SCREEN_HEIGHT / SLICE_HEIGHT;
    for (uint8_t sliceIndex = 0; sliceIndex < slices; sliceIndex++) {
      uint16_t* buffer = sliceIndex % 2 == 0 ? buffer1 : buffer2;
      uint8_t sliceY = sliceIndex * SLICE_HEIGHT;
    
      // the notification is sent to the heading node
      // which in turn will relay it to the next node,
      // and so on, until the end of the list
      listeners->draw(sliceY, SLICE_HEIGHT, buffer);

      if (sliceIndex != 0) waitForPreviousDraw();
      customDrawBuffer(0, sliceY, buffer, SCREEN_WIDTH, SLICE_HEIGHT);
    }
    waitForPreviousDraw();
  }
}
```

To set up this mechanism, we still have to add in the `Node` class the taking into account of the notification signal to relay it to the `Renderable` object that it encapsulates. So, in the class `Node` we will declare a method `draw()` which will transmit this notification:

<div class="filename">Node.h</div>
```c++
class Node
{
    public:

        // relays the notification from the `Renderer`
        // that it's time to draw
        void draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer);

};
```

<div class="filename">Node.cpp</div>
```c++
// the renderer engine notifies that it's time to render!
void Node::draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) {
    // the observer draws itself
    this->renderable->draw(sliceY, sliceHeight, buffer);
    // and we pass the relay to the next node if it exists
    if (this->next != NULL) {
        this->next->draw(sliceY, sliceHeight, buffer);
    }
}
```

And the loop is closed <i class="far fa-smile-wink"></i>.

You can see that our `Renderable` interface is very useful here. We rely solely on the fact that the contract described by this interface guarantees that the object is capable of being drawn. The rendering engine has no knowledge of the exact nature of its observers. And yet everything works perfectly.

Well, now it's time to focus on the objects we're going to draw on the game scene: the ball and the tiling on which it moves. We will start with the ball in the next chapter...