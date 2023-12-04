#include "uartPacket.hpp"

Uart_Packet::Uart_Packet(uint16_t command) : pktId(command), pktLen((uint16_t)0)
{
}
Uart_Packet::Uart_Packet(CharBuffer *que) : pktId(que), pktLen(que)
{
}

Uart_Packet::~Uart_Packet() {}
bool Uart_Packet::actOnPkt() { return false; }
void Uart_Packet::appendPayload(CharBuffer *que) {}
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
void Uart_Packet::appendHeader(CharBuffer *que)
{
    this->pktId.appendToQue(que);
    this->pktLen.data = this->getWireSize();
    this->pktLen.appendToQue(que);
}

// Appends Entire Packet to the wire, this includes any data from the Children
void Uart_Packet::appendToQue(CharBuffer *que)
{
    this->appendHeader(que);
    this->appendPayload(que);
}

Uart_Packet *Uart_Channel::getNextPacket()
{
    return new Uart_Packet(&RxQue);
}

Uart_Channel::Uart_Channel(UART_HandleTypeDef *Core) : BufferedUart(Core)
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
    return RxQue.peak_uint16();
}

bool Uart_Channel::PacketReady()
{
    while (RxQue.getSize() > 0)
    {
        if (parsingStartBitPattern)
        {
            // Lets try to get the buffer as empty as possible, at any given time the head of the que should be some start bits and then a packet......
            uint8_t bufferHead = RxQue.pop();
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
            if (RxQue.getSize() >= 4)
            {
                uint16_t packetSize = RxQue.peak_uint16(2);
                // Packets can max out at 100 that sounds good to me. :) Quite frankly I should probably use a timeout here
                if (HAL_GetTick() - startedWaitingAt > 100)
                {
                    parsingStartBitPattern = true;
                }
                // Has to have the checksum ready too
                return (RxQue.getSize() >= (packetSize + 1));
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
        TxQue.append(startBitPattern[i]);
    }

    // TODO if Checksum is off sometimes, it may be because of interrupts? but I dont want to prevent interrupts until that happens though for good reasons... but I am too lazy to write them down right now so good luck. (Just ask me and ill tell you) -Isaac Christensen(2023)
    TxQue.setQueuedCheckSum(0);
    this->send(packetToSend);
    uint8_t checksum = TxQue.getQueuedCheckSum();
    TxQue.append(checksum);
}

bool Uart_Channel::processNextPacket()
{
    bool GoodPacket = false;
    this->RxQue.setPoppedCheckSum(0);
    Uart_Packet *pkt = getNextPacket();
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
