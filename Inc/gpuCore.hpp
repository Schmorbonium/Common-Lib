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

class GPU_CHannel : protected BufferedUart
{
    GPU_CHannel(UART_HandleTypeDef *Core) : BufferedUart(Core)
    {
    }

    Command peekCommand()
    {
        return (Command)this->RxQue.peak_uint16();
    }

    bool PacketReady()
    {
        if(RxQue.getSize()>4){
            return false;
        }
        uint16_t packetSize = this->RxQue.peak_uint16(2);
        if(RxQue.getSize() >= packetSize){
            return true;
        }
    }

    GPU_Packet* getNextPacket(){
        return new GPU_Packet(&this->RxQue);
    }

    void SendPacket(GPU_Packet* packetToSend){
        packetToSend->appendToQue(&this->TxQue);
    }
};

class GpuHeader : public PacketField
{
public:
    Uint16Field command;
    GpuHeader(Command data) : command(data) {}
    GpuHeader(CharBuffer *que) : command(que) {}
    virtual void parseFromQue(CharBuffer *que)
    {
        command.parseFromQue(que);
    }
    virtual void appendToQue(CharBuffer *que)
    {
        command.appendToQue(que);
    }
    virtual uint16_t getWireSize() { return command.getWireSize(); }
};
class GPU_Packet : public PacketField
{
public:
    GpuHeader command;
    Uint16Field length;
    PacketField *payload;
    GPU_Packet(Command data, PacketField *payload)
        : command(data),
          length((uint16_t)0),
          payload(payload)
    {
        length.data = command.getWireSize() + length.getWireSize() + payload->getWireSize();
    }
    GPU_Packet(CharBuffer *que) : command(que), length(que) {
        parsePayload(que);
    }

    void parsePayload(CharBuffer *que){
        switch((Command)this->command.command.data)
        {
        case Cmd_FrameID:
            this->payload = new FrameIdPkt(que);
            break;
        case Cmd_LutID:
        case Cmd_SpriteID:
        case Cmd_ResetGpu:
        case Cmd_NewFrame:
        case Cmd_NewLut:
        case Cmd_NewSprite:
        case Cmd_LinkFrame:
        case Cmd_LinkMultiFrame:
        case Cmd_PlaceSprite:
        case Cmd_AnimateSprite:
        case Cmd_MoveSprite:
        case Cmd_LoadFrame:
        case Cmd_LoadLut:
        default:
            this->payload = new PacketField();
            break;
        }
    }

    virtual void parseFromQue(CharBuffer *que)
    {
        command.parseFromQue(que);
        length.parseFromQue(que);
    }

    virtual void appendToQue(CharBuffer *que)
    {
        command.appendToQue(que);
        length.appendToQue(que);
    }

    virtual uint16_t getWireSize()
    {
        return command.getWireSize() +
               length.getWireSize() +
               payload->getWireSize();
    }
};

// Cmd_FrameID = 0x1,
// Size 6 Bytes (Header=4 FrameId=2)
class FrameIdPkt : public PacketField
{
public:
    Uint16Field FrameId;
    FrameIdPkt(uint16_t _FrameId) : FrameId(_FrameId) {}
    FrameIdPkt(CharBuffer *que) : FrameId(que) {}
    virtual void parseFromQue(CharBuffer *que)
    {
        FrameId.parseFromQue(que);
    }
    virtual void appendToQue(CharBuffer *que)
    {
        FrameId.appendToQue(que);
    }
    virtual uint16_t getWireSize() { return FrameId.getWireSize(); }
};
//     Cmd_LutID,
class LutIdPkt : public PacketField
{
public:
    Uint16Field LutId;
    LutIdPkt(uint16_t _LutId) : LutId(_LutId) {}
    LutIdPkt(CharBuffer *que) : LutId(que) {}
    virtual void parseFromQue(CharBuffer *que)
    {
        LutId.parseFromQue(que);
    }
    virtual void appendToQue(CharBuffer *que)
    {
        LutId.appendToQue(que);
    }
    virtual uint16_t getWireSize() { return LutId.getWireSize(); }
};

