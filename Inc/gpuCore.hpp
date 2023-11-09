#ifndef _GPU_CORE
#define _GPU_CORE

// There will need to be serval GPU related commands to send over the wire.

// STATUS   RESET_GPU()
// FRAME_ID NEW_FRAME(W,H) Allocate a new section of memory for a image.
// LUT_ID   NEW_LUT() Allocates a new section of memory for a Look Up Table.
// SPRITE_ID NEW_SPRITE(W,H,FRAME_COUNT) Creates a new SPRITE with given Width Height, and frame depth. (should allow for animation for us nerds)

// Status   LOAD_FRAME(FRAME_ID,frameDate)
// Status   LOAD_LUT(LUT_ID,LUT_Data)

// STATUS   LINK_FRAME(SPRITE_ID,FRAME_ID)
// STATUS   LINK_MULTI_FRAME(SPRITE_ID,FRAME_ID,uint8_t SPRITE_FRAME_INDEX)

// STATUS   PLACE_SPRITE(X,Y)
// STATUS   ANIMATE_SPRITE(SPRITE_ID,AnimationRate) Sets the Sprite to automatically cycle though images. rate is a clock divider so higher is slower
#include "bufferedUart.hpp"
#include "charBuffer.hpp"
#include "uartData.hpp"

enum Command : uint16_t
{
    Cmd_FrameID = 0x1,
    Cmd_LutID,
    Cmd_SpriteID,
    Cmd_ResetGpu,
    Cmd_NewFrame,
    Cmd_NewLut,
    Cmd_NewSprite,
    Cmd_LinkFrame,
    Cmd_LinkMultiFrame,
    Cmd_PlaceSprite,
    Cmd_AnimateSprite,
    Cmd_MoveSprite,
    Cmd_LoadFrame,
    Cmd_LoadLut
};

class GPU_CHannel : BufferedUart
{
    GPU_CHannel(UART_HandleTypeDef *Core) : BufferedUart(Core)
    {
    }

    Command peekCommand()
    {
        return (Command)this->RxQue.peak_uint16();
    }
};

class GpuHeader : PacketField
{
public:
    Uint16Field command;
    GpuHeader(uint16_t data) : PacketField(), command(data) {}
    GpuHeader(CharBuffer *que) : command(que) {}
    virtual void parseFromQue(CharBuffer *que) { command.parseFromQue(que); }
    virtual void appendToQue(CharBuffer *que) { command.appendToQue(que); }
};

// Cmd_FrameID = 0x1,
class FrameIdPkt : PacketField
{
public:
    GpuHeader header;
    Uint16Field FrameId;
    FrameIdPkt(uint16_t _FrameId) : header((uint16_t)Cmd_FrameID), FrameId(_FrameId) {}
    FrameIdPkt(CharBuffer *que) : header(que), FrameId(que) {}
    virtual void parseFromQue(CharBuffer *que)
    {
        header.parseFromQue(que);
        FrameId.parseFromQue(que);
    }
    virtual void appendToQue(CharBuffer *que)
    {
        header.appendToQue(que);
        FrameId.appendToQue(que);
    }
};
//     Cmd_LutID,
class LutIdPkt : PacketField
{
public:
    GpuHeader header;
    Uint16Field LutId;
    LutIdPkt(uint16_t _LutId) : header((uint16_t)Cmd_LutID), LutId(_LutId) {}
    LutIdPkt(CharBuffer *que) : header(que), LutId(que) {}
    virtual void parseFromQue(CharBuffer *que)
    {
        header.parseFromQue(que);
        LutId.parseFromQue(que);
    }
    virtual void appendToQue(CharBuffer *que)
    {
        header.appendToQue(que);
        LutId.appendToQue(que);
    }
};

//     Cmd_SpriteID,
class SpriteIdPkt : PacketField
{
public:
    GpuHeader header;
    Uint16Field spriteId;
    SpriteIdPkt(uint16_t _spriteId) : header((uint16_t)Cmd_SpriteID), spriteId(spriteId) {}
    SpriteIdPkt(CharBuffer *que) : header(que), spriteId(que) {}
    virtual void parseFromQue(CharBuffer *que)
    {
        header.parseFromQue(que);
        spriteId.parseFromQue(que);
    }
    virtual void appendToQue(CharBuffer *que)
    {
        header.appendToQue(que);
        spriteId.appendToQue(que);
    }
};
//     Cmd_ResetGpu,
class ResetGpuPkt : PacketField
{
public:
    GpuHeader header;
    ResetGpuPkt() : header((uint16_t)Cmd_ResetGpu) {}
    ResetGpuPkt(CharBuffer *que) : header(que) {}
    virtual void parseFromQue(CharBuffer *que)
    {
        header.parseFromQue(que);
    }
    virtual void appendToQue(CharBuffer *que)
    {
        header.appendToQue(que);
    }
};
//     Cmd_NewFrame, NEW_FRAME(W,H)
class NewFramePkt : PacketField
{
public:
    GpuHeader header;
    Uint16Field width;
    Uint16Field height;
    NewFramePkt(uint16_t _width, uint16_t _height) : header((uint16_t)Cmd_NewFrame), width(_width), height(_height) {}
    NewFramePkt(CharBuffer *que) : header(que), width(que), height(que) {}

