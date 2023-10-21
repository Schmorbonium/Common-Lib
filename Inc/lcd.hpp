#ifndef __LCD_CONTROL
#define __LCD_CONTROL

#include "zHal.h"

#define LCD_W 320
#define LCD_H 240

#define NO_OP 0x00
#define SW_RST 0x01

#define COL_RANGE_SET 0x2A
#define ROW_RANGE_SET 0x2B

#define MEM_SET 0x2C
#define MEM_READ 0x2h

#define DIS_INV_OFF 0x20
#define DIS_INV_ON 0x21

#define DIS_OFF 0x28
#define DIS_ON 0x29

#define COLOR_LUT_SET 0x2D

enum LCD_Dir
{
    up,
    right,
    down,
    left
};
class LCD_Controller
{
private:
    SPI_HandleTypeDef *Spi;

    GPIO_TypeDef *DataComSel_GPIO;
    uint16_t DataComSel_pin;

    GPIO_TypeDef *chipSel_GPIO;
    uint16_t chipSel_pin;

    GPIO_TypeDef *LcdRst_GPIO;
    uint16_t LcdRst_pin;

    GPIO_TypeDef *LcdBackLed_GPIO;
    uint16_t LcdBackLed_pin;

    uint16_t xSize;
    uint16_t ySize;

