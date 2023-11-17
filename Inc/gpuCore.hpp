#ifndef _GPU_CORE
#define _GPU_CORE

// There will need to be serval GPU related commands to send over the wire.

// STATUS   RESET_GPU()
// FRAME_ID NEW_FRAME(W,H) Allocate a new section of memory for a image.
// LUT_ID   NEW_LUT() Allocates a new section of memory for a Look Up Table.
// SPRITE_ID NEW_SPRITE() (should allow for animation for us nerds)

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
    Cmd_NewFrame,
    Cmd_NewLut,
    Cmd_NewSprite,
    Cmd_LinkFrame,
    Cmd_LinkMultiFrame,
    Cmd_PlaceSprite,
    Cmd_AnimateSprite,
    Cmd_MoveSprite,
    Cmd_LoadFrame,
    Cmd_LoadLut,
    Cmd_ResetGpu = 0xA5A5
};

class GPU_Packet : public PacketField
{
public:
    Uint16Field pktId;
    Uint16Field pktLen;

    GPU_Packet(Command command) : pktId((uint16_t)command), pktLen((uint16_t)0)
    {
    }

    GPU_Packet(CharBuffer *que) : pktId(que), pktLen(que)
    {
    }

    virtual bool actOnPkt() {}
    virtual void appendPayload(CharBuffer *que) {}
    virtual uint16_t getPayloadWireSize() { return 0; }

    uint16_t getWireSize()
    {
        return getHeaderWireSize() + getPayloadWireSize();
    }

    uint16_t getHeaderWireSize()
    {
        return pktId.getWireSize() + pktLen.getWireSize();
    }

    void appendHeader(CharBuffer *que)
    {
        this->pktId.appendToQue(que);
        this->pktLen.data = this->getWireSize();
        this->pktLen.appendToQue(que);
    }

    void appendToQue(CharBuffer *que)
    {
        this->appendHeader(que);
        this->appendPayload(que);
    }
};

class GPU_Channel : protected BufferedUart
{
public:
    GPU_Channel(UART_HandleTypeDef *Core);
    Command peekCommand();
    bool PacketReady();
    GPU_Packet *getNextPacket();
    void SendPacket(GPU_Packet *packetToSend);
};

// Cmd_FrameID = 0x1,
class FrameIdPkt : public GPU_Packet
{
public:
    Uint16Field FrameId;
    FrameIdPkt(uint16_t _FrameId);
    FrameIdPkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};

// Cmd_LutID,
class LutIdPkt : public GPU_Packet
{
public:
    Uint16Field LutId;
    LutIdPkt(uint16_t LutId);
    LutIdPkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};

// Cmd_SpriteID,
class SpriteIdPkt : public GPU_Packet
{
public:
    Uint16Field SpriteId;
    SpriteIdPkt(uint16_t SpriteId);
    SpriteIdPkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};

// Cmd_NewFrame,
class NewFramePkt : public GPU_Packet
{
public:
    Uint16Field W;
    Uint16Field H;
    NewFramePkt(uint16_t w, uint16_t h);
    NewFramePkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_NewLut,
class NewLutPkt : public GPU_Packet
{
public:
    NewLutPkt();
    NewLutPkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_NewSprite,
class NewSpritePkt : public GPU_Packet
{
public:
    NewSpritePkt();
    NewSpritePkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};

// Cmd_LinkFrame,
class LinkFramePkt : public GPU_Packet
{
public:
    LinkFramePkt();
    LinkFramePkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_LinkMultiFrame,
class LinkMultiFramePkt : public GPU_Packet
{
public:
    LinkMultiFramePkt();
    LinkMultiFramePkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_PlaceSprite,
class PlaceSpritePkt : public GPU_Packet
{
public:
    Uint16Field LutId;
    PlaceSpritePkt(uint16_t LutId);
    PlaceSpritePkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_AnimateSprite,
class AnimateSpritePkt : public GPU_Packet
{
public:
    AnimateSpritePkt();
    AnimateSpritePkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};

// Cmd_MoveSprite,
class MoveSpritePkt : public GPU_Packet
{
public:
    MoveSpritePkt();
    MoveSpritePkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_LoadFrame,
class LoadFramePkt : public GPU_Packet
{
public:
    LoadFramePkt();
    LoadFramePkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};

// Cmd_LoadLut,
class LoadLutPkt : public GPU_Packet
{
public:
    LoadLutPkt(uint16_t LutId);
    LoadLutPkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};


// Enum to determine what type of reset is requested
enum ResetType_Enum : uint16_t
{
    RST_init,
    RST_full,
    RST_ClearScreen
};
// Cmd_ResetGpu = 0xA5A5
class GpuResetPkt : public GPU_Packet
{
public:
    Uint16Field ResetType;
    GpuResetPkt(ResetType_Enum ResetType);
    GpuResetPkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

#endif