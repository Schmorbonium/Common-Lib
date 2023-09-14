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

void usartIbcHandler() {
    handler->uartHandler();
}