    void writeCmdToLcd(uint8_t cmd)
    {
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_RESET); // Signals that it is a command being written
        HAL_SPI_Transmit(Spi, &cmd, 1, 0);
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
    }

    void writeDataToLcd(uint8_t data)
    {
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_SET); // Signals that data being written
        HAL_SPI_Transmit(Spi, &data, 1, 0);
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
    }

    void writeDataToLcd(uint16_t dataLen, uint8_t* dataLSB)
    {
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_SET); // Signals that data being written
        HAL_SPI_Transmit(Spi, dataLSB, dataLen, 0);
        // HAL_SPI_Transmit(Spi, &dataLSB, 1, 0);
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
    }

    void writeColorDataToLcd(uint16_t dataLen, uint16_t color)
    {
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_SET); // Signals that data being written
        uint8_t MSB = (uint8_t)(color>>8);
        uint8_t LSB = (uint8_t)(color&0xff);
        
        for (uint16_t i = 0; i < dataLen; i++)
        {
            HAL_SPI_Transmit(Spi, &MSB, 1, 0);
            HAL_SPI_Transmit(Spi, &LSB, 1, 0);
        }
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
    }

    void writeCmdToLcd(uint8_t cmd, uint16_t dataLen, uint8_t *data)
    {
        writeCmdToLcd(cmd);
        for (uint16_t i = 0; i < dataLen; i++)
        {
            writeDataToLcd(data[i]);
        }
    }

    void writeCmdToLcd(uint8_t cmd, uint8_t data)
    {
        writeCmdToLcd(cmd);
        writeDataToLcd(data);
    }

    void HWReset()
    {
        HAL_GPIO_WritePin(LcdRst_GPIO, LcdRst_pin, GPIO_PIN_RESET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LcdRst_GPIO, LcdRst_pin, GPIO_PIN_SET);
        HAL_Delay(50);
    }

    void SetWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
    {
        uint16_t xEnd = x + w;
        uint8_t xdata[4] = {(uint8_t)(x >> 8), (uint8_t)(0xFF & x), (uint8_t)(xEnd >> 8), (uint8_t)(xEnd & 0xff)};
        writeCmdToLcd(ROW_RANGE_SET, 4, (uint8_t *)&xdata);
        // LCD_WR_REG(ROW_RANGE_SET);
        // LCD_WR_DATA(x >> 8);
        // LCD_WR_DATA(0x00FF & x);
        // LCD_WR_DATA(xEnd >> 8);
        // LCD_WR_DATA(0x00FF & xEnd);

        uint16_t yEnd = y + h;
        uint8_t ydata[4] = {(uint8_t)(y >> 8), (uint8_t)(0xFF & y), (uint8_t)(yEnd >> 8), (uint8_t)(yEnd & 0xff)};
        writeCmdToLcd(ROW_RANGE_SET, 4, (uint8_t *)&ydata);
        // LCD_WR_REG(COL_RANGE_SET);
        // LCD_WR_DATA(yStar >> 8);
        // LCD_WR_DATA(0x00FF & yStar);
        // LCD_WR_DATA(yEnd >> 8);
        // LCD_WR_DATA(0x00FF & yEnd);
    }

    void WriteMem(uint16_t len, uint8_t *data)
    {
        writeCmdToLcd(MEM_SET, len, data);
    }

    void WriteMemSingleCol(uint16_t len, uint16_t color)
    {
        writeCmdToLcd(MEM_SET);
        writeColorDataToLcd(len,color);
    }

    void SWReset()
    {
        writeCmdToLcd(SW_RST);
        HAL_Delay(120);
        // writeCmdToLcd(0xCF);  // Power Control B
        // writeDataToLcd(0x00); //
        // writeDataToLcd(0xC9); // C1
        // writeDataToLcd(0X30);
        // writeCmdToLcd(0xED); // Power On sequence control
        // writeDataToLcd(0x64);
        // writeDataToLcd(0x03);
        // writeDataToLcd(0X12);
        // writeDataToLcd(0X81);
        // writeCmdToLcd(0xE8); // Driver timing control A
        // writeDataToLcd(0x85);
        // writeDataToLcd(0x10);
        // writeDataToLcd(0x7A);
        // writeCmdToLcd(0xCB);  // Power Control A
        // writeDataToLcd(0x39); // Default
        // writeDataToLcd(0x2C); // Default
        // writeDataToLcd(0x00); // Default
        // writeDataToLcd(0x34); // Default
        // writeDataToLcd(0x02); // Default
        // writeCmdToLcd(0xF7);  // Pump ratio control
        // writeDataToLcd(0x20);
        // writeCmdToLcd(0xEA); // Driver timing control B
        // writeDataToLcd(0x00);
        // writeDataToLcd(0x00);
        // writeCmdToLcd(0xC0);  // Power control
        // writeDataToLcd(0x1B); // VRH[5:0]  -> 4.2V ?
        // writeCmdToLcd(0xC1);  // Power control
        // writeDataToLcd(0x00); // SAP[2:0];BT[3:0] 01
        // writeCmdToLcd(0xC5);  // VCM control
        // writeDataToLcd(0x30); // 3F
        // writeDataToLcd(0x30); // 3C
        // writeCmdToLcd(0xC7);  // VCM control2
        // writeDataToLcd(0XB7);
        // writeCmdToLcd(0x36); // Memory Access Control
        // writeDataToLcd(0x08);
        // writeCmdToLcd(0x3A);  // Pixel Format
        // writeDataToLcd(0x55); // 16Bit Dpi 16bit Dbi
        // writeCmdToLcd(0xB1);  // Frame Rate Control
        // writeDataToLcd(0x00); // No Div
        // writeDataToLcd(0x1A); // 73 Hz
        // writeCmdToLcd(0xB6);  // Display Function Control
        // writeDataToLcd(0x0A); // internal scan -- A_Gnd Default
        // writeDataToLcd(0xA2);
        // writeCmdToLcd(0xF2); // 3Gamma Function Disable
        // writeDataToLcd(0x00);
        // writeCmdToLcd(0x26); // Gamma curve selected
        // writeDataToLcd(0x01);
        // writeCmdToLcd(0xE0); // Set Gamma
        // writeDataToLcd(0x0F);
        // writeDataToLcd(0x2A);
        // writeDataToLcd(0x28);
        // writeDataToLcd(0x08);
        // writeDataToLcd(0x0E);
        // writeDataToLcd(0x08);
        // writeDataToLcd(0x54);
        // writeDataToLcd(0XA9);
        // writeDataToLcd(0x43);
        // writeDataToLcd(0x0A);
        // writeDataToLcd(0x0F);
        // writeDataToLcd(0x00);
        // writeDataToLcd(0x00);
        // writeDataToLcd(0x00);
        // writeDataToLcd(0x00);
        // writeCmdToLcd(0XE1); // Set Gamma
        // writeDataToLcd(0x00);
        // writeDataToLcd(0x15);
        // writeDataToLcd(0x17);
        // writeDataToLcd(0x07);
        // writeDataToLcd(0x11);
        // writeDataToLcd(0x06);
        // writeDataToLcd(0x2B);
        // writeDataToLcd(0x56);
        // writeDataToLcd(0x3C);
        // writeDataToLcd(0x05);
        // writeDataToLcd(0x10);
        // writeDataToLcd(0x0F);
        // writeDataToLcd(0x3F);
        // writeDataToLcd(0x3F);
        // writeDataToLcd(0x0F);
        // writeCmdToLcd(0x2B); // Set Page Address
        // writeDataToLcd(0x00);
        // writeDataToLcd(0x00);
        // writeDataToLcd(0x01);
        // writeDataToLcd(0x3f);
        // writeCmdToLcd(0x2A); // Set Column Address
        // writeDataToLcd(0x00);
        // writeDataToLcd(0x00);
        // writeDataToLcd(0x00);
        // writeDataToLcd(0xef);
        // writeCmdToLcd(0x11); // Exit Sleep
        HAL_Delay(120);
        writeCmdToLcd(0x29); // display on
    }

    void DisOn()
    {
        writeCmdToLcd(DIS_ON);
    }

    void DisOff()
    {
        writeCmdToLcd(DIS_OFF);
    }

