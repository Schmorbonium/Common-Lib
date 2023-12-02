#include "ibcHandler.hpp"
#include "ibc.h"
#include "ibcPacket.hpp"
#include "InterruptController.h"

uint8_t resetFlag = 0;
void setResetFlag();

IbcHandler::IbcHandler(UART_HandleTypeDef* huart, IbcPacketCallback _callback, IbcResetCallback _resetCallback, uint8_t attnMask) :
    BufferedUart(huart),
    callback(_callback),
    attnMask(attnMask) {
    this->stagedPacket = 0;
    this->syncResetCallback = _resetCallback;
    this->resetCountLimit = 8;
    sendResetVector();
}

void IbcHandler::sendResetVector() {
    for (int i = 0; i < resetCountLimit; i++) {
        TxQue.append(0);
    }
}

void setResetFlag() {
    resetFlag = 1;
}

bool IbcHandler::hasCompletePacket() {
    // special reset pkt case
    // if (resetFlag) {
    //     syncResetCallback();
    //     RxQue.clear();
    //     resetFlag = 0;
    // }

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
    InterruptController_enter();
    delete this->stagedPacket;
    this->stagedPacket = new IbcPacket(&this->RxQue);
    InterruptController_leave();
    return true;
}

void IbcHandler::processStagedPacket() {
    // if this is a reset packet, prefix with a reset 
    //  vector to ensure the next board can receive it
    if ((this->stagedPacket->id) == IBCID_RESET) {
        sendResetVector();
    }

    // if this packet has more time to live, forward it
    if (this->stagedPacket->ttl > 1) {
        this->stagedPacket->ttl -= 1;
        this->stagedPacket->queueInto(&this->TxQue);
        this->startSending();
    }

    // if this packet is relevant to this board, send it back up the chain
    if ((this->stagedPacket->attn & this->attnMask)) {
        this->callback(this->stagedPacket->id, this->stagedPacket->len, this->stagedPacket->data);
    }

}

void IbcHandler::sendPacket(IBCATTN attn, uint8_t ttl, uint8_t len, IBCID id, uint8_t* data) {
    // InterruptController_enter();
    IbcPacket p = IbcPacket(attn, ttl, len, id, data);
    p.queueInto(&TxQue);
    startSending();
    // InterruptController_leave();
}