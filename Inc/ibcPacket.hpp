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
#include "ibc.h"

class IbcPacket {
private:
public:
    IBCATTN attn;
    uint8_t ttl;
    uint8_t len;
    IBCID id;
    uint8_t* data;
    const static uint8_t lengthMask = 0xE0;
    IbcPacket() :attn((IBCATTN)0), ttl(0), len(0), id((IBCID)0), data(new uint8_t[1]) {}
    IbcPacket(IBCATTN attn, uint8_t ttl, uint8_t len, IBCID id, uint8_t* data);
    IbcPacket(CharBuffer* buf);
    virtual ~IbcPacket();

    // Delete copy constructor and copy assignment operator
    IbcPacket(const IbcPacket&) = delete;
    IbcPacket& operator=(const IbcPacket&) = delete;

    // Delete move constructor and move assignment operator
    IbcPacket(IbcPacket&&) = delete;
    IbcPacket& operator=(IbcPacket&&) = delete;

    void queueInto(CharBuffer* buf);
    virtual bool process() { return false; }


};




// #include "stm32f1xx_hal.h"
#endif


