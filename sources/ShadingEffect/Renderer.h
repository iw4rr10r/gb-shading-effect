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
    
        // declaration of the two memory buffers in which
        // will be performed the rendering calculations
        static uint16_t buffer1[SCREEN_WIDTH * SLICE_HEIGHT];
        static uint16_t buffer2[SCREEN_WIDTH * SLICE_HEIGHT];
        // flag which will tell us if a memory forwarding
        // to the DMA controller is in progress or not
        static bool drawPending;

        // pointer to the list of observers
        static Node* listeners;

        // method to initiate memory forwarding to the DMA controller
        static void customDrawBuffer(int16_t x, int16_t y, uint16_t* buffer, uint16_t w, uint16_t h);
        // method for waiting for the transfer to be completed
        // and close the transaction with the DMA controller
        static void waitForPreviousDraw();
    
    public:

        // observer subscription
        static void subscribe(Renderable* renderable);
        // unsubscribing an observer
        static void unsubscribe(Renderable* renderable);
        // allows to know if an object of type `Renderable` is already subscribed
        static bool hasSubscribed(Renderable* renderable);

        // performs rendering of the game scene
        static void draw();
};

#endif