
#ifndef __IBC_HANDLER_H
#define __IBC_HANDLER_H

#include "bufferedUart.hpp"
#include "ibcPacket.hpp"
#include "ibc.h"

class IbcHandler : public BufferedUart {
public:
    IbcPacket* stagedPacket;
    IbcPacketCallback callback;
    uint8_t attnMask;
    IbcHandler(UART_HandleTypeDef* huart, IbcPacketCallback _callback, uint8_t attnMask);
    bool hasCompletePacket();
    void processStagedPacket();
    void sendPacket(IBCATTN attn, uint8_t ttl, uint8_t len, IBCID id, uint8_t* data);

};

#endif