#ifndef SHADING_EFFECT_TILING
#define SHADING_EFFECT_TILING

#include "Renderable.h"

// the constant driving impulse
#define PULSE 1

// the distance calculations could be done quickly using
// the square of the halo radius simply expressed as a power of 2
#define HALO_RADIUS2_POWER_OF_TWO 11

// and we can do the same with brightness levels
#define BRIGHTNESS_LEVELS_POWER_OF_TWO 5

// the Tiling class fulfills the contract defined in the `Renderable` interface
class Tiling : public Renderable
{
    private:

        // the descriptive parameters of the sprite
        static const uint8_t TILE_WIDTH;
        static const uint8_t TILE_HEIGHT;

        // the pixel map obtained with the transcoding tool
        // note the change in the type of values
        // now stored in the spritesheet
        static const uint8_t BITMAP[];

        // the color palette associated with the spritesheet
        // including all the luminous versions of the
        // reference colors of our sprites
        static const uint16_t COLORMAP[];

        // the coordinates of the acceleration vector
        float ax,ay;

        // the coordinates of the velocity vector
        float vx,vy;

        // the coordinates of the position vector are no other than
        // than those of our displacement vector
        int8_t offsetX,offsetY;

    public:

        // a constructor is defined in which
        // we will initialize all the kinematics properties
        Tiling();

        // the move commands invoked by `GameEngine`
        void left();
        void right();
        void up();
        void down();

        // the connection point of the control loop
        void tick();

        // the rendering method imposed by the `Renderable` contract
        void draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) override;
};

#endif