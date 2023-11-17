#include "gpuClient.hpp"
#include "lcd.hpp"
#include "schmorby_assets.h"
#include "RenderObj.hpp"

extern GPU_Client gpu_client;

#ifdef USE_GPU_CLIENT
// All of the Thing that must be Implemented on the client and server Side
bool FrameIdPkt::actOnPkt() {}
bool LutIdPkt::actOnPkt() {}
bool SpriteIdPkt::actOnPkt() {}
bool NewFramePkt::actOnPkt() {}
bool NewLutPkt::actOnPkt() {}
bool NewSpritePkt::actOnPkt() {}
bool LinkFramePkt::actOnPkt() {}
bool LinkMultiFramePkt::actOnPkt() {}
bool PlaceSpritePkt::actOnPkt() {}
bool AnimateSpritePkt::actOnPkt() {}
bool MoveSpritePkt::actOnPkt() {}
bool LoadFramePkt::actOnPkt() {}
bool LoadLutPkt::actOnPkt() {}
bool GpuResetPkt::actOnPkt()
{
    switch (this->ResetType.data)
    {
    case RST_init:
        gpu_client.init();
        gpu_client.SendPacket(this);
        /* code */
        break;
    case RST_full:
        gpu_client.fullReset();
        /* code */
        break;
    case RST_ClearScreen:
        gpu_client.clearScreen();
        /* code */
        break;
    default:
        break;
    }
}

#endif

GPU_Client::GPU_Client(UART_HandleTypeDef *Core,
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

void GPU_Client::waitOnInit()
{
    while (!initialized)
    {
        if (ctlUart.RxQue.getSize() > 4)
        {
            Command cmd = this->peekCommand();
            if (cmd != Cmd_ResetGpu)
            {
                ctlUart.RxQue.pop();
                continue;
            }

            uint16_t cmdLen = ctlUart.RxQue.peak_uint16(2);
            if (ctlUart.RxQue.getSize() >= cmdLen)
            {
                GpuResetPkt resetCommand(&ctlUart.RxQue);
                resetCommand.actOnPkt();
            }
        }
    }
}

void GPU_Client::init()
{
    display.initLcd();
    display.BackLightEnable();
    display.DisOff();
    display.DisOn();
    ctlUart.startListening();
    waitOnInit();
}

void GPU_Client::fullReset()
{
}

void GPU_Client::clearScreen()
{
}

Color getBackgroundColor(uint16_t x, uint16_t y)
{
    return Color{
        (uint8_t)((x * 255) / 320),
        (uint8_t)((y * 255) / 480),
        (uint8_t)(255 - ((x * 255) / 320))};
}

void drawBackground(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t xStart = x / 10;
    uint16_t yStart = y / 10;
    uint16_t xEnd = xStart + w / 10 + 1;
    uint16_t yEnd = yStart + h / 10 + 1;
    for (uint16_t i = xStart; i < xEnd; i++)
    {
        for (uint16_t j = yStart; j < yEnd; j++)
        {
            gpu_client.display.drawRec(
                i * 10,
                j * 10,
                11,
                11,
                Color{
                    (uint8_t)((i * 255) / 32),
                    (uint8_t)((j * 255) / 48),
                    (uint8_t)(255 - ((i * 255) / 32))});
        }
    }
}

void drawBackground()
{
    drawBackground(0, 0, 320, 480);
}

void GPU_Client::testDisplay()
{
    // lcd_driver.drawRec(100, 100, 40, 40, Color{0x44, 0x44, 0x44});
    drawBackground();
    HAL_Delay(100);
    DisplayObj b1(display, schmorby_assets.Bomb1, 0, 0);
    b1.setBackground(&getBackgroundColor);
    DisplayObj b2(display, schmorby_assets.Bomb2, 0, 100);
    b2.setBackground(&getBackgroundColor);
    DisplayObj b3(display, schmorby_assets.Bomb3, 0, 200);
    b3.setBackground(&getBackgroundColor);

    DisplayObj bkt1(display, schmorby_assets.Bucket1, 100, 0);
    DisplayObj bkt2(display, schmorby_assets.Bucket2, 100, 100);
    DisplayObj bkt3(display, schmorby_assets.Bucket3, 100, 200);

    DisplayObj sch(display, schmorby_assets.Schmorby, 200, 0);

    HAL_Delay(100);
}
