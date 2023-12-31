#include "ibcHandler.hpp"
#include "ibc.h"
#include "InterruptController.h"

extern IbcResetCallback Global_iBCResetHandler;
extern IBC_Channel *handler;

IBC_Packet::IBC_Packet(IBCCommand command) : Uart_Packet(command) {}
IBC_Packet::IBC_Packet(IQueue *que) : Uart_Packet(que) {}
IBC_Packet::~IBC_Packet() {}
bool IBC_Packet::actOnPkt() { return false; }
void IBC_Packet::appendPayload(IQueue *que) {}
uint16_t IBC_Packet::getPayloadWireSize() { return 0; }

RSTPkt::RSTPkt() : IBC_Packet(IBC_CMD_RESET) {}
RSTPkt::RSTPkt(IQueue *que) : IBC_Packet(que) {}
RSTPkt::~RSTPkt() {}
void RSTPkt::appendPayload(IQueue *que) {}
uint16_t RSTPkt::getPayloadWireSize() { return 0; }
bool RSTPkt::actOnPkt()
{
    if (handler->boardID != MemIo_BoardId)
    {
        handler->SendPacket(this);
    }

    Global_iBCResetHandler();
    return true;
}

ContPkt::ContPkt(uint32_t inst, uint32_t pc, uint8_t aluOp, uint8_t memOp, uint8_t branch, uint8_t routing)
    : IBC_Packet(IBC_CMD_CONT),
      inst(inst),
      pc(pc),
      aluOp(aluOp),
      memOp(memOp),
      branch(branch),
      routing(routing)
{
}
ContPkt::ContPkt(IQueue *que) : IBC_Packet(que),
                                    inst(que),
                                    pc(que),
                                    aluOp(que),
                                    memOp(que),
                                    branch(que),
                                    routing(que)
{
}

ContPkt::~ContPkt()
{
}
void ContPkt::appendPayload(IQueue *que)
{
    inst.appendToQue(que);
    pc.appendToQue(que);
    aluOp.appendToQue(que);
    memOp.appendToQue(que);
    branch.appendToQue(que);
    routing.appendToQue(que);
}
uint16_t ContPkt::getPayloadWireSize()
{
    return inst.getWireSize() + pc.getWireSize() + aluOp.getWireSize() + memOp.getWireSize() + branch.getWireSize() + routing.getWireSize();
}
bool ContPkt::actOnPkt()
{
    if (handler->boardID != Cont_BoardId)
    {
        handler->SendPacket(this);
    }
    else
    {
        if (handler->isInitialized())
        {
            gotContData(inst.data, pc.data, aluOp.data, memOp.data, branch.data, routing.data);
        }
    }
    return true;
}

ALUPkt::ALUPkt(bool inASrc, bool inBSrc, uint8_t aluFlags, uint32_t aluOutVal)
    : IBC_Packet(IBC_CMD_ALU),
      flags((uint8_t)0),
      aluFlags(aluFlags),
      aluOutVal(aluOutVal)
{
    flags.setFlag(0, inASrc);
    flags.setFlag(1, inBSrc);
}
ALUPkt::ALUPkt(IQueue *que)
    : IBC_Packet(que),
      flags(que),
      aluFlags(que),
      aluOutVal(que)
{
}
ALUPkt::~ALUPkt()
{
}
void ALUPkt::appendPayload(IQueue *que)
{
    flags.appendToQue(que);
    aluFlags.appendToQue(que);
    aluOutVal.appendToQue(que);
}
uint16_t ALUPkt::getPayloadWireSize()
{
    return flags.getWireSize() + aluFlags.getWireSize() + aluOutVal.getWireSize();
}
bool ALUPkt::actOnPkt()
{
    if (handler->boardID != Alu_BoardId)
    {
        handler->SendPacket(this);
    }
    else
    {
        if (handler->isInitialized())
        {
            bool temp_inASrc = flags.getFlag(0);
            bool temp_inBSrc = flags.getFlag(1);
            gotALUData(temp_inASrc, temp_inBSrc, aluFlags.data, aluOutVal.data);
        }
    }
    return true;
}

