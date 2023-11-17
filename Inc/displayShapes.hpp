#ifndef __DispShape__
#define __DispShape__
#include "zHal.h"
#include "lcd.hpp"

class RectangleObj : public IDrawable, ISendable
{

    Uint16Field x, y, w, h;
    ColorField FillColor;

public:
    RectangleObj(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color color) : x(x), y(y), w(w), h(h), FillColor(color) {}
    RectangleObj(CharBuffer *que) : x(que), y(que), w(que), h(que), FillColor(que) {}

    virtual void appendToQue(CharBuffer *que)
    {
        x.appendToQue(que);
        y.appendToQue(que);
        w.appendToQue(que);
        h.appendToQue(que);
        FillColor.appendToQue(que);
    }

    virtual BoundingBox_t GetBoundingBox()
    {
        return BoundingBox_t{x.data, y.data, w.data, h.data};
    }

    virtual void draw(Color *colorBuffer, uint32_t StartIndex, uint32_t RequestSize)
    {
        for (uint16_t i = 0; i < RequestSize; i++)
        {
            colorBuffer[i] = this->FillColor.value; 
        }
    }
};

#endif