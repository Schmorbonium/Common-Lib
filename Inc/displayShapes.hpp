#ifndef __DispShape__
#define __DispShape__
#include "zHal.h"
#include "lcd.hpp"

// Enum to determine what type of reset is requested
enum ShapeType_Enum : uint16_t
{
    s_enum_rectangle = 0x1,
    s_enum_circle,
    s_enum_triangle
};

class ShapeObj : public IDrawable, ISendable
{
    public:
    Uint16Field shapeType;
    ColorField fillColor;

    ShapeObj(uint16_t shapeType, Color color) : shapeType(shapeType), fillColor(color) {}
    ShapeObj(CharBuffer *que) : shapeType(que), fillColor(que) {}

    virtual void appendShapeDataToQue(CharBuffer *que) {}
    virtual uint16_t GetShapeSize() { return 0; }

    uint16_t getWireSize()
    {
        return shapeType.getWireSize() + fillColor.getWireSize() + GetShapeSize();
    }

    void appendToQue(CharBuffer *que)
    {
        shapeType.appendToQue(que);
        fillColor.appendToQue(que);
        appendShapeDataToQue(que);
    }

    static ShapeObj *ParseFromWire(CharBuffer *que)
    {
        uint16_t type = que->peak_uint16();
        switch (type)
        {
        case s_enum_rectangle:
            return (new RectangleObj(que));
            break;
        case s_enum_circle:
            return (new CircleObj(que));
            break;
        case s_enum_triangle:
            return (new TriangleObj(que));
            break;
        default:
            break;
        }
    }
};

class RectangleObj : public ShapeObj
{
    Uint16Field x, y, w, h;

public:
    RectangleObj(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color color)
        : ShapeObj(s_enum_rectangle, color),
          x(x),
          y(y),
          w(w),
          h(h) {}

    RectangleObj(CharBuffer *que)
        : ShapeObj(que),
          x(que),
          y(que),
          w(que),
          h(que) {}

    virtual void appendShapeDataToQue(CharBuffer *que) {}
    virtual uint16_t GetShapeSize() { return 0; }

    virtual BoundingBox_t GetBoundingBox()
    {
        return BoundingBox_t{x.data, y.data, w.data, h.data};
    }

    virtual void draw(Color *colorBuffer, uint32_t StartIndex, uint32_t RequestSize)
    {
        for (uint16_t i = 0; i < RequestSize; i++)
        {
            colorBuffer[i] = this->fillColor.value;
        }
    }
};

class CircleObj : public ShapeObj
{

    Uint16Field x, y, r;

public:
    CircleObj(uint16_t x, uint16_t y, uint16_t r, Color color) :ShapeObj(s_enum_circle, color), x(x), y(y), r(r) {}
    CircleObj(CharBuffer *que) :ShapeObj(que), x(que), y(que), r(que) {}

    virtual void appendToQue(CharBuffer *que)
    {
        x.appendToQue(que);
        y.appendToQue(que);
        r.appendToQue(que);
    }

    virtual BoundingBox_t GetBoundingBox()
    {
        return BoundingBox_t{(uint16_t)( x.data-r.data),(uint16_t)( y.data-r.data),(uint16_t)(x.data+r.data), (uint16_t)(y.data+r.data)};
    }

    virtual void draw(Color *colorBuffer, uint32_t StartIndex, uint32_t RequestSize)
    {
        for (uint16_t i = 0; i < RequestSize; i++)
        {
            colorBuffer[i] = this->fillColor.value;
        }
    }
};

class TriangleObj : public ShapeObj
{

    Uint16Field x, y, r;

public:
    TriangleObj(uint16_t x, uint16_t y, uint16_t r, Color color) :ShapeObj(s_enum_circle, color), x(x), y(y), r(r) {}
    TriangleObj(CharBuffer *que) :ShapeObj(que), x(que), y(que), r(que) {}

    virtual void appendToQue(CharBuffer *que)
    {
        x.appendToQue(que);
        y.appendToQue(que);
        r.appendToQue(que);
    }

    virtual BoundingBox_t GetBoundingBox()
    {
        return BoundingBox_t{(uint16_t)( x.data-r.data),(uint16_t)( y.data-r.data),(uint16_t)(x.data+r.data), (uint16_t)(y.data+r.data)};
    }

    virtual void draw(Color *colorBuffer, uint32_t StartIndex, uint32_t RequestSize)
    {
        for (uint16_t i = 0; i < RequestSize; i++)
        {
            colorBuffer[i] = this->fillColor.value;
        }
    }
};

#endif