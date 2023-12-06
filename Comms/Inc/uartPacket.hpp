
#ifndef __UART_PKT
#define __UART_PKT

#include "zHal.h"
#include "iQueue.hpp"
#include "uartData.hpp"
#include "iBufferedChannel.hpp"


class Uart_Packet : public ISendable, public PacketField
{
public:
    Uint16Field pktId;
    Uint16Field pktLen;

    Uart_Packet(uint16_t command);
    Uart_Packet(IQueue *que);

    virtual ~Uart_Packet();
    virtual bool actOnPkt();
    virtual void appendPayload(IQueue *que);
    virtual uint16_t getPayloadWireSize();
    uint16_t getWireSize();
    uint16_t getHeaderWireSize();
    void appendHeader(IQueue *que);

    // Appends Entire Packet to the wire, this includes any data from the Children
    void appendToQue(IQueue *que);
};

class Uart_Channel 
{
private:
    uint32_t startedWaitingAt;
    uint8_t startBitPatternLen;
    uint8_t startBitPattern[4];
    bool parsingStartBitPattern;
    uint8_t NextPatternIndex;
    bool seenData;
    virtual Uart_Packet *getNextPacket();

public:
    IBufferedChannel* channel;
    Uart_Channel(IBufferedChannel *Core);
    uint16_t peekCommand();
    bool PacketReady();
    void SendPacket(ISendable *packetToSend);
    bool processNextPacket();
};

#endif // __UART_PKT
