
#include "zHal.h"
#include "imageData.h"
#include "lcd.hpp"
#include "iQueue.hpp"
#include "bufferedUart.hpp"

void LCD_Controller::writeCmdToLcd(uint8_t cmd)
{
    HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_RESET); // Signals that it is a command being written
    HAL_SPI_Transmit(Spi, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
}

void LCD_Controller::writeDataToLcd(uint8_t data)
{
    HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_SET); // Signals that data being written
    HAL_SPI_Transmit(Spi, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
}

void LCD_Controller::writeDataToLcd(uint32_t dataLen, uint8_t *data)
{
    HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_SET); // Signals that data being written
    // uint16_t status=
    HAL_SPI_Transmit(Spi, data, dataLen, HAL_MAX_DELAY);
    // uint8_t volatile testing = 10;
    // if(status){
    //     testing = 10+1;
    // }
    // HAL_SPI_Transmit(Spi, &dataLSB, 1, 0);
    HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
}
void LCD_Controller::writeLUTDataToLcd(uint32_t dataLen, uint8_t *data, uint8_t *LUT)
{
    HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_SET); // Signals that data being written
    for (uint32_t i = 0; i < dataLen; i++)
    {
        uint8_t *c1 = &(LUT[((data[i] >> 0) & 0xF) * 4]);
        uint8_t *c2 = &(LUT[((data[i] >> 4) & 0xF) * 4]);
        HAL_SPI_Transmit(Spi, c2, 3, HAL_MAX_DELAY);
        HAL_SPI_Transmit(Spi, c1, 3, HAL_MAX_DELAY);
    }

    HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
}
void LCD_Controller::writeColorDataToLcd(uint32_t dataLen, Color color)
{
    HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_SET); // Signals that data being written

    uint8_t data[] = {color.R, color.G, color.B};

    for (uint32_t i = 0; i < dataLen; i++)
    {
        HAL_SPI_Transmit(Spi, data, 3, HAL_MAX_DELAY);
    }
    HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
}

void LCD_Controller::writeCmdToLcd(uint32_t cmd, uint32_t dataLen, uint8_t *data)
{
    writeCmdToLcd(cmd);
    for (uint32_t i = 0; i < dataLen; i++)
    {
        writeDataToLcd(data[i]);
    }
}

void LCD_Controller::writeCmdToLcd(uint8_t cmd, uint8_t data)
{
    writeCmdToLcd(cmd);
    writeDataToLcd(data);
}

void LCD_Controller::HWReset()
{
    HAL_GPIO_WritePin(LcdRst_GPIO, LcdRst_pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(LcdRst_GPIO, LcdRst_pin, GPIO_PIN_SET);
    HAL_Delay(50);
}

void LCD_Controller::SetWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t xEnd = x + w;
    uint8_t xdata[4] = {(uint8_t)(x >> 8), (uint8_t)(0xFF & x), (uint8_t)(xEnd >> 8), (uint8_t)(xEnd & 0xff)};
    writeCmdToLcd(COL_RANGE_SET, 4, (uint8_t *)&xdata);

    uint16_t yEnd = y + h;
    uint8_t ydata[4] = {(uint8_t)(y >> 8), (uint8_t)(0xFF & y), (uint8_t)(yEnd >> 8), (uint8_t)(yEnd & 0xff)};
    writeCmdToLcd(ROW_RANGE_SET, 4, (uint8_t *)&ydata);
}

void LCD_Controller::WriteMem(uint16_t len, uint8_t *data)
{
    writeCmdToLcd(MEM_SET, len, data);
}

void LCD_Controller::WriteMemSingleCol(uint32_t len, Color color)
{
    writeCmdToLcd(MEM_SET);
    writeColorDataToLcd(len, color);
}
void LCD_Controller::SWReset()
{
    // writeCmdToLcd(SW_RST);
    writeCmdToLcd(SET_PIXEL_FORMAT);
    writeDataToLcd(0x66); // RGB666

    writeCmdToLcd(SET_SPI_CONF);
    writeDataToLcd(0x80); // disable MISO pin

    writeCmdToLcd(SET_SPI_CONF); // Why twice, Dont know
    writeDataToLcd(0x80);        // disable MISO pin

    writeCmdToLcd(SLEEP_OUT);
    HAL_Delay(120);

    writeCmdToLcd(DIS_ON);
    HAL_Delay(120);
}

