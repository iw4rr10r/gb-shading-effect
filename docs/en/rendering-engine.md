---
rank: 4
layout: default
title: Rendering Engine
permalink: /en/rendering-engine/
lang: en
lang-ref: renderer
---

# Organization of game architecture

After all this talk, we can finally move on to implementation <i class="far fa-smile-wink"></i>. We will start by setting up the rendering engine of our application. But just before we do, let's organize things a bit.

Let's define the global constants that will be useful to us in the future:

<div class="filename">constants.h</div>
```c++
#ifndef SHADING_EFFECT_CONSTANTS
#define SHADING_EFFECT_CONSTANTS

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 128

#endif
```

Then let's define the *sketch* `ShadingEffect.ino` of our application:

<div class="filename">ShadingEffect.ino</div>
```c++
#include <Gamebuino-Meta.h>
#include "GameEngine.h"

void setup() {
    gb.begin();

    // we will not use the standard graphic buffer
    // defined by `gb.display`, so let's initialize it with
    // a zero size so that it does not waste memory space
    gb.display.init(0, 0, ColorMode::rgb565);

    // initialization of the main controller
    GameEngine::init();
}

void loop() {
    while(!gb.update());

    // delegates the main control loop
    GameEngine::tick();
}
```

You may note that we are going to give global control of the application to the `GameEngine` component:

<div class="filename">GameEngine.h</div>
```c++
#ifndef SHADING_EFFECT_GAME_ENGINE
#define SHADING_EFFECT_GAME_ENGINE

class GameEngine
{
    public:

        // initialization
        static void init();

        // entry point of the main control loop
        static void tick();
};

#endif
```

Which we'll define as follows at this stage:

<div class="filename">GameEngine.cpp</div>
```c++
#include "GameEngine.h"

// the rendering engine
#include "Renderer.h"

void GameEngine::init() {
    // nothing special to do at the moment
}

void GameEngine::tick() {
    // performs rendering of the game scene
    Renderer::draw();
}
```

We see here that the main loop is simply responsible for rendering the game scene, which is relayed to the `Renderer` rendering engine:


# Definition of the rendering engine

Here we will strictly apply the Andy method described in the previous chapter to define our rendering engine:

<div class="filename">Renderer.h</div>
```c++
#ifndef SHADING_EFFECT_RENDERER
#define SHADING_EFFECT_RENDERER

#include <Gamebuino-Meta.h>
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

        // method to initiate memory forwarding to the DMA controller
        static void customDrawBuffer(int16_t x, int16_t y, uint16_t* buffer, uint16_t w, uint16_t h);
        // method for waiting for the transfer to be completed
        // and close the transaction with the DMA controller
        static void waitForPreviousDraw();
    
    public:

        // performs rendering of the game scene
        static void draw();
};

#endif
```

And here is the definition of magic methods:

<div class="filename">Renderer.cpp</div>
```c++
#include "Renderer.h"

// definition of the two memory buffers
uint16_t Renderer::buffer1[SCREEN_WIDTH * SLICE_HEIGHT];
uint16_t Renderer::buffer2[SCREEN_WIDTH * SLICE_HEIGHT];

// no memory forwarding is in progress at the moment
bool Renderer::drawPending = false;

// the magic routine related to the DMA controller....
// take a look at the official library
// if it arouses your curiosity:
// Gamebuino-META/src/utility/Display-ST7735/Display-ST7735.cpp
namespace Gamebuino_Meta {
  #define DMA_DESC_COUNT (3)
  extern volatile uint32_t dma_desc_free_count;

  static inline void wait_for_transfers_done(void) {
    while (dma_desc_free_count < DMA_DESC_COUNT);
  }

  static SPISettings tftSPISettings = SPISettings(24000000, MSBFIRST, SPI_MODE0);
};

// method to initiate memory forwarding to the DMA controller....
// these operations are also defined in:
// Gamebuino-META/src/utility/Display-ST7735/Display-ST7735.cpp
void Renderer::customDrawBuffer(int16_t x, int16_t y, uint16_t* buffer, uint16_t w, uint16_t h) {
  drawPending = true;
  gb.tft.setAddrWindow(x, y, x + w - 1, y + h - 1);
  SPI.beginTransaction(Gamebuino_Meta::tftSPISettings);
  gb.tft.dataMode();
  gb.tft.sendBuffer(buffer, w*h);
}

// method for waiting for the transfer to be completed
// and to close the transaction with the DMA controller...
// ditto, it's all in:
// Gamebuino-META/src/utility/Display-ST7735/Display-ST7735.cpp
void Renderer::waitForPreviousDraw() {
  if (drawPending) {
    // the little magic routine defined above
    Gamebuino_Meta::wait_for_transfers_done();
    gb.tft.idleMode();
    SPI.endTransaction();
    drawPending = false;
  }
}

// rendering of the game scene
void Renderer::draw() {
  // the number of horizontal slices to be cut is calculated
  uint8_t slices = SCREEN_HEIGHT / SLICE_HEIGHT;
  // then we go through each slice one by one
  for (uint8_t sliceIndex = 0; sliceIndex < slices; sliceIndex++) {

    // we define a pointer that will alternate between the two memory buffers
    uint16_t* buffer = sliceIndex % 2 == 0 ? buffer1 : buffer2;
    // the ordinate of the first horizontal fringe of the slice is calculated
    uint8_t sliceY = sliceIndex * SLICE_HEIGHT;

    // ---------------------------------------------------------------------
    // here we will perform the rendering calculations of our graphic assets
    // from the following 3 parameters:
    // - sliceY : ordinate of the current slice (upper fringe)
    // - SLICE_HEIGHT: height of the slices
    // - buffer : pointer to the current buffer
    // ---------------------------------------------------------------------
  
    // then we make sure that sending the previous buffer
    // to the DMA controller has taken place
    if (sliceIndex != 0) waitForPreviousDraw();
    // after which you can then send the current buffer
    customDrawBuffer(0, sliceY, buffer, SCREEN_WIDTH, SLICE_HEIGHT);
  }

  // always wait until the DMA transfer is completed
  // for the last slice before leaving the method!
  waitForPreviousDraw();
}
```

We now have to define precisely the calculation of the rendering of each graphic element present on the game scene. We have not yet defined them, but we will do so soon. However, beforehand, to introduce a little genericity to our rendering engine, we will define a very useful mechanism: the **Observer** *Design Pattern*... in the next chapter.