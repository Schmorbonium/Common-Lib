#include "usbPackets.hpp"

bool MemoryRequestActive = false;
bool ReadOp = false;

uint32_t MemoryRequestedAddr;
uint32_t MemIoValue;

UsbPacket::UsbPacket(USBCommand command)
    : Uart_Packet((uint16_t)command)
{
}
UsbPacket::UsbPacket(IQueue *que)
    : Uart_Packet(que)
{
}
UsbPacket::~UsbPacket()
{
}
void UsbPacket::appendPayload(IQueue *que)
{
}
uint16_t UsbPacket::getPayloadWireSize()
{
    return 0;
}
bool UsbPacket::actOnPkt()
{
    return false;
}

UsbRst::UsbRst()
    : UsbPacket(USB_CMD_RESET_ENUM)
{
}
UsbRst::UsbRst(IQueue *que)
    : UsbPacket(que)
{
}
UsbRst::~UsbRst()
{
}
void UsbRst::appendPayload(IQueue *que)
{
}
uint16_t UsbRst::getPayloadWireSize()
{
    return 0;
}
bool UsbRst::actOnPkt()
{
    return false;
}

UsbBadRequest::UsbBadRequest(uint16_t failedCMD)
    : UsbPacket(USB_CMD_BAD_REQUEST),
      failedRequest(failedCMD)
{
}
UsbBadRequest::UsbBadRequest(IQueue *que)
    : UsbPacket(que),
      failedRequest(que)
{
}
UsbBadRequest::~UsbBadRequest()
{
}
void UsbBadRequest::appendPayload(IQueue *que)
{
    failedRequest.appendToQue(que);
}
uint16_t UsbBadRequest::getPayloadWireSize()
{
    return failedRequest.getWireSize();
}
bool UsbBadRequest::actOnPkt()
{
    return false;
}

UsbReadWordRequest::UsbReadWordRequest(uint32_t PageNumber, uint16_t pktId)
    : UsbPacket(USB_CMD_WORD_REQUEST),
      Address(PageNumber),
      PktCnt(pktId)
{
}
UsbReadWordRequest::UsbReadWordRequest(IQueue *que)
    : UsbPacket(que),
      Address(que),
      PktCnt(que)
{
}
UsbReadWordRequest::~UsbReadWordRequest()
{
}
void UsbReadWordRequest::appendPayload(IQueue *que)
{
    Address.appendToQue(que);
    PktCnt.appendToQue(que);
}
uint16_t UsbReadWordRequest::getPayloadWireSize()
{
    return Address.getWireSize() + PktCnt.getWireSize();
}

UsbReadWordResponse::UsbReadWordResponse(uint32_t addr, uint32_t value, uint16_t PktCnt)
    : UsbPacket(USN_CMD_WORD_RESPONSE),
      Address(addr),
      Value(value),
      PktCnt(PktCnt)
{
}
UsbReadWordResponse::UsbReadWordResponse(IQueue *que)
    : UsbPacket(que),
      Address(que),
      Value(que),
      PktCnt(que)
{
}
UsbReadWordResponse::~UsbReadWordResponse()
{
}
void UsbReadWordResponse::appendPayload(IQueue *que)
{
    Address.appendToQue(que);
    Value.appendToQue(que);
    PktCnt.appendToQue(que);
}

uint16_t UsbReadWordResponse::getPayloadWireSize()
{
    return Address.getWireSize() + Value.getWireSize() + PktCnt.getWireSize();
}

UsbWriteWordRequest::UsbWriteWordRequest(uint32_t addr, uint32_t value, uint16_t PktCnt)
    : UsbPacket(USB_CMD_WRITE_WORD_REQUEST),
      Address(addr),
      Value(value),
      PktCnt(PktCnt) {}
UsbWriteWordRequest::UsbWriteWordRequest(IQueue *que)
    : UsbPacket(que),
      Address(que),
      Value(que),
      PktCnt(que) {}
UsbWriteWordRequest::~UsbWriteWordRequest()
{
}
void UsbWriteWordRequest::appendPayload(IQueue *que)
{
    Address.appendToQue(que);
    Value.appendToQue(que);
    PktCnt.appendToQue(que);
}
uint16_t UsbWriteWordRequest::getPayloadWireSize()
{
    return Address.getWireSize() + Value.getWireSize() + PktCnt.getWireSize();
}

UsbWriteWordResponse::UsbWriteWordResponse(uint32_t addr, uint16_t PktCnt)
    : UsbPacket(USN_CMD_WRITE_WORD_RESPONSE),
      Address(addr),
      PktCnt(PktCnt)
{
}
UsbWriteWordResponse::UsbWriteWordResponse(IQueue *que)
    : UsbPacket(que),
      Address(que),
      PktCnt(que)
{
}
UsbWriteWordResponse::~UsbWriteWordResponse() {}
void UsbWriteWordResponse::appendPayload(IQueue *que)
{
    Address.appendToQue(que);
    PktCnt.appendToQue(que);
}
uint16_t UsbWriteWordResponse::getPayloadWireSize()
{
    return Address.getWireSize() + PktCnt.getWireSize();
}

UsbPacket *UsbChannel::getNextPacket()
{
    switch (this->peekCommand())
    {
    case USB_CMD_RESET_ENUM:
        return (new UsbRst(&this->channel->RxQue));
    case USB_CMD_BAD_REQUEST:
        return (new UsbBadRequest(&this->channel->RxQue));
    case USB_CMD_WORD_REQUEST:
        return (new UsbReadWordRequest(&this->channel->RxQue));
    case USN_CMD_WORD_RESPONSE:
        return (new UsbReadWordResponse(&this->channel->RxQue));
    case USB_CMD_WRITE_WORD_REQUEST:
        return (new UsbWriteWordRequest(&this->channel->RxQue));
    case USN_CMD_WRITE_WORD_RESPONSE:
        return (new UsbWriteWordResponse(&this->channel->RxQue));
    default:
        return (new UsbPacket(&this->channel->RxQue));
    }
}

UsbChannel::UsbChannel(IBufferedChannel *Core)
    : Uart_Channel(Core)
{
}