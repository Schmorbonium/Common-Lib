
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

    virtual ~Uart_Packet(){}
    virtual bool actOnPkt(){ return false;}
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
