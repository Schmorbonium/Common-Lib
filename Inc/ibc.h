
#ifndef __IBC_H
#define __IBC_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "zHal.h"

    typedef enum
    {
        // Program Counter
        IBCID_PC = 0x01,
        // ALU Output
        IBCID_ALUQ = 0x02,
        // decoded IMMediate
        IBCID_IMM = 0x03,
        // REGister A value
        IBCID_REGA = 0x04,
        // REGister B value
        IBCID_REGB = 0x05,
        // MEMory ADDRess (for read and write)
        IBCID_MEMADDR = 0x06,
        // MEMory WRite DATa
        IBCID_MEMWRDAT = 0x07,
        // MEMory ReaD DATa
        IBCID_MEMRDDAT = 0x08,
        // INSTruction DATa
        IBCID_INSTDAT = 0x09,
        // Controller to Memory flags (len 1)
        // 0b XXXE
        // - [0] E: Memory write enable
        IBCID_C2MFLAGS = 0x0A,

        // Controller to Regfile flags (len 2)
        // 0b0SSD DDDB BBBA AAAE
        // - [14:13] S: register destination MUX (aluQ, pc+4, memData, imm)
        // - [12:9] D: Register Select Destination
        // - [8:5] B: Register Select B
        // - [4:1] A: Register Select A
        // - [0] E: Register Write Enable
        IBCID_C2RFLAGS = 0x0B,

        // Controller to ALU flags
        // 0b PP PPAB
        // - alu op
        // - alu src a [rs1/pc]
        // - alu src b [rs2/imm]
        IBCID_C2AFLAGS = 0x0C,

        IBCID_CYCLEDONE = 0x0E,
        IBCID_CLKRISINGEDGE = 0x0F,

        // Isaac's requests
        IBCID_SETREG = 0x10,
        IBCID_GETREG = 0x11,
        IBCID_SETPC = 0x12,
        IBCID_GETPC = 0x13,
        IBCID_GETFLAGS = 0x14
    } IBCID;

    typedef enum
    {
        IBCATTN_MEMIO = 0x1,
        IBCATTN_CONT = 0x2,
        IBCATTN_ALU = 0x4,
        IBCATTN_REGFILE = 0x8
    } IBCATTN;

    typedef enum
    {
        IBC_ALUOP_ADD = 0x0,
        IBC_ALUOP_SLL = 0x1,
        IBC_ALUOP_SLT = 0x2,
        IBC_ALUOP_SLTU = 0x3,
        IBC_ALUOP_XOR = 0x4,
        IBC_ALUOP_SRL = 0x5,
        IBC_ALUOP_OR = 0x6,
        IBC_ALUOP_AND = 0x7,
        IBC_ALUOP_SUB = 0x8,
        IBC_ALUOP_SRA = 0x9
    } IBC_ALUOP;

    typedef enum
    {
        MemIo_BoardId,
        Cont_BoardId,
        Alu_BoardId,
        Reg_BoardId
    } IBC_BOARD_ID_ENUM;


    typedef void (*IbcResetCallback)(void);

    void processIbcInbox();

    void initIbc(UART_HandleTypeDef* huart, IbcResetCallback resetCallback, IBC_BOARD_ID_ENUM boardID);

    void usartIbcHandler();

    void gotContData(uint32_t inst, uint32_t pc, uint8_t aluOp, uint8_t memOp, uint8_t branch, uint8_t routing);

    void gotRegData(bool regASrc, uint8_t regAIndex, uint32_t regAVal, bool regBSrc, uint8_t regBIndex, uint32_t regBVal, bool regDestSrc, uint8_t regDestIndex, uint32_t regDest);

    void gotALUData(bool inASrc, bool inBSrc, uint8_t aluFlags, uint32_t aluOutVal);

    void sendContData(uint32_t inst, uint32_t pc, uint8_t aluOp, uint8_t memOp, uint8_t branch, uint8_t routing);

    void sendRegData(bool regASrc, uint8_t regAIndex, uint32_t regAVal, bool regBSrc, uint8_t regBIndex, uint32_t regBVal, bool regDestSrc, uint8_t regDestIndex, uint32_t regDest);

    void sendALUData(bool inASrc, bool inBSrc, uint8_t aluFlags, uint32_t aluOutVal);

#ifdef __cplusplus
}
#endif
#endif