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
#include "gpuCore.hpp"
#include "bufferedUart.hpp"
#include "iQueue.hpp"
#include "uartData.hpp"

// ------------------------------------- Generic packet stuff -------------------------------------
// Cmd_FrameID = 0x1,
FrameIdPkt::FrameIdPkt(uint16_t _FrameId) : GPU_Packet(Cmd_FrameID), FrameId(_FrameId) {}
FrameIdPkt::FrameIdPkt(IQueue *que) : GPU_Packet(que), FrameId(que) {}
FrameIdPkt::~FrameIdPkt() {}
void FrameIdPkt::appendPayload(IQueue *que) { FrameId.appendToQue(que); }
uint16_t FrameIdPkt::getPayloadWireSize() { return FrameId.getWireSize(); }

// Cmd_LutID,
// Cmd_SpriteID,
// Cmd_NewFrame,
// Cmd_NewLut,
// Cmd_NewSprite,
// Cmd_LinkFrame,
// Cmd_LinkMultiFrame,
// Cmd_PlaceSprite,
// Cmd_AnimateSprite,
// Cmd_MoveSprite,
// Cmd_LoadFrame,
// Cmd_LoadLut,
// Cmd_NewShape,
NewShapePkt::NewShapePkt(ShapeObj shapeData)
    : GPU_Packet(Cmd_NewShape),
      shapeData(shapeData) {}
NewShapePkt::NewShapePkt(IQueue *que)
    : GPU_Packet(que),
      shapeData(que) {}
NewShapePkt::~NewShapePkt() {}

void NewShapePkt::appendPayload(IQueue *que)
{
    shapeData.appendToQue(que);
}
uint16_t NewShapePkt::getPayloadWireSize() { return shapeData.getWireSize(); }

// Cmd_SetShape,
SetShapePkt::SetShapePkt(uint16_t ShapeId, ShapeObj shapeData)
    : GPU_Packet(Cmd_SetShape),
      shapeData(shapeData) {}
SetShapePkt::SetShapePkt(IQueue *que)
    : GPU_Packet(que),
      shapeData(que) {}
SetShapePkt::~SetShapePkt() {}
void SetShapePkt::appendPayload(IQueue *que) {}
uint16_t SetShapePkt::getPayloadWireSize() { return shapeData.getWireSize(); }
// Cmd_MoveShape,
MoveShapePkt::MoveShapePkt(uint16_t LutId)
    : GPU_Packet(Cmd_SetShape) {}
MoveShapePkt::MoveShapePkt(IQueue *que)
    : GPU_Packet(que) {}
MoveShapePkt::~MoveShapePkt() {}
void MoveShapePkt::appendPayload(IQueue *que) {}
uint16_t MoveShapePkt::getPayloadWireSize() { return 0; }

FillBackGroundPkt::FillBackGroundPkt(Color color) : GPU_Packet(Cmd_FillBackGround), fillColor(color) {}
FillBackGroundPkt::FillBackGroundPkt(IQueue *que) : GPU_Packet(que), fillColor(que) {}
FillBackGroundPkt::~FillBackGroundPkt() {}
void FillBackGroundPkt::appendPayload(IQueue *que)
{
    fillColor.appendToQue(que);
}
uint16_t FillBackGroundPkt::getPayloadWireSize()
{
    return fillColor.getWireSize();
}

// Cmd_ResetGpu = 0xA5A5
GpuResetPkt::GpuResetPkt(ResetType_Enum ResetType) : GPU_Packet(Cmd_ResetGpu), ResetType(ResetType) {}
GpuResetPkt::GpuResetPkt(IQueue *que) : GPU_Packet(que), ResetType(que) {}
GpuResetPkt::~GpuResetPkt() {}
void GpuResetPkt::appendPayload(IQueue *que) { ResetType.appendToQue(que); }
uint16_t GpuResetPkt::getPayloadWireSize() { return ResetType.getWireSize(); }

// ------------------------------------- GPU Channel -------------------------------------
GPU_Channel::GPU_Channel(UART_HandleTypeDef *Core) : Uart_Channel(Core)
{
}

GPU_Packet *GPU_Channel::getNextPacket()
{
    IQueue *inputQue = &RxQue;
    switch (this->peekCommand())
    {
    case Cmd_FillBackGround:
        return new FillBackGroundPkt(inputQue);
    default:
        break;
    }
    return new GPU_Packet(&RxQue);
}