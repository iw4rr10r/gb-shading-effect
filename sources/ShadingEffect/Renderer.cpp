#include "Renderer.h"

// definition of the two memory buffers
uint16_t Renderer::buffer1[SCREEN_WIDTH * SLICE_HEIGHT];
uint16_t Renderer::buffer2[SCREEN_WIDTH * SLICE_HEIGHT];

// no memory forwarding is in progress at the moment
bool Renderer::drawPending = false;

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

    // the notification is sent to the heading node
    // which in turn will relay it to the next node,
    // and so on, until the end of the list
    listeners->draw(sliceY, SLICE_HEIGHT, buffer);
  
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