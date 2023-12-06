
#ifndef __USB_Packets_
#define __USB_Packets_

#include "bufferedUart.hpp"
#include "uartData.hpp"
#include "uartPacket.hpp"
#include "ibc.h"

enum USBCommand : uint16_t
{
    USB_CMD_RESET_ENUM = 0x1,
    USB_CMD_BAD_REQUEST,
    USB_CMD_WORD_REQUEST,
    USN_CMD_WORD_RESPONSE
    // USB_CMD_PAGE_REQUEST,
    // USN_CMD_PAGE_RESPONSE,
};

/// @brief This class is not intended to be used, so like dont.
class UsbPacket : public Uart_Packet
{
public:
    UsbPacket(USBCommand command);
    UsbPacket(IQueue *que);
    virtual ~UsbPacket();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

// This Packet triggers a reset
class UsbRst : public UsbPacket
{
public:
    UsbRst();
    UsbRst(IQueue *que);
    ~UsbRst();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

class UsbBadRequest : public UsbPacket
{
    Uint16Field failedRequest;

public:
    UsbBadRequest(uint16_t failedCMD);
    UsbBadRequest(IQueue *que);
    ~UsbBadRequest();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

class UsbReadWordRequest : public UsbPacket
{
    Uint32Field Address;
    Uint16Field PktCnt;

public:
    UsbReadWordRequest(uint32_t PageNumber,uint16_t PktCnt);
    UsbReadWordRequest(IQueue *que);
    ~UsbReadWordRequest();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

class UsbReadWordResponse : public UsbPacket
{
    Uint32Field Address;
    Uint32Field Value;
    Uint16Field PktCnt;

public:
    UsbReadWordResponse(uint32_t addr, uint32_t value,uint16_t PktCnt);
    UsbReadWordResponse(IQueue *que);
    ~UsbReadWordResponse();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

// This will replace IbcHandler
class UsbChannel : public Uart_Channel
{
private:
    UsbPacket *getNextPacket();

public:
    bool isServer;
    UsbChannel(IBufferedChannel *Core);
};

#endif