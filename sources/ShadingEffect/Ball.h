#ifndef SHADING_EFFECT_BALL
#define SHADING_EFFECT_BALL

#include "Renderable.h"

// here is how to declare the fact that the `Ball` class
// fulfills the contract defined by the `Renderable` interface
// (which is actually a class)
class Ball : public Renderable
{
    private:

        // the descriptive parameters of the sprite
        static const uint8_t FRAME_WIDTH;
        static const uint8_t FRAME_HEIGHT;
        static const uint16_t TRANSPARENT_COLOR;
        // the pixel map obtained with the transcoding tool
        static const uint16_t BITMAP[];
        
        // the coordinates of the ball, which are constant
        // since the ball is fixed in the center of the screen
        static const uint8_t X_POS;
        static const uint8_t Y_POS;

    public:

        // the famous method of fulfilling the `Renderable` contract
        void draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) override;
};

#endif