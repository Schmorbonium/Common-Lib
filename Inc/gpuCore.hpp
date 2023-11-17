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
#include "displayShapes.hpp"

typedef uint16_t ShapeID_t;
typedef uint16_t FrameID_t;
typedef uint16_t LutID_t;
typedef uint16_t SpriteID_t;
typedef uint16_t PosX_t;
typedef uint16_t PosY_t;
typedef uint16_t Width_t;
typedef uint16_t Height_t;

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
    Cmd_FillBackGround,
    Cmd_NewShape,
    Cmd_SetShape,
    Cmd_MoveShape,
    Cmd_ResetGpu = 0xA5A5
};

class GPU_Packet : public ISendable, public PacketField
{
public:
    Uint16Field pktId;
    Uint16Field pktLen;

    GPU_Packet(Command command);
    GPU_Packet(CharBuffer *que);
    virtual ~GPU_Packet(){};
    virtual bool actOnPkt();
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    uint16_t getWireSize();
    uint16_t getHeaderWireSize();
    void appendHeader(CharBuffer *que);
    void appendToQue(CharBuffer *que);
};

class GPU_Channel
{
protected:
    BufferedUart ctlUart;

public:
    GPU_Channel(UART_HandleTypeDef *Core);
    Command peekCommand();
    bool PacketReady();
    GPU_Packet *getNextPacket();
    void SendPacket(GPU_Packet *packetToSend);
    bool processNextPacket();
    void UartIRQHandler();
};

// Cmd_FrameID = 0x1,
class FrameIdPkt : public GPU_Packet
{
public:
    Uint16Field FrameId;
    FrameIdPkt(FrameID_t _FrameId);
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
    LutIdPkt(LutID_t LutId);
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
    SpriteIdPkt(SpriteID_t SpriteId);
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
    NewFramePkt(Width_t w, Height_t h);
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
    LoadLutPkt(LutID_t LutId);
    LoadLutPkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_FillBackGround
class FillBackGroundPkt : public GPU_Packet
{
public:
    ColorField fillColor;
    FillBackGroundPkt(Color color);
    FillBackGroundPkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Enum to determine what type of reset is requested
enum ShapeType_Enum : uint16_t
{
    rectangle = 0x1,
    circle,
    triangle
};

// Cmd_NewShape,
class NewShapePkt : public GPU_Packet
{
public:
    NewShapePkt();
    NewShapePkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_SetShape,
class SetShapePkt : public GPU_Packet
{
public:
    SetShapePkt(uint16_t ShapeId, ShapeType_Enum shapeType, uint16_t w, uint16_t h);
    SetShapePkt(CharBuffer *que);
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_MoveShape,
class MoveShapePkt : public GPU_Packet
{
public:
    MoveShapePkt(uint16_t LutId);
    MoveShapePkt(CharBuffer *que);
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