#ifndef __Z_HAL__
#define __Z_HAL__

#ifdef __cplusplus
extern "C"
{
#endif

#define __ZHAL_SET(reg, mask) reg |= mask
#define __ZHAL_RESET(reg, mask) reg &= (~mask)

// Disco
#ifdef STM32F072xB
#include "stm32f0xx_hal.h"

#define _ZHAL_UART_SET_TXI(uartHandle) __ZHAL_SET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_RESET_TXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_SET_RXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 5))
#define _ZHAL_UART_RESET_RXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 5))

#define _ZHAL_UART_RX_READY(uartHandle) (uartHandle->ISR & (1 << 5))
#define _ZHAL_UART_TX_READY(uartHandle) (uartHandle->ISR & (1 << 7))

#define _ZHAL_UART_TX_BYTE(uartHandle, byteVal) uartHandle->TDR = byteVal
#define _ZHAL_UART_RX_BYTE(uartHandle) ((uint8_t)uartHandle->RDR)

#endif

// BluePill
#ifdef STM32F103x6
#include "stm32f1xx_hal.h"

#define _ZHAL_UART_SET_TXI(uartHandle) __ZHAL_SET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_RESET_TXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_SET_RXI(uartHandle) __ZHAL_SET(uartHandle->CR1, (1 << 5))
#define _ZHAL_UART_RESET_RXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 5))

#define _ZHAL_UART_RX_READY(uartHandle) (uartHandle->SR & (1 << 5))
#define _ZHAL_UART_TX_READY(uartHandle) (uartHandle->SR & (1 << 7))

#define _ZHAL_UART_TX_BYTE(uartHandle, byteVal) uartHandle->DR = byteVal
#define _ZHAL_UART_RX_BYTE(uartHandle) ((uint8_t)uartHandle->DR)

#endif

// BluePill
#ifdef STM32F103xB
#include "stm32f1xx_hal.h"

#define _ZHAL_UART_SET_TXI(uartHandle) __ZHAL_SET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_RESET_TXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_SET_RXI(uartHandle) __ZHAL_SET(uartHandle->CR1, (1 << 5))
#define _ZHAL_UART_RESET_RXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 5))

#define _ZHAL_UART_RX_READY(uartHandle) (uartHandle->SR & (1 << 5))
#define _ZHAL_UART_TX_READY(uartHandle) (uartHandle->SR & (1 << 7))

#define _ZHAL_UART_TX_BYTE(uartHandle, byteVal) uartHandle->DR = byteVal
#define _ZHAL_UART_RX_BYTE(uartHandle) ((uint8_t)uartHandle->DR)

#endif

// BlackPill
#ifdef STM32F401xC
#include "stm32f4xx_hal.h"

#define _ZHAL_UART_SET_TXI(uartHandle) __ZHAL_SET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_RESET_TXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 7))
#define _ZHAL_UART_SET_RXI(uartHandle) __ZHAL_SET(uartHandle->CR1, (1 << 5))
#define _ZHAL_UART_RESET_RXI(uartHandle) __ZHAL_RESET(uartHandle->CR1, (1 << 5))

#define _ZHAL_UART_RX_READY(uartHandle) (uartHandle->SR & (1 << 5))
#define _ZHAL_UART_TX_READY(uartHandle) (uartHandle->SR & (1 << 7))

#define _ZHAL_UART_TX_BYTE(uartHandle, byteVal) uartHandle->DR = byteVal
#define _ZHAL_UART_RX_BYTE(uartHandle) ((uint8_t)uartHandle->DR)

#endif

// No MicroController target
#if !defined(STM32F401xC) && !defined(STM32F103xB) && !defined(STM32F103x6) && !defined(STM32F072xB)

#include <cstdint>
#include <time.h>

#define _ZHAL_UART_SET_TXI(uartHandle) /* testing */
#define _ZHAL_UART_RESET_TXI(uartHandle) /* testing */
#define _ZHAL_UART_SET_RXI(uartHandle) /* testing */
#define _ZHAL_UART_RESET_RXI(uartHandle) /* testing */

#define _ZHAL_UART_RX_READY(uartHandle) true
#define _ZHAL_UART_TX_READY(uartHandle) true

#define _ZHAL_UART_TX_BYTE(uartHandle, byteVal) /* testing */
#define _ZHAL_UART_RX_BYTE(uartHandle) 1


#define __weak   __attribute__((weak))

#define HAL_GetTick() time(nullptr) 

#define __disable_irq()
#define __enable_irq()

    typedef struct
    {
        uint8_t Instance;
    } UART_HandleTypeDef;

#endif
#ifdef __cplusplus
}
#endif

#endif /* __Z_HAL__ */
