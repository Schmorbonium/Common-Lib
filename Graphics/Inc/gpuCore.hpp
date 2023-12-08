#ifndef _GPU_CORE
#define _GPU_CORE

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

    ColorFeild(Color color);
    ColorFeild(IQueue *que);
    virtual void parseFromQue(IQueue *que);
    virtual void appendToQue(IQueue *que);
    virtual uint16_t getWireSize();
    Color getColor();
};

/// @brief This class is not intended to be used, so like dont.
class GPUPacket : public Uart_Packet
{
public:
    GPUPacket(GPU_Pkt_Command command);
    GPUPacket(IQueue *que);
    virtual ~GPUPacket();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

class ResponseGPUPkt : public GPUPacket
{
    Uint8Field cmdCnt;

public:
    ResponseGPUPkt(uint8_t cmdCnt);
    ResponseGPUPkt(IQueue *que);
    ~ResponseGPUPkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

// This Packet triggers a reset
class ResetGPUPkt : public GPUPacket
{
    Uint8Field pktCnt;
    Uint8Field resetPhase;

public:
    ResetGPUPkt(uint8_t pktCnt, uint8_t resetPhase);
    ResetGPUPkt(IQueue *que);
    ~ResetGPUPkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

class SetBackground : public GPUPacket
{
    Uint8Field pktCnt;
    ColorFeild setColor;

public:
    SetBackground(uint16_t pktCnt, Color color);
    SetBackground(IQueue *que);
    ~SetBackground();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

class DrawRectPkt : public GPUPacket
{
    Uint8Field pktCnt;
    Uint16Field x;
    Uint16Field y;
    Uint16Field h;
    Uint16Field w;
    Uint16Field fillEnum;
    ColorFeild setColor;

public:
    DrawRectPkt(uint16_t pktCnt, BoundingBox_t box, uint16_t fillType, Color fillColor);
    DrawRectPkt(IQueue *que);
    ~DrawRectPkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

class DrawStrPkt : public GPUPacket
{
    Uint8Field pktCnt;
    Uint16Field x;
    Uint16Field y;
    ColorFeild color;
    StringField str;

public:
    DrawStrPkt(uint16_t pktCnt, uint16_t x, uint16_t y, Color c, char *string, uint16_t strLeng);
    DrawStrPkt(IQueue *que);
    ~DrawStrPkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

class GPU_Channel : public Uart_Channel
{
private:
    GPUPacket *getNextPacket();
public:
    GPU_Channel(IBufferedChannel *Core);
};

#endif