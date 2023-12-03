
#ifndef __IBC_HANDLER_H
#define __IBC_HANDLER_H

#include "bufferedUart.hpp"
#include "uartData.hpp"
#include "uartPacket.hpp"
#include "ibcPacket.hpp"
#include "ibc.h"

enum IBCCommand : uint16_t
{
    IBC_CMD_RESET = 0x1,
    IBC_CMD_CONT,
    IBC_CMD_ALU,
    IBC_CMD_REG,
};

/// @brief This class is not intended to be used, so like dont.
class IBC_Packet : public Uart_Packet<IBCCommand>
{
public:
    IBC_Packet(IBCCommand command);
    IBC_Packet(CharBuffer *que);
    virtual ~IBC_Packet();
    virtual bool actOnPkt();
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
};

// This Packet triggers a reset
class RSTPkt : public IBC_Packet
{
public:
    RSTPkt();
    RSTPkt(CharBuffer *que);
    ~RSTPkt();
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

// This packet contains all the data for the controller to update
class ContPkt : public IBC_Packet
{
    Uint32Field inst;
    Uint32Field pc;
    Uint8Field aluOp;
    Uint8Field memOp;
    Uint8Field branch;
    Uint8Field routing;

public:
    ContPkt(uint32_t inst, uint32_t pc, uint8_t aluOp, uint8_t memOp, uint8_t branch, uint8_t routing);
    ContPkt(CharBuffer *que);

    ~ContPkt();
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

// This packet contains all the data for the ALU to update
class ALUPkt : public IBC_Packet
{
    FlagField flags; // inASrc -> 0, inBSrc -> 1
    Uint8Field aluFlags;
    Uint32Field aluOutVal;

public:
    ALUPkt(bool inASrc, bool inBSrc, uint8_t aluFlags, uint32_t aluOutVal);
    ALUPkt(CharBuffer *que);
    ~ALUPkt();
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

// This packet contains all the data for the RegFile to update
class RegPkt : public IBC_Packet
{
    FlagField flags; // regASrc -> 0, regBSrc -> 1 ,regDestSrc -> 2
    Uint8Field regAIndex;
    Uint32Field regAVal;
    Uint8Field regBIndex;
    Uint32Field regBVal;
    Uint8Field regDestIndex;
    Uint32Field regDestVal;

public:
    RegPkt(bool regASrc, uint8_t regAIndex, uint32_t regAVal, bool regBSrc, uint8_t regBIndex, uint32_t regBVal, bool regDestSrc, uint8_t regDestIndex, uint32_t regDestVal);
    RegPkt(CharBuffer *que);
    ~RegPkt();
    virtual void appendPayload(CharBuffer *que);
    virtual uint16_t getPayloadWireSize();
    virtual bool actOnPkt();
};

// This will replace IbcHandler
class IBC_Channel : public Uart_Channel<IBCCommand, IBC_Packet>
{
    bool initialized = false;
public:
    IBC_BOARD_ID_ENUM boardID;
    IBC_Channel(UART_HandleTypeDef *Core, IBC_BOARD_ID_ENUM board);
    IBC_Packet *getNextPacket();
    void waitOnInit(); 
};

#endif