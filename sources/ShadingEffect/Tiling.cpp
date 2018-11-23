#include <Gamebuino-Meta.h>
#include "Tiling.h"
#include "constants.h"

// the descriptive parameters of our sprites
const uint8_t Tiling::TILE_WIDTH = 16;
const uint8_t Tiling::TILE_HEIGHT = 16;

// the spritesheet obtained with the transcoding tool
const uint8_t Tiling::BITMAP[] = {
    // light tile
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0, 
    // dark tile
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 0
};

// the color palette provided by the transcoding tool
// including their 32 levels of brightness
const uint16_t Tiling::COLORMAP[] = {
    // color #0 (transparent color)
    0xffff, 
    // color #1
    0x79ce, 0x38c6, 0xf7bd, 0xd7bd, 0x96b5, 0x75ad, 0x34a5, 0xf39c, 0xd39c, 0x9294, 0x718c, 0x3084, 0x1084, 0xcf7b, 0x8e73, 0x6d6b, 0x2c63, 0x0c63, 0xcb5a, 0x8a52, 0x694a, 0x2842, 0x0842, 0xc739, 0x8631, 0x6529, 0x2421, 0x0421, 0xc318, 0x8210, 0x6108, 0x2000, 
    // color #2
    0x55ad, 0x34a5, 0xf39c, 0xd39c, 0xb294, 0x718c, 0x518c, 0x3084, 0x1084, 0xcf7b, 0xae73, 0x8e73, 0x4d6b, 0x2c63, 0x0c63, 0xcb5a, 0xaa52, 0x8a52, 0x494a, 0x2842, 0x0842, 0xc739, 0xa631, 0x8631, 0x4529, 0x2421, 0x0421, 0xc318, 0xa210, 0x8210, 0x4108, 0x2000, 
    // color #3
    0x2842, 0x0842, 0x0842, 0xe739, 0xe739, 0xc739, 0xa631, 0xa631, 0x8631, 0x8631, 0x6529, 0x6529, 0x4529, 0x4529, 0x2421, 0x2421, 0x0421, 0x0421, 0xe318, 0xe318, 0xc318, 0xa210, 0xa210, 0x8210, 0x8210, 0x6108, 0x6108, 0x4108, 0x4108, 0x2000, 0x2000, 0x0000, 
    // color #4
    0x518c, 0x3084, 0x1084, 0xcf7b, 0xae73, 0x8e73, 0x6d6b, 0x4d6b, 0x2c63, 0x0c63, 0xeb5a, 0xcb5a, 0xaa52, 0x8a52, 0x694a, 0x494a, 0x2842, 0x0842, 0xe739, 0xa631, 0x8631, 0x6529, 0x4529, 0x2421, 0x0421, 0xe318, 0xc318, 0xa210, 0x8210, 0x6108, 0x4108, 0x2000
};

// then comes the constructor, which initializes:
// - the acceleration vector
// - the veolcity vector
// - the displacement vector
Tiling::Tiling() {
    this->ax = 0;
    this->ay = 0;
    this->vx = 0;
    this->vy = 0;
    this->offsetX = 0;
    this->offsetY = 0;
}

// the fundamental relationship of dynamics applies
// here in a very simpl way :-)

void Tiling::left() {
    this->ax = -PULSE;
}

void Tiling::right() {
    this->ax = PULSE;
}

void Tiling::up() {
    this->ay = -PULSE;
}

void Tiling::down() {
    this->ay = PULSE;
}

// connection to the control loop
// is an opportunity to apply our motion equations

void Tiling::tick() {
    // the velocity vector is directly derived
    // from the acceleration vector
    this->vx += this->ax;
    this->vy += this->ay;

    // the displacement vector is directly derived
    // from the velocity vector
    this->offsetX += this->vx;
    this->offsetY += this->vy;

    // !watch out here!
    // remember that acceleration is an INSTANT value
    // so once it's been applied to the velocity vector,
    // you must reset it to zero!
    this->ax = 0;
    this->ay = 0;

    // application of the friction phenomenon:
    this->vx *= .9;
    this->vy *= .9;

    // and we can also decide that when the components
    // of the velocity become less than 0.5 (in absolute values),
    // then we can consider that they simply cancel each other out
    if (this->vx > -.5 && this->vx < .5) { this->vx = 0; }
    if (this->vy > -.5 && this->vy < .5) { this->vy = 0; }
}

