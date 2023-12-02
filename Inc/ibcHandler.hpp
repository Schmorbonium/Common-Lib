
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
    IbcResetCallback syncResetCallback;
    IbcHandler(UART_HandleTypeDef* huart, IbcPacketCallback _callback, IbcResetCallback _resetCallback, uint8_t attnMask);

    // Delete copy constructor and copy assignment operator
    IbcHandler(const IbcHandler&) = delete;
    IbcHandler& operator=(const IbcHandler&) = delete;

    // Delete move constructor and move assignment operator
    IbcHandler(IbcHandler&&) = delete;
    IbcHandler& operator=(IbcHandler&&) = delete;


    bool hasCompletePacket();
    void processStagedPacket();
    void sendPacket(IBCATTN attn, uint8_t ttl, uint8_t len, IBCID id, uint8_t* data);
    void sendResetVector();
};

#endif