    virtual void parseFromQue(CharBuffer *que)
    {
        header.parseFromQue(que);
        width.parseFromQue(que);
        height.parseFromQue(que);
    }

    virtual void appendToQue(CharBuffer *que)
    {
        header.appendToQue(que);
        width.appendToQue(que);
        height.appendToQue(que);
    }
};
//     Cmd_NewLut,
class NewLutPkt : PacketField
{
public:
    GpuHeader header;
    NewLutPkt() : header((uint16_t)Cmd_NewFrame) {}
    NewLutPkt(CharBuffer *que) : header(que) {}

    virtual void parseFromQue(CharBuffer *que)
    {
        header.parseFromQue(que);
    }

    virtual void appendToQue(CharBuffer *que)
    {
        header.appendToQue(que);
    }
};

//     Cmd_NewSprite,
// SPRITE_ID NEW_SPRITE(W,H,FRAME_COUNT) Creates a new SPRITE with given Width Height, and frame depth. (should allow for animation for us nerds)

class NewSpritePkt : PacketField
{
public:
    GpuHeader header;
    Uint16Field width;
    Uint16Field height;
    Uint16Field frameCnt;
    NewSpritePkt(uint16_t w, uint16_t h, uint16_t d)
        : header((uint16_t)Cmd_NewFrame),
          width(w),
          height(h),
          frameCnt(d)
    {
    }
    NewSpritePkt(CharBuffer *que)
        : header(que), width(que), height(que), frameCnt(que)
    {
    }

    virtual void parseFromQue(CharBuffer *que)
    {
        header.parseFromQue(que);
        width.parseFromQue(que);
        height.parseFromQue(que);
        frameCnt.parseFromQue(que);
    }

    virtual void appendToQue(CharBuffer *que)
    {
        header.appendToQue(que);
        width.appendToQue(que);
        height.appendToQue(que);
        frameCnt.appendToQue(que);
    }
};
// Status   LOAD_FRAME(FRAME_ID,frameDate)
class LoadFramePkt : PacketField
{
public:
    GpuHeader header;
    Uint16Field frameId;
    Uint8ArrayFeild data;
    LoadFramePkt(uint16_t _frameId, uint16_t _dLen, uint8_t *d)
        : header((uint16_t)Cmd_NewFrame),
          frameId(_frameId),
          data(d, _dLen)
    {
    }
    LoadFramePkt(CharBuffer *que)
        : header(que),
          frameId(que),
          data(que)
    {
    }

    virtual void parseFromQue(CharBuffer *que)
    {
        header.parseFromQue(que);
        frameId.parseFromQue(que);
    }

    virtual void appendToQue(CharBuffer *que)
    {
        header.appendToQue(que);
        frameId.appendToQue(que);
    }
};
// Status   LOAD_LUT(LUT_ID,LUT_Data)
class LoadLudPkt : PacketField
{
public:
    GpuHeader header;
    Uint16Field lutId;
    Uint8ArrayFeild lutData;
    LoadLudPkt(uint16_t _lutId, uint16_t _dLen, uint8_t *d)
        : header((uint16_t)Cmd_NewFrame),
          lutId(_lutId),
          lutData(d, _dLen)
    {
    }
    LoadLudPkt(CharBuffer *que)
        : header(que),
          lutId(que),
          lutData(que)
    {
    }

    virtual void parseFromQue(CharBuffer *que)
    {
        header.parseFromQue(que);
        lutId.parseFromQue(que);
        lutData.parseFromQue(que);
    }

    virtual void appendToQue(CharBuffer *que)
    {
        header.appendToQue(que);
        lutId.appendToQue(que);
        lutData.appendToQue(que);
    }
};
//     Cmd_LinkFrame,
//     Cmd_LinkMultiFrame,
//     Cmd_PlaceSprite,
//     Cmd_AnimateSprite,
//     Cmd_MoveSprite
#endif