// and we define the method for calculating the rendering of the tiling
void Tiling::draw(uint8_t sliceY, uint8_t sliceHeight, uint16_t* buffer) {
    // we will pre-calculate some parameters
    // to optimize the processing time....

    // we will try to number the tiles according to the two axes X and Y
    // to associate indices that could be named `tx` and `ty`.
    // and then we will calculate a parity flag on these two indices:
    //   txodd = true when `tx` is odd and false otherwise
    //   tyodd = true when `ty` is odd and false otherwise
    bool txodd,tyodd;

    // it will then be sufficient to test jointly the parity of `tx` and `ty`
    // to know if you should display a light or dark tile
    // in other words, if you have to make a "jump" in the spritesheet
    // to reach the dark tile... that's the role of the `jump` flag
    bool jump;

    // and here is the offset to apply in the spritesheet
    // to access the colors of the dark tile
    uint16_t nfo = TILE_WIDTH * TILE_HEIGHT;

    // all pixels (sx,sy) of the current slice will be examined...
    // it will then be necessary to switch from the (sx,sy) local coordinate
    // system to the (x,y) global coordinate system of the screen...
    // and `sx` is actually equivalent to `x` since the slice covers the entire
    // width of the screen:
    //   x = sx
    //   y = sy + sliceY
    uint8_t sy,x,y;

    // don't forget that we have to take into account the displacement vector
    // (offsetX, offsetY) that will be applied to the tiling to give an impression
    // of motion at the ball...
    // we will therefore have to transpose our global coordinate system (x,y):
    //   xo = x + offsetX
    //   yo = y + offsetY
    uint8_t xo,yo;

    // when we have to write in the buffer, we will have to deal with
    // a one-dimensional array... so we will have to project the
    // global coordinates (x,y) on the corresponding index in the buffer :
    //   buffer_index = x + (sy * SCREEN_WIDTH)
    //                       -------syw-------
    uint16_t syw;

    // when we have to go pick the `value` color from the spritesheet
    // which will then have to be copied into the buffer, we will have to
    // calculate to which index to look for this color in the BITMAP array
    uint16_t index, value;

    // the calculation of this index can be broken down into two parts:
    //   index = index_x + index_y, where :
    //   index_x = (xo % TILE_WIDTH) + (jump * nfo)
    //   index_y = (yo % TILE_HEIGHT) * TILE_WIDTH
    uint16_t index_y;

    // we will need to measure the square of the distance `r2`
    // between the pixel of the tiling and the center of the screen....
    // an intermediate calculation will determine
    // the Y component of `r2` that will be named `ry2`
    uint16_t r2,ry2;
    uint8_t hsw = SCREEN_WIDTH / 2;
    uint8_t hsh = SCREEN_HEIGHT / 2;

    // we will then calculate the brightness level of the pixel
    // as a function of the ratio between the square of the distance `r2`
    // which separates it from the center of the screen
    // and the square of the halo radius defined as a power of 2
    // `HALO_RADIUS2_POWER_OF_TWO`
    uint8_t lux;

    // this variable will allow us to store the color index
    // retrieved from the spritesheet and corresponding to
    // the index of the reference color in the `COLORMAP` palette
    uint8_t colorIndex;

    // we plunge the whole scene into darkness by filling the slice with zeros
    // > we use pointers here to speed up processing...
    // we could also move this procedure
    // in the `draw()` method of the `Renderer`
    uint32_t* tmp = (uint32_t*)buffer;
    uint16_t bs = (sliceHeight * SCREEN_WIDTH) >> 1;
    while (bs--) *tmp++ = 0;

    // scanning of each pixel of the slice (here the Y component)
    for (sy = 0; sy < sliceHeight; sy++) {

        // transition from the two-dimensional system of the slice
        // to the one-dimensional system of the buffer
        syw = sy * SCREEN_WIDTH;

        // transition from the local coordinate system of the slice
        // to the global coordinate system of the screen
        y = sliceY + sy;

        // the Y component of the displacement vector is applied
        yo = y + this->offsetY;

        // the parity indicator of the tile is calculated along the Y axis
        tyodd = (yo / TILE_HEIGHT) % 2;

        // then the Y component of the reading index in the spritesheet
        index_y = (yo % TILE_HEIGHT) * TILE_WIDTH;

        // the Y component of the square of the distance that
        // separates the pixel from the center of the screen
        // is calculated
        ry2 = (y - hsh) * (y - hsh);

        // scanning of each pixel of the slice (here the X component)
        for (x = 0; x < SCREEN_WIDTH; x++) {

            // the X component of the displacement vector is applied
            xo = x + this->offsetX;

            // the parity indicator of the tile is calculated along the X axis
            txodd = (xo / TILE_WIDTH) % 2;

            // then we determine if we should pick the color code
            // in a light or dark tile
            jump = txodd ^ tyodd;

            // the reading index in the spritesheet can now
            // be fully determined
            index = index_y + (xo % TILE_WIDTH) + (jump * nfo);

            // we calculate the square of the distance between
            // the pixel and the center of the screen
            r2 = (x - hsw) * (x - hsw) + ry2;

            // if the pixel is inside the light halo
            if (!(r2 >> HALO_RADIUS2_POWER_OF_TWO)) {
                // we get the color code of the tile's sprite
                colorIndex = BITMAP[index];

                // if it is not the transparent color
                if (colorIndex--) {
                    // remark on the `--`
                    // if colorIndex is not the index of the transparent color
                    // then colorIndex > 0... so we're going to decrease its value
                    // immediately after reading it to simplify the calculation
                    // which will consist in picking the color code from the
                    // the color palette `COLORMAP`
                    //
                    // the brightness level is calculated at this distance
                    lux = (r2 << BRIGHTNESS_LEVELS_POWER_OF_TWO) >> HALO_RADIUS2_POWER_OF_TWO;

                    // we will then pick the corresponding color code
                    // in the color palette....
                    // > and do not forget to apply the corresponding offset
                    // > of the transparent color, which is the first element
                    // > of `COLORMAP`
                    // and we end by writing this value in the buffer
                    buffer[x + syw] = COLORMAP[1 + (colorIndex << BRIGHTNESS_LEVELS_POWER_OF_TWO) + lux];
                }
            }
        }
    }
}