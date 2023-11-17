
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
               uint16_t LcdBackLed_pin)
        : GPU_Channel(
              Core),
          display(
              Spi,
              DataComSel_GPIO,
              DataComSel_pin,
              chipSel_GPIO,
              chipSel_pin,
              LcdRst_GPIO,
              LcdRst_pin,
              LcdBackLed_GPIO,
              LcdBackLed_pin)
    {
    }

    void waitOnInit()
    {
        while (!initialized)
        {
            if (this->RxQue.getSize() > 4)
            {
                Command cmd = this->peekCommand();
                if (cmd != Cmd_ResetGpu)
                {
                    RxQue.pop();
                    continue;
                }

                uint16_t cmdLen = RxQue.peak_uint16(2);
                if (RxQue.getSize() >= cmdLen)
                {
                    GpuResetPkt resetCommand(&RxQue);
                    resetCommand.actOnPkt();
                }
            }
        }
    }

    void init()
    {
        this->initialized = true;
    }

    void fullReset()
    {
    }

    void clearScreen()
    {
    }
};

#endif