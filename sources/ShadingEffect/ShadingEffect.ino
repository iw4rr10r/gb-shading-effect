#include <Gamebuino-Meta.h>
#include "GameEngine.h"

void setup() {
    gb.begin();

    // serial port initialization
    SerialUSB.begin(9600);

    // we will not use the standard graphic buffer
    // defined by `gb.display`, so let's initialize it with
    // a zero size so that it does not waste memory space
    gb.display.init(0, 0, ColorMode::rgb565);

    // maximum frame rate to reach 100% of CPU loading
    // gb.setFrameRate(32);

    // initialization of the main controller
    GameEngine::init();
}

void loop() {
    while(!gb.update());

    // measure the CPU load every second (we are at 25 fps by default)
    // and send the data to the serial port
    if (gb.frameCount % 25 == 0) {
        SerialUSB.printf("CPU: %i, RAM: %i\n", gb.getCpuLoad(), gb.getFreeRam());
    }

    // delegates the main control loop
    GameEngine::tick();
}