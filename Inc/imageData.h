#include "zHal.h"
#ifndef __IMAGE_DATA
#define __IMAGE_DATA

typedef struct
{
    uint16_t width;
    uint16_t height;
    uint8_t *lut;
    uint8_t *data;
} ImageData;
#endif