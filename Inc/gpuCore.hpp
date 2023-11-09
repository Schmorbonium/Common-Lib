#ifndef _GPU_CORE
#define _GPU_CORE

// There will need to be serval GPU related commands to send over the wire.

// STATUS   RESET_GPU()
// FRAME_ID NEW_FRAME(W,H) Allocate a new section of memory for a image.
// LUT_ID   NEW_LUT() Allocates a new section of memory for a Look Up Table.
// SPRITE_ID NEW_SPRITE(W,H,FRAME_COUNT) Creates a new SPRITE with given Width Height, and frame depth. (should allow for animation for us nerds)

// STATUS   LINK_FRAME(SPRITE_ID,FRAME_ID)
// STATUS   LINK_MULTI_FRAME(SPRITE_ID,FRAME_ID,uint8_t SPRITE_FRAME_INDEX)

// STATUS   PLACE_SPRITE(X,Y)
// STATUS   ANIMATE_SPRITE(SPRITE_ID,AnimationRate) Sets the Sprite to automatically cycle though images. rate is a clock divider so higher is slower
#include "bufferedUart.hpp"
#include "charBuffer.hpp"


class GPU_CHannel : BufferedUart
{
};



#endif