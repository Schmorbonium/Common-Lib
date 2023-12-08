// #ifdef USE_GPU_CLIENT

// #include "gpuClient.hpp"
// #include "lcd.hpp"
// #include "schmorby_assets.h"
// #include "RenderObj.hpp"

// extern GPU_Client gpu_client;
// // Implement in client and server
// // Implement in client and server
// // Implement in client and server
// bool NewShapePkt::actOnPkt()
// {
//     return false;
// }
// bool SetShapePkt::actOnPkt() { return false; }
// bool MoveShapePkt::actOnPkt() { return false; }

// // All of the Thing that must be Implemented on the client and server Side
// bool FrameIdPkt::actOnPkt() { return false; }
// bool LutIdPkt::actOnPkt() { return false; }
// bool SpriteIdPkt::actOnPkt() { return false; }
// bool NewFramePkt::actOnPkt() { return false; }
// bool NewLutPkt::actOnPkt() { return false; }
// bool NewSpritePkt::actOnPkt() { return false; }
// bool LinkFramePkt::actOnPkt() { return false; }
// bool LinkMultiFramePkt::actOnPkt() { return false; }
// bool PlaceSpritePkt::actOnPkt() { return false; }
// bool AnimateSpritePkt::actOnPkt() { return false; }
// bool MoveSpritePkt::actOnPkt() { return false; }
// bool LoadFramePkt::actOnPkt() { return false; }
// bool LoadLutPkt::actOnPkt() { return false; }
// bool FillBackGroundPkt::actOnPkt()
// {
//     gpu_client.display.clearFrame(this->fillColor.value);
//     return true;
// }
// bool GpuResetPkt::actOnPkt()
// {
//     switch (this->ResetType.data)
//     {
//     case RST_init:
//         // gpu_client.init();
//         gpu_client.markInitalized();
//         gpu_client.SendPacket(this);
//         /* code */
//         break;
//     case RST_full:
//         gpu_client.fullReset();
//         /* code */
//         break;
//     case RST_ClearScreen:
//         gpu_client.clearScreen();
//         /* code */
//         break;
//     default:
//         return false;
//     }
//     return true;
// }

// GPU_Client::GPU_Client(UART_HandleTypeDef *Core,
//                        SPI_HandleTypeDef *Spi,
//                        GPIO_TypeDef *DataComSel_GPIO,
//                        uint16_t DataComSel_pin,
//                        GPIO_TypeDef *chipSel_GPIO,
//                        uint16_t chipSel_pin,
//                        GPIO_TypeDef *LcdRst_GPIO,
//                        uint16_t LcdRst_pin,
//                        GPIO_TypeDef *LcdBackLed_GPIO,
//                        uint16_t LcdBackLed_pin)
//     : GPU_Channel(
//           Core),
//       display(
//           Spi,
//           DataComSel_GPIO,
//           DataComSel_pin,
//           chipSel_GPIO,
//           chipSel_pin,
//           LcdRst_GPIO,
//           LcdRst_pin,
//           LcdBackLed_GPIO,
//           LcdBackLed_pin)
// {
// }

// void GPU_Client::markInitalized()
// {
//     this->initialized = true;
// }

// void GPU_Client::waitOnInit()
// {
//     while (!initialized)
//     {
//         if (ctlUart.RxQue.getSize() > 4)
//         {
//             Command cmd = this->peekCommand();
//             if (cmd != Cmd_ResetGpu)
//             {
//                 ctlUart.RxQue.pop();
//                 continue;
//             }

//             uint16_t cmdLen = ctlUart.RxQue.peak_uint16(2);
//             if (ctlUart.RxQue.getSize() >= cmdLen)
//             {
//                 GpuResetPkt resetCommand(&ctlUart.RxQue);
//                 resetCommand.actOnPkt();
//             }
//         }
//     }
// }

// void GPU_Client::init()
// {
//     display.initLcd();
//     display.BackLightEnable();
//     display.DisOff();
//     display.DisOn();
//     ctlUart.startListening();
//     waitOnInit();
// }

// void GPU_Client::fullReset()
// {
// }

// void GPU_Client::clearScreen()
// {
// }

// Color getBackgroundColor(uint16_t x, uint16_t y)
// {
//     return Color{
//         (uint8_t)((x * 255) / 320),
//         (uint8_t)((y * 255) / 480),
//         (uint8_t)(255 - ((x * 255) / 320))};
// }

// void drawBackground(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
// {
//     uint16_t xStart = x / 10;
//     uint16_t yStart = y / 10;
//     uint16_t xEnd = xStart + w / 10 + 1;
//     uint16_t yEnd = yStart + h / 10 + 1;
//     for (uint16_t i = xStart; i < xEnd; i++)
//     {
//         for (uint16_t j = yStart; j < yEnd; j++)
//         {
//             gpu_client.display.drawRec(
//                 i * 10,
//                 j * 10,
//                 11,
//                 11,
//                 Color{
//                     (uint8_t)((i * 255) / 32),
//                     (uint8_t)((j * 255) / 48),
//                     (uint8_t)(255 - ((i * 255) / 32))});
//         }
//     }
// }

// void drawBackground()
// {
//     drawBackground(0, 0, 320, 480);
// }



// #endif