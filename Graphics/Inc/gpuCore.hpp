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
#include "iQueue.hpp"
#include "uartData.hpp"
#include "uartPacket.hpp"
#include "displayShapes.hpp"

typedef uint16_t ShapeID_t;
typedef uint16_t FrameID_t;
typedef uint16_t LutID_t;
typedef uint16_t SpriteID_t;
typedef uint16_t PosX_t;
typedef uint16_t PosY_t;
typedef uint16_t Width_t;
typedef uint16_t Height_t;

enum GpuCommand : uint16_t
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

class GPU_Packet : public Uart_Packet
{
public:
    GPU_Packet(GpuCommand command) : Uart_Packet(command) {}
    GPU_Packet(IQueue *que) : Uart_Packet(que) {}
    virtual ~GPU_Packet() {}
    virtual bool actOnPkt() { return false; }
    virtual void appendPayload(IQueue *que) {}
    virtual uint16_t getPayloadWireSize() { return 0; }
};

class GPU_Channel : public Uart_Channel
{
private:
    GPU_Packet *getNextPacket();

public:
    GPU_Channel(UART_HandleTypeDef *Core);
};

// Cmd_FrameID = 0x1,
class FrameIdPkt : public GPU_Packet
{
public:
    Uint16Field FrameId;
    FrameIdPkt(FrameID_t _FrameId);
    FrameIdPkt(IQueue *que);
    ~FrameIdPkt();
    virtual void appendPayload(IQueue *que);
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
    LutIdPkt(IQueue *que);
    ~LutIdPkt();
    virtual void appendPayload(IQueue *que);
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
    SpriteIdPkt(IQueue *que);
    ~SpriteIdPkt();
    virtual void appendPayload(IQueue *que);
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
    NewFramePkt(IQueue *que);
    ~NewFramePkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_NewLut,
class NewLutPkt : public GPU_Packet
{
public:
    ~NewLutPkt();
    NewLutPkt();
    NewLutPkt(IQueue *que);
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_NewSprite,
class NewSpritePkt : public GPU_Packet
{
public:
    ~NewSpritePkt();
    NewSpritePkt();
    NewSpritePkt(IQueue *que);
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};

// Cmd_LinkFrame,
class LinkFramePkt : public GPU_Packet
{
public:
    ~LinkFramePkt();
    LinkFramePkt();
    LinkFramePkt(IQueue *que);
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_LinkMultiFrame,
class LinkMultiFramePkt : public GPU_Packet
{
public:
    LinkMultiFramePkt();
    LinkMultiFramePkt(IQueue *que);
    ~LinkMultiFramePkt();
    virtual void appendPayload(IQueue *que);
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
    PlaceSpritePkt(IQueue *que);
    ~PlaceSpritePkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_AnimateSprite,
class AnimateSpritePkt : public GPU_Packet
{
public:
    AnimateSpritePkt();
    AnimateSpritePkt(IQueue *que);
    ~AnimateSpritePkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};

// Cmd_MoveSprite,
class MoveSpritePkt : public GPU_Packet
{
public:
    MoveSpritePkt();
    MoveSpritePkt(IQueue *que);
    ~MoveSpritePkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_LoadFrame,
class LoadFramePkt : public GPU_Packet
{
public:
    LoadFramePkt();
    LoadFramePkt(IQueue *que);
    ~LoadFramePkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};

// Cmd_LoadLut,
class LoadLutPkt : public GPU_Packet
{
public:
    LoadLutPkt(LutID_t LutId);
    LoadLutPkt(IQueue *que);
    ~LoadLutPkt();
    virtual void appendPayload(IQueue *que);
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
    FillBackGroundPkt(IQueue *que);
    ~FillBackGroundPkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};

// Cmd_NewShape,
class NewShapePkt : public GPU_Packet
{
    ShapeObj shapeData;

public:
    NewShapePkt(ShapeObj shapeData);
    NewShapePkt(IQueue *que);
    ~NewShapePkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_SetShape,
class SetShapePkt : public GPU_Packet
{
    ShapeObj shapeData;

public:
    SetShapePkt(uint16_t ShapeId, ShapeObj shapeData);
    SetShapePkt(IQueue *que);
    ~SetShapePkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    // Implement in client and server
    virtual bool actOnPkt();
};
// Cmd_MoveShape,
class MoveShapePkt : public GPU_Packet
{
public:
    MoveShapePkt(uint16_t LutId);
    MoveShapePkt(IQueue *que);
    ~MoveShapePkt();
    virtual void appendPayload(IQueue *que);
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
    GpuResetPkt(IQueue *que);
    ~GpuResetPkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

#endif