public:
    void LCD_Dir(LCD_Dir direction)
    {
        // xWinCmd = 0x2A;
        // yWinCmd = 0x2B;
        // // lcddev.wramcmd = 0x2C;
        switch (direction)
        {
        case up:
            xSize = LCD_W;
            ySize = LCD_H;
            writeCmdToLcd(0x36, (1 << 3) | (0 << 6) | (0 << 7)); // BGR==1,MY==0,MX==0,MV==0
            break;
        case right:
            xSize = LCD_H;
            ySize = LCD_W;
            writeCmdToLcd(0x36, (1 << 3) | (0 << 7) | (1 << 6) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
            break;
        case down:
            xSize = LCD_W;
            ySize = LCD_H;
            writeCmdToLcd(0x36, (1 << 3) | (1 << 6) | (1 << 7)); // BGR==1,MY==0,MX==0,MV==0
            break;
        case left:
            xSize = LCD_H;
            ySize = LCD_W;
            writeCmdToLcd(0x36, (1 << 3) | (1 << 7) | (1 << 5)); // BGR==1,MY==1,MX==0,MV==1
            break;
        default:
            break;
        }
    }

    LCD_Controller(
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

    void initLcd()
    {
        HWReset();
        SWReset();
        BackLightEnable();
    }

    void BackLightEnable()
    {
        HAL_GPIO_WritePin(LcdBackLed_GPIO, LcdBackLed_pin, GPIO_PIN_SET);
    }
    void BackLightDisable()
    {
        HAL_GPIO_WritePin(LcdBackLed_GPIO, LcdBackLed_pin, GPIO_PIN_RESET);
    }
    void Inv()
    {
        writeCmdToLcd(DIS_INV_ON);
    }
    void UnInv()
    {
        writeCmdToLcd(DIS_INV_OFF);
    }

    // void writeImageDate(uint16_t x, uint16_t y,uint16_t w, uint16_t h, uint8_t *frame)
    // {
    //     HAL_GPIO_WritePin(LcdBackLed_GPIO, LcdBackLed_pin, GPIO_PIN_RESET);
    // }

    void drawRec(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
    {
        SetWindow(x, y, w, h);
        WriteMemSingleCol(w * h, color);
    }
};

#endif
