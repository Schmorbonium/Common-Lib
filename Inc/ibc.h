
#ifndef __IBC_H
#define __IBC_H

#ifdef __cplusplus
extern "C" {
#endif
#include "zHal.h"

typedef enum {
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
    ICBID_MEMRDDAT = 0x08,
    // INSTruction DATa
    IBCID_INSTDAT = 0x09,
    // Controller to Memory flags (len 1)
    // 0b XXXE
    // - [0] E: Memory write enable
    IBCID_C2MFLAGS = 0x0A,
    // Controller to Regfile flags (len 2)
    // 0bDDDD DBBB BBAA AAAE
    // - [15:11] D: Register Select Destination
    // - [10:6] B: Register Select B
    // - [5:1] A: Register Select A
    // - [0] E: Register Write Enable
    IBCID_C2RFLAGS = 0x0B,
    // Controller to ALU flags
    // - alu op
    // - alu src a
    // - alu src b
    IBCID_C2AFLAGS = 0x0C,

    // Isaac's requests
    IBCID_SETREG = 0x10,
    IBCID_GETREG = 0x11,
    IBCID_SETPC = 0x12,
    IBCID_GETPC = 0x13,
    IBCID_GETFLAGS = 0x14
} IBCID;

    typedef void (*IbcPacketCallback)(IBCID, uint8_t, uint8_t*);
    void processIbcInbox();
    void initIbc(UART_HandleTypeDef* huart, IbcPacketCallback callback, uint8_t attnMask);
    void usartIbcHandler();
    // len must be 1, 2, or 4 bytes
    uint32_t unpackData(uint8_t len, uint8_t* data);

#ifdef __cplusplus
}
#endif
#endif