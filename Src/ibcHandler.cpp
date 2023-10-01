#include "ibcHandler.hpp"
#include "ibc.h"
#include "ibcPacket.hpp"

IbcHandler::IbcHandler(UART_HandleTypeDef* huart, IbcPacketCallback _callback, uint8_t attnMask) :
    BufferedUart(huart),
    callback(_callback),
    attnMask(attnMask) {
        this->stagedPacket = 0;
}

bool IbcHandler::hasCompletePacket() {
    // buffer does not have enough bytes to check header
    if (this->RxQue.getSize() < 2) {
        return false;
    }

    // buffer does not have all the data bytes that belong to this header
    uint8_t byte1 = this->RxQue.peak(1);
    if (this->RxQue.getSize() < ((byte1 & IbcPacket::lengthMask) >> 5) + 2) {
        return false;
    }

    // stage this packet to be processed
    delete this->stagedPacket;
    this->stagedPacket = new IbcPacket(&this->RxQue);
    return true;
}

void IbcHandler::processStagedPacket() {
    // if this packet is relevant to this board, send it back up the chain
    if ((this->stagedPacket->attn & this->attnMask)) {
        this->callback(this->stagedPacket->id, this->stagedPacket->len, this->stagedPacket->data);
    }

    // if this packet has more time to live, forward it
    if (this->stagedPacket->ttl > 1) {
        this->stagedPacket->ttl -= 1;
        this->stagedPacket->queueInto(&this->TxQue);
        this->startSending();
    }
}