#include "gpuCore.hpp"

ColorFeild::ColorFeild(Color color)
    : r(color.R),
      g(color.G),
      b(color.B)
{
}
ColorFeild::ColorFeild(IQueue *que)
    : r(que),
      g(que),
      b(que)
{
}
void ColorFeild::parseFromQue(IQueue *que)
{
    r.parseFromQue(que);
    g.parseFromQue(que);
    b.parseFromQue(que);
}
void ColorFeild::appendToQue(IQueue *que)
{
    r.appendToQue(que);
    g.appendToQue(que);
    b.appendToQue(que);
}
uint16_t ColorFeild::getWireSize()
{
    return r.getWireSize() +
           g.getWireSize() +
           b.getWireSize();
}
Color ColorFeild::getColor()
{
    Color c;
    c.R = this->r.data;
    c.G = this->g.data;
    c.B = this->b.data;
    return c;
}

GPUPacket::GPUPacket(GPU_Pkt_Command command) : Uart_Packet(command) {}
GPUPacket::GPUPacket(IQueue *que) : Uart_Packet(que) {}
GPUPacket::~GPUPacket() {}
void GPUPacket::appendPayload(IQueue *que) { return; }
uint16_t GPUPacket::getPayloadWireSize() { return 0; }
bool GPUPacket::actOnPkt() { return false; };

ResponseGPUPkt::ResponseGPUPkt(uint8_t cmdCnt)
    : GPUPacket(GPU_RESET_PKT_ENUM),
      cmdCnt(cmdCnt)
{
}
ResponseGPUPkt::ResponseGPUPkt(IQueue *que)
    : GPUPacket(que),
      cmdCnt(que)
{
}
ResponseGPUPkt::~ResponseGPUPkt() {}
void ResponseGPUPkt::appendPayload(IQueue *que) { cmdCnt.appendToQue(que); }
uint16_t ResponseGPUPkt::getPayloadWireSize() { return cmdCnt.getWireSize(); }

// This Packet triggers a reset
ResetGPUPkt::ResetGPUPkt(uint8_t pktCnt, uint8_t resetPhase)
    : GPUPacket(GPU_RESET_PKT_ENUM),
      pktCnt(pktCnt),
      resetPhase(resetPhase)
{
}
ResetGPUPkt::ResetGPUPkt(IQueue *que)
    : GPUPacket(que),
      pktCnt(que),
      resetPhase(que)
{
}
ResetGPUPkt::~ResetGPUPkt() {}
void ResetGPUPkt::appendPayload(IQueue *que) { resetPhase.appendToQue(que); }
uint16_t ResetGPUPkt::getPayloadWireSize() { return resetPhase.getWireSize() + pktCnt.getWireSize(); }

SetBackground::SetBackground(uint16_t pktCnt, Color color)
    : GPUPacket(GPU_SET_BACKGROUND),
      pktCnt(pktCnt),
      setColor(color)
{
}
SetBackground::SetBackground(IQueue *que)
    : GPUPacket(que),
      pktCnt(que),
      setColor(que)
{
}
SetBackground::~SetBackground() {}
void SetBackground::appendPayload(IQueue *que)
{
    pktCnt.appendToQue(que);
    setColor.appendToQue(que);
}
uint16_t SetBackground::getPayloadWireSize()
{
    return pktCnt.getWireSize() +
           setColor.getWireSize();
}

DrawRectPkt::DrawRectPkt(uint16_t pktCnt, BoundingBox_t box, uint16_t fillType, Color fillColor)
    : GPUPacket(GPU_PLACE_RECTANGLE),
      pktCnt(pktCnt),
      x(box.x),
      y(box.y),
      h(box.h),
      w(box.w),
      fillEnum(fillType),
      setColor(fillColor)
{
}
DrawRectPkt::DrawRectPkt(IQueue *que)
    : GPUPacket(que),
      pktCnt(que),
      x(que),
      y(que),
      h(que),
      w(que),
      fillEnum(que),
      setColor(que)
{
}
DrawRectPkt::~DrawRectPkt() {}
void DrawRectPkt::appendPayload(IQueue *que)
{
    pktCnt.appendToQue(que);
    x.appendToQue(que);
    y.appendToQue(que);
    h.appendToQue(que);
    w.appendToQue(que);
    fillEnum.appendToQue(que);
    setColor.appendToQue(que);
}
uint16_t DrawRectPkt::getPayloadWireSize()
{
    return pktCnt.getWireSize() +
           x.getWireSize() +
           y.getWireSize() +
           h.getWireSize() +
           w.getWireSize() +
           fillEnum.getWireSize() +
           setColor.getWireSize();
}

DrawStrPkt::DrawStrPkt(uint16_t pktCnt, uint16_t x, uint16_t y, Color c, char *string, uint16_t strLeng)
    : GPUPacket(GPU_PLACE_TEXT),
      pktCnt(pktCnt),
      x(x),
      y(y),
      color(c),
      str(string, strLeng)
{
}

DrawStrPkt::DrawStrPkt(IQueue *que)
    : GPUPacket(que),
      pktCnt(que),
      x(que),
      y(que),
      color(que),
      str(que) {}
DrawStrPkt::~DrawStrPkt() {}
void DrawStrPkt::appendPayload(IQueue *que)
{
    pktCnt.appendToQue(que);
    x.appendToQue(que);
    y.appendToQue(que);
    color.appendToQue(que);
    str.appendToQue(que);
}
uint16_t DrawStrPkt::getPayloadWireSize()
{
    return pktCnt.getWireSize() +
           x.getWireSize() +
           y.getWireSize() +
           color.getWireSize() +
           str.getWireSize();
}

GPUPacket *GPU_Channel::getNextPacket()
{
    GPU_Pkt_Command cmd = (GPU_Pkt_Command)this->peekCommand();
    switch (cmd)
    {
    case GPU_RESET_PKT_ENUM:
        return new ResetGPUPkt(&this->channel->RxQue);
        break;
    case GPU_SET_BACKGROUND:
        return new SetBackground(&this->channel->RxQue);
        break;
    case GPU_PLACE_RECTANGLE:
        return new DrawRectPkt(&this->channel->RxQue);
        break;
    case GPU_PLACE_TEXT:
        return new DrawStrPkt(&this->channel->RxQue);
        break;
    case GPU_RESPONSE:
        return new ResponseGPUPkt(&this->channel->RxQue);
        break;
    default:
        break;
    }

    return new GPUPacket(&this->channel->RxQue);
}

GPU_Channel::GPU_Channel(IBufferedChannel *Core) : Uart_Channel(Core) {}






// To Implement in the Client and server respetivly
// bool ResetGPUPkt::actOnPkt();
// bool SetBackground::actOnPkt();
// bool DrawRectPkt::actOnPkt();
// bool DrawStrPkt::actOnPkt();
// bool ResponseGPUPkt::actOnPkt();