RegPkt::RegPkt(bool regASrc, uint8_t regAIndex, uint32_t regAVal, bool regBSrc, uint8_t regBIndex, uint32_t regBVal, bool regDestSrc, uint8_t regDestIndex, uint32_t regDestVal)
    : IBC_Packet(IBC_CMD_REG),
      flags((uint8_t)0),
      regAIndex(regAIndex),
      regAVal(regAVal),
      regBIndex(regBIndex),
      regBVal(regBVal),
      regDestIndex(regDestIndex),
      regDestVal(regDestVal)
{
}
RegPkt::RegPkt(IQueue *que)
    : IBC_Packet(que),
      flags(que),
      regAIndex(que),
      regAVal(que),
      regBIndex(que),
      regBVal(que),
      regDestIndex(que),
      regDestVal(que)
{
}

RegPkt::~RegPkt()
{
}
void RegPkt::appendPayload(IQueue *que)
{
    flags.appendToQue(que);
    regAIndex.appendToQue(que);
    regAVal.appendToQue(que);
    regBIndex.appendToQue(que);
    regBVal.appendToQue(que);
    regDestIndex.appendToQue(que);
    regDestVal.appendToQue(que);
}
uint16_t RegPkt::getPayloadWireSize()
{
    return flags.getWireSize() +
           regAIndex.getWireSize() +
           regAVal.getWireSize() +
           regBIndex.getWireSize() +
           regBVal.getWireSize() +
           regDestIndex.getWireSize() +
           regDestVal.getWireSize();
}
bool RegPkt::actOnPkt()
{
    if (handler->boardID != Reg_BoardId)
    {
        handler->SendPacket(this);
    }
    else
    {
        if (handler->isInitialized())
        {
            bool temp_regASrc = flags.getFlag(0);
            bool temp_regBSrc = flags.getFlag(1);
            bool temp_regDestSrc = flags.getFlag(2);

            gotRegData(temp_regASrc, regAIndex.data, regAVal.data, temp_regBSrc, regBIndex.data, regBVal.data, temp_regDestSrc, regDestIndex.data, regDestVal.data);
        }
    }
    return true;
}

IBC_Channel::IBC_Channel(UART_HandleTypeDef *Core, IBC_BOARD_ID_ENUM board) : Uart_Channel(new BufferedUart(Core))
{
    boardID = board;
}

IBC_Packet *IBC_Channel::getNextPacket()
{
    IQueue *inputQue = &this->channel->RxQue;
    switch (this->peekCommand())
    {
    case IBC_CMD_RESET:
        this->initialized = true;
        return new RSTPkt(inputQue);
    case IBC_CMD_CONT:
        return new ContPkt(inputQue);
    case IBC_CMD_REG:
        return new RegPkt(inputQue);
    case IBC_CMD_ALU:
        return new ALUPkt(inputQue);
    default:
        break;
    }
    return new IBC_Packet(inputQue);
}
bool IBC_Channel::isInitialized()
{
    return this->initialized;
}
void IBC_Channel::waitOnInit()
{
    if (this->boardID != MemIo_BoardId)
    {
        while (!this->initialized)
        {
            if (this->PacketReady())
            {
                this->processNextPacket();
                this->initialized = true;
            }
        }
    }
    else
    {
        uint32_t tick = (HAL_GetTick() + 1000);
        while (!this->initialized)
        {
            if (this->PacketReady())
            {
                this->processNextPacket();
                this->initialized = true;
            }
            if ((HAL_GetTick() - tick) > 1000)
            {
                RSTPkt rstPkt = RSTPkt();
                this->SendPacket((IBC_Packet *)(&rstPkt));
                tick = HAL_GetTick();
            }
        }
    }
}