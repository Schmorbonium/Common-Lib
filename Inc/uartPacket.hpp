
#ifndef __UART_PKT
#define __UART_PKT

#include "zHal.h"
#include "uartData.hpp"

template <typename T>
class Uart_Packet : public ISendable, public PacketField
{
private:
public:
    static const uint32_t StartBitPattern = 0x0DDBA115;
    static const uint8_t StartBitLength = 4;

    Uint16Field pktId;
    Uint16Field pktLen;

    Uart_Packet(T command) : pktId((uint16_t)command), pktLen((uint16_t)0)
    {
    }
    Uart_Packet(CharBuffer *que) : pktId(que), pktLen(que)
    {
    }

    virtual ~Uart_Packet() {}
    virtual bool actOnPkt() { return false; }
    virtual uint8_t appendPayload(CharBuffer *que) {return 0;}
    virtual uint16_t getPayloadWireSize() { return 0; }
    uint16_t getWireSize()
    {
        return getHeaderWireSize() + getPayloadWireSize();
    }
    uint16_t getHeaderWireSize()
    {
        return pktId.getWireSize() + pktLen.getWireSize();
    }
    uint8_t appendHeader(CharBuffer *que)
    {
        uint8_t checksum = 0;
        checksum += this->pktId.appendToQue(que);
        this->pktLen.data = this->getWireSize();
        checksum += this->pktLen.appendToQue(que);
        return checksum;
    }

    uint8_t appendToQue(CharBuffer *que)
    {
        uint8_t checksum = this->appendHeader(que);
        checksum += this->appendPayload(que);
        return checksum;
    }
};

template <typename T, typename U>
class Uart_Channel : public BufferedUart
{
public:
    virtual U *getNextPacket()
    {
        return new U(&RxQue);
    }

    Uart_Channel(UART_HandleTypeDef *Core) : BufferedUart(Core)
    {
    }

    T peekCommand()
    {
        return static_cast<T>(RxQue.peak_uint16());
    }

    bool PacketReady()
    {
        if (RxQue.getSize() <= 4)
        {
            return false;
        }
        uint16_t packetSize = RxQue.peak_uint16(2);
        return (RxQue.getSize() >= packetSize);
    }

    void SendPacket(U *packetToSend)
    {
        this->send(packetToSend);
    }

    bool processNextPacket()
    {
        U *pkt = getNextPacket();
        pkt->actOnPkt();
        delete pkt;
        return true;
    }
};

#endif // __UART_PKT
