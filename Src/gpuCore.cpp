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
#include "charBuffer.hpp"
#include "uartData.hpp"



// ------------------------------------- Core Packet Class -------------------------------------
GPU_Packet::GPU_Packet(Command command) : pktId((uint16_t)command), pktLen((uint16_t)0)
{
}

GPU_Packet::GPU_Packet(CharBuffer *que) : pktId(que), pktLen(que)
{
}

bool GPU_Packet::actOnPkt() {}
void GPU_Packet::appendPayload(CharBuffer *que) {}
uint16_t GPU_Packet::getPayloadWireSize() { return 0; }

uint16_t GPU_Packet::getWireSize()
{
    return getHeaderWireSize() + getPayloadWireSize();
}

uint16_t GPU_Packet::getHeaderWireSize()
{
    return pktId.getWireSize() + pktLen.getWireSize();
}

void GPU_Packet::appendHeader(CharBuffer *que)
{
    this->pktId.appendToQue(que);
    this->pktLen.data = this->getWireSize();
    this->pktLen.appendToQue(que);
}

void GPU_Packet::appendToQue(CharBuffer *que)
{
    this->appendHeader(que);
    this->appendPayload(que);
}



// ------------------------------------- Generic packet stuff -------------------------------------
// Cmd_FrameID = 0x1,
FrameIdPkt::FrameIdPkt(uint16_t _FrameId) : GPU_Packet(Cmd_FrameID), FrameId(_FrameId) {}
FrameIdPkt::FrameIdPkt(CharBuffer *que) : GPU_Packet(que), FrameId(que) {}
void FrameIdPkt::appendPayload(CharBuffer *que) { FrameId.appendToQue(que); }
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

// Cmd_ResetGpu = 0xA5A5
GpuResetPkt::GpuResetPkt(ResetType_Enum ResetType) : GPU_Packet(Cmd_ResetGpu), ResetType(ResetType) {}
GpuResetPkt::GpuResetPkt(CharBuffer *que) : GPU_Packet(que), ResetType(que) {}
void GpuResetPkt::appendPayload(CharBuffer *que) { ResetType.appendToQue(que); }
uint16_t GpuResetPkt::getPayloadWireSize() { return ResetType.getWireSize(); }



// ------------------------------------- GPU Channel -------------------------------------
GPU_Channel::GPU_Channel(UART_HandleTypeDef *Core) : BufferedUart(Core)
{
}

Command GPU_Channel::peekCommand()
{
    return (Command)this->RxQue.peak_uint16();
}

bool GPU_Channel::PacketReady()
{
    if (RxQue.getSize() > 4)
    {
        return false;
    }
    uint16_t packetSize = this->RxQue.peak_uint16(2);
    return (RxQue.getSize() >= packetSize);
}

GPU_Packet *GPU_Channel::getNextPacket()
{
    return new GPU_Packet(&this->RxQue);
}

void GPU_Channel::SendPacket(GPU_Packet *packetToSend)
{
    packetToSend->appendToQue(&this->TxQue);
}