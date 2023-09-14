
#ifndef __IBC_H
#define __IBC_H

#ifdef __cplusplus
extern "C" {
#endif
#include "zHal.h"

    typedef void (*IbcPacketCallback)(uint8_t, uint8_t, uint8_t*);
    void processIbcInbox();
    void initIbc(UART_HandleTypeDef* huart, IbcPacketCallback callback, uint8_t attnMask);
    void usartIbcHandler();


#ifdef __cplusplus
}
#endif
#endif