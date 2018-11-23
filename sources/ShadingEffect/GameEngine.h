#ifndef SHADING_EFFECT_GAME_ENGINE
#define SHADING_EFFECT_GAME_ENGINE

// we will define the `Tiling` class just after...
#include "Tiling.h"
#include "Ball.h"

class GameEngine
{
    private:

        // a pointer to the instance of the tiling
        static Tiling* tiling;

        // a pointer to the instance of the ball
        static Ball* ball;

    public:

        // initialization
        static void init();

        // entry point of the main control loop
        static void tick();
};

#endif