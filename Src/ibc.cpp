#include "ibc.h"
#include "zHal.h"
#include "ibcHandler.hpp"

IbcHandler* handler;

void processIbcInbox() {
    while (handler->hasCompletePacket()) {
        handler->processStagedPacket();
    }
}

void initIbc(UART_HandleTypeDef* huart, IbcPacketCallback callback, uint8_t attnMask) {
    handler = new IbcHandler(huart, callback, attnMask);
    handler->startListening();
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