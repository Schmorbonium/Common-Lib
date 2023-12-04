#include "zHal.h"
#include "lcd.hpp"
#include "displayShapes.hpp"
#include "isaacsTypes.hpp"

ShapeObj::ShapeObj(uint16_t shapeType, Color color)
    : shapeType(shapeType),
      ShapeId(ShapeIDManager.allocateID()),
      fillColor(color)
{
    // this->ShapeId.data = ShapeIDManager.allocateID();
}
ShapeObj::ShapeObj(IQueue *que)
    : shapeType(que),
      ShapeId(que),
      fillColor(que) {}

void ShapeObj::appendShapeDataToQue(IQueue *que) {}
uint16_t ShapeObj::GetShapeSize() { return 0; }

uint16_t ShapeObj::getWireSize()
{
    return shapeType.getWireSize() + ShapeId.getWireSize() + fillColor.getWireSize() + GetShapeSize();
}

void ShapeObj::appendToQue(IQueue *que)
{
    shapeType.appendToQue(que);
    ShapeId.appendToQue(que);
    fillColor.appendToQue(que);
    appendShapeDataToQue(que);
}

ShapeObj *ShapeObj::ParseFromWire(IQueue *que)
{
    uint16_t type = que->peak_uint16();
    switch (type)
    {
    case s_enum_rectangle:
        return (ShapeObj *)(new RectangleObj(que));
        break;
    case s_enum_circle:
        return (ShapeObj *)(new CircleObj(que));
        break;
    case s_enum_triangle:
        return (ShapeObj *)(new TriangleObj(que));
        break;
    default:
        break;
    }
    return nullptr;
}

RectangleObj::RectangleObj(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color color)
    : ShapeObj(s_enum_rectangle, color),
      x(x),
      y(y),
      w(w),
      h(h) {}

RectangleObj::RectangleObj(IQueue *que)
    : ShapeObj(que),
      x(que),
      y(que),
      w(que),
      h(que) {}

void RectangleObj::appendShapeDataToQue(IQueue *que) {}
uint16_t RectangleObj::GetShapeSize() { return 0; }
BoundingBox_t RectangleObj::GetBoundingBox()
{
    return BoundingBox_t{x.data, y.data, w.data, h.data};
}

void RectangleObj::draw(Color *colorBuffer, uint32_t StartIndex, uint32_t RequestSize)
{
    for (uint16_t i = 0; i < RequestSize; i++)
    {
        colorBuffer[i] = this->fillColor.value;
    }
}

CircleObj::CircleObj(uint16_t x, uint16_t y, uint16_t r, Color color)
    : ShapeObj(s_enum_circle, color), x(x), y(y), r(r) {}
CircleObj::CircleObj(IQueue *que) : ShapeObj(que), x(que), y(que), r(que) {}

void CircleObj::appendToQue(IQueue *que)
{
    x.appendToQue(que);
    y.appendToQue(que);
    r.appendToQue(que);
}

BoundingBox_t CircleObj::GetBoundingBox()
{
    return BoundingBox_t{(uint16_t)(x.data - r.data), (uint16_t)(y.data - r.data), (uint16_t)(x.data + r.data), (uint16_t)(y.data + r.data)};
}

void CircleObj::draw(Color *colorBuffer, uint32_t StartIndex, uint32_t RequestSize)
{
    for (uint16_t i = 0; i < RequestSize; i++)
    {
        colorBuffer[i] = this->fillColor.value;
    }
}

TriangleObj::TriangleObj(uint16_t x, uint16_t y, uint16_t r, Color color) : ShapeObj(s_enum_circle, color), x(x), y(y), r(r) {}
TriangleObj::TriangleObj(IQueue *que) : ShapeObj(que), x(que), y(que), r(que) {}

void TriangleObj::appendToQue(IQueue *que)
{
    x.appendToQue(que);
    y.appendToQue(que);
    r.appendToQue(que);
}

BoundingBox_t TriangleObj::GetBoundingBox()
{
    return BoundingBox_t{(uint16_t)(x.data - r.data), (uint16_t)(y.data - r.data), (uint16_t)(x.data + r.data), (uint16_t)(y.data + r.data)};
}

void TriangleObj::draw(Color *colorBuffer, uint32_t StartIndex, uint32_t RequestSize)
{
    for (uint16_t i = 0; i < RequestSize; i++)
    {
        colorBuffer[i] = this->fillColor.value;
    }
}