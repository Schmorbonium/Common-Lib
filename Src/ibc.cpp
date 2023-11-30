#include "ibc.h"
#include "zHal.h"
#include "ibcHandler.hpp"

IbcHandler* handler;

void processIbcInbox() {
    while (handler->hasCompletePacket()) {
        handler->processStagedPacket();
    }
}

void initIbc(UART_HandleTypeDef* huart, IbcPacketCallback callback, IbcResetCallback resetCallback, uint8_t attnMask) {
    handler = new IbcHandler(huart, callback, resetCallback, attnMask);
    handler->startListening();
}

void sendIbcPacket(uint8_t attn, uint8_t ttl, uint8_t len, IBCID id, uint8_t* data) {
    handler->sendPacket((IBCATTN)attn, ttl, len, id, data);
}

uint32_t unpackData(uint8_t len, uint8_t* data) {
    switch (len) {
    case 1:
        return *data;
    case 2:
        return *((uint16_t*)data);
        //case 4:
    default:
        return *((uint32_t*)data);
    }
}

void usartIbcHandler() {
    handler->uartHandler();
}