
#ifndef _GPU_CLIENT
#define _CPU_CLIENT

#include "gpuCore.hpp"
#include "lcd.hpp"

class GPU_Client : public GPU_Channel
{
public:
    bool initialized = false;
    LCD_Controller display;
    GPU_Client(UART_HandleTypeDef *Core,
               SPI_HandleTypeDef *Spi,
               GPIO_TypeDef *DataComSel_GPIO,
               uint16_t DataComSel_pin,
               GPIO_TypeDef *chipSel_GPIO,
               uint16_t chipSel_pin,
               GPIO_TypeDef *LcdRst_GPIO,
               uint16_t LcdRst_pin,
               GPIO_TypeDef *LcdBackLed_GPIO,
               uint16_t LcdBackLed_pin);

    void waitOnInit();
    void testDisplay();
    void init();
    void fullReset();
    void clearScreen();
};

#endif