//     Cmd_SpriteID,
class SpriteIdPkt : public PacketField
{
public:
    Uint16Field spriteId;
    SpriteIdPkt(uint16_t _spriteId) : spriteId(spriteId) {}
    SpriteIdPkt(CharBuffer *que) : spriteId(que) {}
    virtual void parseFromQue(CharBuffer *que)
    {
        spriteId.parseFromQue(que);
    }
    virtual void appendToQue(CharBuffer *que)
    {
        spriteId.appendToQue(que);
    }
    virtual uint16_t getWireSize() { return spriteId.getWireSize(); }
};
//     Cmd_ResetGpu,
class ResetGpuPkt : public PacketField
{
public:
    ResetGpuPkt() {}
    ResetGpuPkt(CharBuffer *que) {}
    virtual void parseFromQue(CharBuffer *que)
    {
    }
    virtual void appendToQue(CharBuffer *que)
    {
    }
};
//     Cmd_NewFrame, NEW_FRAME(W,H)
class NewFramePkt : public PacketField
{
public:
    Uint16Field width;
    Uint16Field height;
    NewFramePkt(uint16_t _width, uint16_t _height) : width(_width), height(_height) {}
    NewFramePkt(CharBuffer *que) : width(que), height(que) {}

    virtual void parseFromQue(CharBuffer *que)
    {
        width.parseFromQue(que);
        height.parseFromQue(que);
    }

    virtual void appendToQue(CharBuffer *que)
    {
        width.appendToQue(que);
        height.appendToQue(que);
    }
};
//     Cmd_NewLut,
class NewLutPkt : public PacketField
{
public:
    NewLutPkt() {}
    NewLutPkt(CharBuffer *que) {}

    virtual void parseFromQue(CharBuffer *que)
    {
    }

    virtual void appendToQue(CharBuffer *que)
    {
    }
};

//     Cmd_NewSprite,
// SPRITE_ID NEW_SPRITE(W,H,FRAME_COUNT) Creates a new SPRITE with given Width Height, and frame depth. (should allow for animation for us nerds)

class NewSpritePkt : public PacketField
{
public:
    Uint16Field width;
    Uint16Field height;
    Uint16Field frameCnt;
    NewSpritePkt(uint16_t w, uint16_t h, uint16_t d)
        : width(w),
          height(h),
          frameCnt(d)
    {
    }
    NewSpritePkt(CharBuffer *que)
        : width(que), height(que), frameCnt(que)
    {
    }

    virtual void parseFromQue(CharBuffer *que)
    {
        width.parseFromQue(que);
        height.parseFromQue(que);
        frameCnt.parseFromQue(que);
    }

    virtual void appendToQue(CharBuffer *que)
    {
        width.appendToQue(que);
        height.appendToQue(que);
        frameCnt.appendToQue(que);
    }
};
// Status   LOAD_FRAME(FRAME_ID,frameDate)
class LoadFramePkt : public PacketField
{
public:
    Uint16Field frameId;
    Uint8ArrayFeild data;
    LoadFramePkt(uint16_t _frameId, uint16_t _dLen, uint8_t *d)
        : frameId(_frameId),
          data(d, _dLen)
    {
    }
    LoadFramePkt(CharBuffer *que)
        : frameId(que),
          data(que)
    {
    }

    virtual void parseFromQue(CharBuffer *que)
    {
        frameId.parseFromQue(que);
    }

    virtual void appendToQue(CharBuffer *que)
    {
        frameId.appendToQue(que);
    }
};
// Status   LOAD_LUT(LUT_ID,LUT_Data)
class LoadLudPkt : public PacketField
{
public:
    Uint16Field lutId;
    Uint8ArrayFeild lutData;
    LoadLudPkt(uint16_t _lutId, uint16_t _dLen, uint8_t *d)
        : lutId(_lutId),
          lutData(d, _dLen)
    {
    }
    LoadLudPkt(CharBuffer *que)
        : lutId(que),
          lutData(que)
    {
    }

    virtual void parseFromQue(CharBuffer *que)
    {
        lutId.parseFromQue(que);
        lutData.parseFromQue(que);
    }

    virtual void appendToQue(CharBuffer *que)
    {
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