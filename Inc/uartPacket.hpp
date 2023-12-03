
#ifndef __UART_PKT
#define __UART_PKT

#include "zHal.h"
#include "uartData.hpp"

template <typename T>
class Uart_Packet : public ISendable, public PacketField
{
public:
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
    virtual void appendPayload(CharBuffer *que) {}
    virtual uint16_t getPayloadWireSize() { return 0; }
    uint16_t getWireSize()
    {
        // The extra Byte at the end is a checksum for basic  packet verification
        return getHeaderWireSize() + getPayloadWireSize() + 1;
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

    // Appends Entire Packet to the wire, this includes any data from the Children and finally a basic checksum appended in the end
    void appendToQue(CharBuffer *que)
    {
        // TODO if Checksum is off sometimes, it may be because of interrupts? but I dont want to prevent interrupts until that happens though for good reasons... but I am too lazy to write them down right now so good luck. (Just ask me and ill tell you) -Isaac Christensen(2023)
        uint8_t oldVal = que->setQueuedCheckSum(0);
        this->appendHeader(que);
        this->appendPayload(que);
        uint8_t checksum = que->setQueuedCheckSum(oldVal);
        que->append(checksum);
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
        bool GoodPacket = false;
        uint8_t old = this->RxQue.setPoppedCheckSum(0);
        U *pkt = getNextPacket();
        uint8_t calcCheckSum = this->RxQue.setPoppedCheckSum(old);
        uint8_t readCheckSum = this->RxQue.pop();
        if (calcCheckSum == readCheckSum)
        {
            pkt->actOnPkt();
            GoodPacket = true;
        }

        delete pkt;
        return GoodPacket;
    }
};

#endif // __UART_PKT
