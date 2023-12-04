#ifndef __DispShape__
#define __DispShape__
#include "zHal.h"
#include "iQueue.hpp"
#include "lcd.hpp"
#include "uartData.hpp"
#include "isaacsTypes.hpp"


// Enum to determine what type of reset is requested
enum ShapeType_Enum : uint16_t
{
    s_enum_rectangle = 0x1,
    s_enum_circle,
    s_enum_triangle
};


class ShapeObj : public IDrawable, ISendable
{
private:
static IDManager ShapeIDManager;
protected:
    virtual void appendShapeDataToQue(IQueue *que);
    virtual uint16_t GetShapeSize();
public:
    Uint16Field shapeType;
    Uint16Field ShapeId;
    ColorField fillColor;

    ShapeObj(uint16_t shapeType, Color color);
    ShapeObj(IQueue *que);


    uint16_t getWireSize();
    void appendToQue(IQueue *que);
    static ShapeObj *ParseFromWire(IQueue *que);
};

class RectangleObj : public ShapeObj
{
    Uint16Field x, y, w, h;

public:
    RectangleObj(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color color);

    RectangleObj(IQueue *que);

    virtual void appendShapeDataToQue(IQueue *que);
    virtual uint16_t GetShapeSize();

    virtual BoundingBox_t GetBoundingBox();
    virtual void draw(Color *colorBuffer, uint32_t StartIndex, uint32_t RequestSize);
};

class CircleObj : public ShapeObj
{

    Uint16Field x, y, r;

public:
    CircleObj(uint16_t x, uint16_t y, uint16_t r, Color color);
    CircleObj(IQueue *que);

    virtual void appendToQue(IQueue *que);
    virtual BoundingBox_t GetBoundingBox();
    virtual void draw(Color *colorBuffer, uint32_t StartIndex, uint32_t RequestSize);
};

class TriangleObj : public ShapeObj
{

    Uint16Field x, y, r;

public:
    TriangleObj(uint16_t x, uint16_t y, uint16_t r, Color color);
    TriangleObj(IQueue *que);

    virtual void appendToQue(IQueue *que);
    virtual BoundingBox_t GetBoundingBox();
    virtual void draw(Color *colorBuffer, uint32_t StartIndex, uint32_t RequestSize);
};

#endif