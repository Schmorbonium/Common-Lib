#include "uartPacket.hpp"

Uart_Packet::Uart_Packet(uint16_t command) : pktId(command), pktLen((uint16_t)0)
{
}
Uart_Packet::Uart_Packet(IQueue *que) : pktId(que), pktLen(que)
{
}

Uart_Packet::~Uart_Packet() {}
bool Uart_Packet::actOnPkt() { return false; }
void Uart_Packet::appendPayload(IQueue *que) {}
uint16_t Uart_Packet::getPayloadWireSize() { return 0; }
uint16_t Uart_Packet::getWireSize()
{
    // The extra Byte at the end is a checksum for basic  packet verification
    return getHeaderWireSize() + getPayloadWireSize();
}
uint16_t Uart_Packet::getHeaderWireSize()
{
    return pktId.getWireSize() + pktLen.getWireSize();
}
void Uart_Packet::appendHeader(IQueue *que)
{
    this->pktId.appendToQue(que);
    this->pktLen.data = this->getWireSize();
    this->pktLen.appendToQue(que);
}

// Appends Entire Packet to the wire, this includes any data from the Children
void Uart_Packet::appendToQue(IQueue *que)
{
    this->appendHeader(que);
    this->appendPayload(que);
}

Uart_Packet *Uart_Channel::getNextPacket()
{
    return new Uart_Packet(&channel->RxQue);
}

Uart_Channel::Uart_Channel(IBufferedChannel *core) : channel(core)
{
    parsingStartBitPattern = true;
    NextPatternIndex = 0;
    startBitPatternLen = 4;
    startBitPattern[0] = 0x0D;
    startBitPattern[1] = 0xDB;
    startBitPattern[2] = 0xA1;
    startBitPattern[3] = 0x15;
    seenData = false;
}

uint16_t Uart_Channel::peekCommand()
{
    return channel->RxQue.peak_uint16();
}

bool Uart_Channel::PacketReady()
{
    while (channel->RxQue.getSize() > 0)
    {
        if (parsingStartBitPattern)
        {
            // Lets try to get the buffer as empty as possible, at any given time the head of the que should be some start bits and then a packet......
            uint8_t bufferHead = channel->RxQue.pop();
            if (bufferHead == startBitPattern[NextPatternIndex])
            {
                NextPatternIndex += 1;
                if (NextPatternIndex >= startBitPatternLen)
                {
                    startedWaitingAt = HAL_GetTick();
                    parsingStartBitPattern = false;
                    NextPatternIndex = 0;
                }
            }
            else
            {
                NextPatternIndex = 0;
            }
        }
        else
        {
            uint64_t timeWaiting = HAL_GetTick() - startedWaitingAt;
            // Packets can max out at 100 that sounds good to me. :) Quite frankly I should probably use a timeout here
            if ( timeWaiting > 100)
            {
                parsingStartBitPattern = true;
            }
            if (channel->RxQue.getSize() >= 4)
            {
                uint16_t packetSize = channel->RxQue.peak_uint16(2);
                // Has to have the checksum ready too
                return (channel->RxQue.getSize() >= (packetSize + 1));
            }else{
                return false;
            }
        }
    }
    return false;
}

void Uart_Channel::SendPacket(ISendable *packetToSend)
{
    // Start by sending the startBitPatternBits
    for (uint16_t i = 0; i < startBitPatternLen; i++)
    {
        channel->TxQue.append(startBitPattern[i]);
    }

    // TODO if Checksum is off sometimes, it may be because of interrupts? but I dont want to prevent interrupts until that happens though for good reasons... but I am too lazy to write them down right now so good luck. (Just ask me and ill tell you) -Isaac Christensen(2023)
    channel->TxQue.setQueuedCheckSum(0);
    channel->send(packetToSend);
    uint8_t checksum = channel->TxQue.getQueuedCheckSum();
    channel->TxQue.append(checksum);
}

bool Uart_Channel::processNextPacket()
{
    bool GoodPacket = false;
    channel->RxQue.setPoppedCheckSum(0);
    Uart_Packet *pkt = getNextPacket();
    parsingStartBitPattern = true;
    uint8_t calcCheckSum = channel->RxQue.getPoppedCheckSum();
    uint8_t readCheckSum = channel->RxQue.pop();
    if (calcCheckSum == readCheckSum)
    {
        pkt->actOnPkt();
        GoodPacket = true;
    }

    delete pkt;
    return GoodPacket;
}
