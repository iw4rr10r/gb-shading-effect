#include "GameEngine.h"
#include "Renderer.h"

// always initialize a pointer to NULL
Tiling* GameEngine::tiling = NULL;
Ball* GameEngine::ball = NULL;

void GameEngine::init() {
    // instantiation of the tiling
    tiling = new Tiling();
    // instantiation of the ball
    ball = new Ball();

    // registration of observers
    // with the rendering engine
    Renderer::subscribe(tiling);
    Renderer::subscribe(ball);
}

void GameEngine::tick() {

    // interception of user events

    if (gb.buttons.repeat(BUTTON_LEFT, 1)) {
        tiling->left();
    }

    if (gb.buttons.repeat(BUTTON_RIGHT, 1)) {
        tiling->right();
    }

    if (gb.buttons.repeat(BUTTON_UP, 1)) {
        tiling->up();
    }

    if (gb.buttons.repeat(BUTTON_DOWN, 1)) {
        tiling->down();
    }

    // the calculation of the motio is then delegated to the tiling
    // so we're going to add a control loop to it
    // to perform these calculations
    tiling->tick();

    // performs rendering of the game scene
    Renderer::draw();
}