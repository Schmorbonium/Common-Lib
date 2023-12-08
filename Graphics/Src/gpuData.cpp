#ifndef __GPU_DATA_
#define __GPU_DATA_

#include "uartData.hpp"
#include "uartPacket.hpp"
#include "lcd.hpp"

//  Okay we are always sending a list of commands, they will be compiled into a super class so I dont have to worry about it.

enum GPU_Pkt_Command : uint16_t
{
    GPU_RESET_PKT_ENUM = 0x1,
    GPU_SET_BACKGROUND,
    GPU_PLACE_RECTANGLE,
    GPU_PLACE_TEXT,
    GPU_RESPONSE
};

class ColorFeild : public PacketField
{
public:
    Uint8Field r;
    Uint8Field g;
    Uint8Field b;

    ColorFeild(Color color)
        : r(color.R),
          g(color.G),
          b(color.B)
    {
    }
    ColorFeild(IQueue *que)
        : r(que),
          g(que),
          b(que)
    {
    }
    virtual void parseFromQue(IQueue *que)
    {
        r.parseFromQue(que);
        g.parseFromQue(que);
        b.parseFromQue(que);
    }
    virtual void appendToQue(IQueue *que)
    {
        r.appendToQue(que);
        g.appendToQue(que);
        b.appendToQue(que);
    }
    virtual uint16_t getWireSize()
    {
        return r.getWireSize() +
               g.getWireSize() +
               b.getWireSize();
    }
    Color getColor()
    {
        Color c;
        c.R = this->r.data;
        c.G = this->g.data;
        c.B = this->b.data;
        return c;
    }
};

/// @brief This class is not intended to be used, so like dont.
class GPUPacket : public Uart_Packet
{
public:
    GPUPacket(GPU_Pkt_Command command) : Uart_Packet(command) {}
    GPUPacket(IQueue *que) : Uart_Packet(que) {}
    virtual ~GPUPacket() {}
    virtual void appendPayload(IQueue *que) { return; }
    virtual uint16_t getPayloadWireSize() { return 0; }
    virtual bool actOnPkt(){return true;};
};

class ResponseGPUPkt : public GPUPacket
{
    Uint8Field cmdCnt;

public:
    ResponseGPUPkt(uint8_t cmdCnt)
        : GPUPacket(GPU_RESET_PKT_ENUM),
          cmdCnt(cmdCnt)
    {
    }
    ResponseGPUPkt(IQueue *que)
        : GPUPacket(que),
          cmdCnt(que)
    {
    }
    ~ResponseGPUPkt() {}
    virtual void appendPayload(IQueue *que) { cmdCnt.appendToQue(que); }
    virtual uint16_t getPayloadWireSize() { return cmdCnt.getWireSize(); }
    virtual bool actOnPkt() {return true;}
};

// This Packet triggers a reset
class ResetGPUPkt : public GPUPacket
{
    Uint8Field resetPhase;

public:
    ResetGPUPkt(uint8_t resetPhase)
        : GPUPacket(GPU_RESET_PKT_ENUM),
          resetPhase(resetPhase)
    {
    }
    ResetGPUPkt(IQueue *que)
        : GPUPacket(que),
          resetPhase(que)
    {
    }
    ~ResetGPUPkt() {}
    virtual void appendPayload(IQueue *que) { resetPhase.appendToQue(que); }
    virtual uint16_t getPayloadWireSize() { return resetPhase.getWireSize(); }
    virtual bool actOnPkt();
};

class SetBackground : public GPUPacket
{
    ColorFeild setColor;

public:
    SetBackground(Color color)
        : GPUPacket(GPU_SET_BACKGROUND),
          setColor(color)
    {
    }
    SetBackground(IQueue *que)
        : GPUPacket(que),
          setColor(que)
    {
    }
    ~SetBackground();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

class DrawRectPkt : public GPUPacket
{
    Uint16Field x;
    Uint16Field y;
    Uint16Field h;
    Uint16Field w;
    Uint16Field fillEnum;
    ColorFeild setColor;

public:
    DrawRectPkt(BoundingBox_t box, uint16_t fillType, Color fillColor)
        : GPUPacket(GPU_PLACE_RECTANGLE),
          x(box.x),
          y(box.y),
          h(box.h),
          w(box.w),
          fillEnum(fillType),
          setColor(fillColor)
    {
    }
    DrawRectPkt(IQueue *que)
        : GPUPacket(que),
          x(que),
          y(que),
          h(que),
          w(que),
          fillEnum(que),
          setColor(que)
    {
    }
    ~DrawRectPkt() {}
    virtual void appendPayload(IQueue *que)
    {
        x.appendToQue(que);
        y.appendToQue(que);
        h.appendToQue(que);
        w.appendToQue(que);
        fillEnum.appendToQue(que);
        setColor.appendToQue(que);
    }
    virtual uint16_t getPayloadWireSize()
    {
        return x.getWireSize() +
               y.getWireSize() +
               h.getWireSize() +
               w.getWireSize() +
               fillEnum.getWireSize() +
               setColor.getWireSize();
    }
    virtual bool actOnPkt();
};

class DrawStrPkt : public GPUPacket
{
    Uint16Field x;
    Uint16Field y;
    ColorFeild color;
    StringField str;

public:
    DrawStrPkt(uint16_t x, uint16_t y, Color c, char *string, uint16_t strLeng)
        : GPUPacket(GPU_PLACE_TEXT),
          x(x),
          y(y),
          color(c),
          str(string, strLeng)
    {
    }

    DrawStrPkt(IQueue *que)
        : GPUPacket(que),
          x(que),
          y(que),
          color(que),
          str(que) {}
    ~DrawStrPkt() {}
    virtual void appendPayload(IQueue *que)
    {
        x.appendToQue(que);
        y.appendToQue(que);
        color.appendToQue(que);
        str.appendToQue(que);
    }
    virtual uint16_t getPayloadWireSize()
    {
        return x.getWireSize() +
               y.getWireSize() +
               color.getWireSize() +
               str.getWireSize();
    }
    virtual bool actOnPkt();
};

class GPU_Channel : public Uart_Channel
{
private:
    GPUPacket *getNextPacket()
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
    }

public:
    GPU_Channel(IBufferedChannel *Core) : Uart_Channel(Core) {}
};

#endif