void LCD_Controller::DisOn()
{
    writeCmdToLcd(DIS_ON);
}

void LCD_Controller::DisOff()
{
    LCD_Controller::writeCmdToLcd(DIS_OFF);
}

void LCD_Controller::LCD_Dir(LCD_Dir_t direction)
{
    // xWinCmd = 0x2A;
    // yWinCmd = 0x2B;
    // // lcddev.wramcmd = 0x2C;
    switch (direction)
    {
    case up:
        xSize = LCD_W;
        ySize = LCD_H;
        writeCmdToLcd(MADCTL_CMD, Dir_MADCTL_0DEG); // BGR==1,MY==0,MX==0,MV==0
        break;
    case right:
        xSize = LCD_H;
        ySize = LCD_W;
        writeCmdToLcd(MADCTL_CMD, Dir_MADCTL_90DEG); // BGR==1,MY==1,MX==0,MV==1
        break;
    case down:
        xSize = LCD_W;
        ySize = LCD_H;
        writeCmdToLcd(MADCTL_CMD, Dir_MADCTL_180DEG); // BGR==1,MY==0,MX==0,MV==0
        break;
    case left:
        xSize = LCD_H;
        ySize = LCD_W;
        writeCmdToLcd(MADCTL_CMD, Dir_MADCTL_270DEG); // BGR==1,MY==1,MX==0,MV==1
        break;
    default:
        break;
    }
}

LCD_Controller::LCD_Controller(
    SPI_HandleTypeDef *Spi,
    GPIO_TypeDef *DataComSel_GPIO,
    uint16_t DataComSel_pin,
    GPIO_TypeDef *chipSel_GPIO,
    uint16_t chipSel_pin,
    GPIO_TypeDef *LcdRst_GPIO,
    uint16_t LcdRst_pin,
    GPIO_TypeDef *LcdBackLed_GPIO,
    uint16_t LcdBackLed_pin) : Spi(Spi),
                               DataComSel_GPIO(DataComSel_GPIO),
                               DataComSel_pin(DataComSel_pin),
                               chipSel_GPIO(chipSel_GPIO),
                               chipSel_pin(chipSel_pin),
                               LcdRst_GPIO(LcdRst_GPIO),
                               LcdRst_pin(LcdRst_pin),
                               LcdBackLed_GPIO(LcdBackLed_GPIO),
                               LcdBackLed_pin(LcdBackLed_pin)
{
}

void LCD_Controller::initLcd()
{
    HWReset();
    SWReset();
    BackLightEnable();
    LCD_Dir(up);
}

void LCD_Controller::BackLightEnable()
{
    HAL_GPIO_WritePin(LcdBackLed_GPIO, LcdBackLed_pin, GPIO_PIN_SET);
}
void LCD_Controller::BackLightDisable()
{
    HAL_GPIO_WritePin(LcdBackLed_GPIO, LcdBackLed_pin, GPIO_PIN_RESET);
}
void LCD_Controller::Inv()
{
    writeCmdToLcd(DIS_INV_ON);
}
void LCD_Controller::UnInv()
{
    writeCmdToLcd(DIS_INV_OFF);
}
void LCD_Controller::drawImage(uint16_t x, uint16_t y, ImageData image)
{
    SetWindow(x, y, image.width - 1, image.height - 1);
    writeCmdToLcd(MEM_SET);
    writeLUTDataToLcd((image.width * image.height) / 2, image.data, image.lut);
    // WriteMemSingleCol(w * h, color);
}

void LCD_Controller::drawRec(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color color)
{
    SetWindow(x, y, w, h);
    WriteMemSingleCol(w * h, color);
}
void LCD_Controller::clearFrame(Color color)
{
    drawRec(0, 0, xSize, ySize, color);
}

ColorField::ColorField(Color color) : value(color) {}

ColorField::ColorField(IQueue *que)
{
    value.R = que->pop();
    value.G = que->pop();
    value.B = que->pop();
}

uint16_t ColorField::getWireSize()
{
    return 3;
}

void ColorField::appendToQue(IQueue *que)
{
    que->append(value.R);
    que->append(value.G);
    que->append(value.B);
}

Color ColorField::getColor() { return value; }
