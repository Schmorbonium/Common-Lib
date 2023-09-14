/**
 **************************************************
 * @file        : ibc_packet.h
 * @brief       : Header for ibc_packet.c. file
 * @author      : dknelson
 **************************************************
 *
*/

#ifndef __IBC_PACKET_H
#define __IBC_PACKET_H

#include "charBuffer.hpp"

class IbcPacket {
private:
public:
    uint8_t attn;
    uint8_t ttl;
    uint8_t len;
    uint8_t id;
    uint8_t* data;
    const static uint8_t lengthMask = 0xE0;
    IbcPacket(){}
    IbcPacket(CharBuffer* buf);
    ~IbcPacket();
    void queueInto(CharBuffer* buf);
    virtual bool process();


};




// #include "stm32f1xx_hal.h"
#endif


