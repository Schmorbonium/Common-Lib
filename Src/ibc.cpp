#include "ibc.h"
#include "zHal.h"
#include "ibcHandler.hpp"

IBC_Channel *handler;
IbcResetCallback Global_iBCResetHandler = NULL;

void processIbcInbox()
{
    while (handler->PacketReady())
    {
        IBC_Packet *nextPacket = handler->getNextPacket();

        switch (nextPacket->pktId.data)
        {
        case IBC_CMD_RESET:
            if (handler->boardID != Reg_BoardId)
            {
                handler->SendPacket(nextPacket);
            }
            nextPacket->actOnPkt();
            break;
        case IBC_CMD_CONT:
            if (handler->boardID != Reg_BoardId)
            {
                handler->SendPacket(nextPacket);
            }
            else
            {
                nextPacket->actOnPkt();
            }

            break;
        case IBC_CMD_ALU:
            if (handler->boardID != Reg_BoardId)
            {
                handler->SendPacket(nextPacket);
            }
            else
            {
                nextPacket->actOnPkt();
            }
            break;
        case IBC_CMD_REG:
            if (handler->boardID != Reg_BoardId)
            {
                handler->SendPacket(nextPacket);
            }
            else
            {
                nextPacket->actOnPkt();
            }
            break;
        default:
            break;
        }
    }
}

void initIbc(UART_HandleTypeDef *huart, IbcResetCallback resetCallback, IBC_BOARD_ID_ENUM boardID)
{
    Global_iBCResetHandler = resetCallback;
    handler = new IBC_Channel(huart, boardID);
    handler->startListening();
    handler->waitOnInit();
}

// void sendIbcPacket(uint8_t attn, uint8_t ttl, uint8_t len, IBCID id, uint8_t* data) {
//     // handler->SendPacket((IBCATTN)attn, ttl, len, id, data);
// }

uint32_t unpackData(uint8_t len, uint8_t *data)
{
    switch (len)
    {
    case 1:
        return *data;
    case 2:
        return *((uint16_t *)data);
        // case 4:
    default:
        return *((uint32_t *)data);
    }
}

void usartIbcHandler()
{
    handler->uartHandler();
}

__weak void gotContData(uint32_t inst, uint32_t pc, uint8_t aluOp, uint8_t memOp, uint8_t branch, uint8_t routing)
{
}

__weak void gotRegData(bool regASrc, uint8_t regAIndex, uint32_t regAVal, bool regBSrc, uint8_t regBIndex, uint32_t regBVal, bool regDestSrc, uint8_t regDestIndex, uint32_t regDest)
{
}

__weak void gotALUData(bool inASrc, bool inBSrc, uint8_t aluFlags, uint32_t aluOutVal)
{
}

void sendContData(uint32_t inst, uint32_t pc, uint8_t aluOp, uint8_t memOp, uint8_t branch, uint8_t routing)
{
    ContPkt contPkt(inst, pc, aluOp, memOp, branch, routing);
    handler->SendPacket((IBC_Packet *)(&contPkt));
}

void sendRegData(bool regASrc, uint8_t regAIndex, uint32_t regAVal, bool regBSrc, uint8_t regBIndex, uint32_t regBVal, bool regDestSrc, uint8_t regDestIndex, uint32_t regDest)
{
    RegPkt regPkt(regASrc, regAIndex, regAVal, regBSrc, regBIndex, regBVal, regDestSrc, regDestIndex, regDest);
    handler->SendPacket((IBC_Packet *)(&regPkt));
}
void sendALUData(bool inASrc, bool inBSrc, uint8_t aluFlags, uint32_t aluOutVal)
{
    ALUPkt aluPkt(inASrc,inBSrc,aluFlags,aluOutVal);
    handler->SendPacket((IBC_Packet *)(&aluPkt));
}