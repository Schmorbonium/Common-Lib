
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

    // Appends Entire Packet to the wire, this includes any data from the Children
    void appendToQue(CharBuffer *que)
    {
        this->appendHeader(que);
        this->appendPayload(que);
    }
};

template <typename T, typename U>
class Uart_Channel : public BufferedUart
{
private:
    inline static const uint8_t startBitPatternLen = 4;
    inline static const uint8_t startBitPattern[] = {0x0D, 0xDB, 0xA1, 0x15};
    bool parsingStartBitPattern = true;
    uint8_t NextPatternIndex = 0;

    virtual U *getNextPacket()
    {
        return new U(&RxQue);
    }

public:
    Uart_Channel(UART_HandleTypeDef *Core) : BufferedUart(Core)
    {
    }

    T peekCommand()
    {
        return static_cast<T>(RxQue.peak_uint16());
    }

    bool PacketReady()
    {
        if (parsingStartBitPattern)
        {
            // Lets try to get the buffer as empty as possible, at any given time the head of the que should be some start bits and then a packet......
            volatile uint16_t queLen = RxQue.getSize();
            while (queLen > 0)
            {
                queLen = RxQue.getSize();
                uint8_t bufferHead = RxQue.pop();
                if (bufferHead == startBitPattern[NextPatternIndex])
                {
                    NextPatternIndex++;
                    if (NextPatternIndex >= startBitPatternLen)
                    {
                        parsingStartBitPattern = false;
                        NextPatternIndex = 0;
                    }
                }
                else
                {
                    NextPatternIndex = 0;
                }
            }
        }
        else
        {
            if (RxQue.getSize() >= 4)
            {
                uint16_t packetSize = RxQue.peak_uint16(2);
                return (RxQue.getSize() >= (packetSize + 1)); // Has to have the checksum ready too
            }
        }
        return false;
    }

    void SendPacket(U *packetToSend)
    {
        // Start by sending the startBitPatternBits
        for (uint16_t i = 0; i < startBitPatternLen; i++)
        {
            TxQue.append(startBitPattern[i]);
        }

        // TODO if Checksum is off sometimes, it may be because of interrupts? but I dont want to prevent interrupts until that happens though for good reasons... but I am too lazy to write them down right now so good luck. (Just ask me and ill tell you) -Isaac Christensen(2023)
        TxQue.setQueuedCheckSum(0);
        this->send(packetToSend);
        uint8_t checksum = TxQue.getQueuedCheckSum();
        TxQue.append(checksum);
    }

    bool processNextPacket()
    {
        bool GoodPacket = false;
        this->RxQue.setPoppedCheckSum(0);
        U *pkt = getNextPacket();
        parsingStartBitPattern = true;
        uint8_t calcCheckSum = this->RxQue.getPoppedCheckSum();
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
