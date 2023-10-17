#ifndef __Z_HAL__
#define __Z_HAL__


#ifdef __cplusplus
extern "C" {
#endif
// Disco
#define __ZHAL_SET(reg,mask) reg |= mask
#define __ZHAL_RESET(reg,mask) reg &= (~mask)

#ifdef STM32F072xB
#include "stm32f0xx_hal.h"

#define _ZHAL_UART_SET_TXI(uartHandle) __ZHAL_SET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_RESET_TXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_SET_RXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 5))
#define _ZHAL_UART_RESET_RXI(uartHandle) __ZHAL_RESET(uartHandle->CR1,(1 << 5))

#define _ZHAL_UART_RX_READY(uartHandle) (uartHandle->ISR & (1 << 5))
#define _ZHAL_UART_TX_READY(uartHandle) (uartHandle->ISR & (1 << 7))

#define _ZHAL_UART_TX_BYTE(uartHandle,byteVal) uartHandle->TDR = byteVal
#define _ZHAL_UART_RX_BYTE(uartHandle) ((uint8_t)uartHandle->RDR)

#endif 

// BluePill
#ifdef STM32F103x6
#include "stm32f1xx_hal.h"

#define _ZHAL_UART_SET_TXI(uartHandle) __ZHAL_SET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_RESET_TXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_SET_RXI(uartHandle) __ZHAL_SET(uartHandle->CR1, (1 << 5))
#define _ZHAL_UART_RESET_RXI(uartHandle) __ZHAL_RESET(uartHandle->CR1,(1 << 5))

#define _ZHAL_UART_RX_READY(uartHandle) (uartHandle->SR & (1 << 5))
#define _ZHAL_UART_TX_READY(uartHandle) (uartHandle->SR & (1 << 7))

#define _ZHAL_UART_TX_BYTE(uartHandle,byteVal) uartHandle->DR = byteVal
#define _ZHAL_UART_RX_BYTE(uartHandle) ((uint8_t)uartHandle->DR)

#endif 

#ifdef STM32F103xB
#include "stm32f1xx_hal.h"

#define _ZHAL_UART_SET_TXI(uartHandle) __ZHAL_SET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_RESET_TXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_SET_RXI(uartHandle) __ZHAL_SET(uartHandle->CR1, (1 << 5))
#define _ZHAL_UART_RESET_RXI(uartHandle) __ZHAL_RESET(uartHandle->CR1,(1 << 5))

#define _ZHAL_UART_RX_READY(uartHandle) (uartHandle->SR & (1 << 5))
#define _ZHAL_UART_TX_READY(uartHandle) (uartHandle->SR & (1 << 7))

#define _ZHAL_UART_TX_BYTE(uartHandle,byteVal) uartHandle->DR = byteVal
#define _ZHAL_UART_RX_BYTE(uartHandle) ((uint8_t)uartHandle->DR)

#endif 

// BlackPill
#ifdef STM32F401xC
#include "stm32f4xx_hal.h"

#endif 

#ifdef __cplusplus
}
#endif

#endif /* __Z_HAL__ */
