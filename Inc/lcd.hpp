#ifndef __LCD_CONTROL
#define __LCD_CONTROL

#include "zHal.h"
#include "imageData.h"

#define LCD_W 320
#define LCD_H 480

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
#define SET_PIXEL_FORMAT 0x3A
#define SET_SPI_CONF 0xB0
#define SLEEP_OUT 0x11

#define MADCTL_CMD 0x36
#define Dir_MADCTL_0DEG 0X88
#define Dir_MADCTL_90DEG 0xE8
#define Dir_MADCTL_180DEG 0x48
#define Dir_MADCTL_270DEG 0x28

enum LCD_Dir
{
    up,
    right,
    down,
    left
};

typedef struct
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} Color;
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
        HAL_SPI_Transmit(Spi, &cmd, 1, HAL_MAX_DELAY);
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
    }

    void writeDataToLcd(uint8_t data)
    {
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_SET); // Signals that data being written
        HAL_SPI_Transmit(Spi, &data, 1, HAL_MAX_DELAY);
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
    }

    void writeDataToLcd(uint32_t dataLen, uint8_t *data)
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
    void writeLUTDataToLcd(uint32_t dataLen, uint8_t *data, uint8_t *LUT){
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_SET); // Signals that data being written
        for (uint32_t i = 0; i < dataLen; i++)
        {
            uint8_t*  c1 = &(LUT[((data[i]>>0)&0xF)*4]);
            uint8_t*  c2 = &(LUT[((data[i]>>4)&0xF)*4]);
            HAL_SPI_Transmit(Spi,c2, 3, HAL_MAX_DELAY);
            HAL_SPI_Transmit(Spi,c1, 3, HAL_MAX_DELAY);
        }
        
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
    }
    void writeColorDataToLcd(uint32_t dataLen, Color color)
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

    void writeCmdToLcd(uint32_t cmd, uint32_t dataLen, uint8_t *data)
    {
        writeCmdToLcd(cmd);
        for (uint32_t i = 0; i < dataLen; i++)
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
        writeCmdToLcd(COL_RANGE_SET, 4, (uint8_t *)&xdata);

        uint16_t yEnd = y + h;
        uint8_t ydata[4] = {(uint8_t)(y >> 8), (uint8_t)(0xFF & y), (uint8_t)(yEnd >> 8), (uint8_t)(yEnd & 0xff)};
        writeCmdToLcd(ROW_RANGE_SET, 4, (uint8_t *)&ydata);
    }

    void WriteMem(uint16_t len, uint8_t *data)
    {
        writeCmdToLcd(MEM_SET, len, data);
    }

    void WriteMemSingleCol(uint32_t len, Color color)
    {
        writeCmdToLcd(MEM_SET);
        writeColorDataToLcd(len, color);
    }

    void SWReset()
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



public:

    void DisOn()
    {
        writeCmdToLcd(DIS_ON);
    }

    void DisOff()
    {
        writeCmdToLcd(DIS_OFF);
    }

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
        LCD_Dir(down);
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

    void drawImage(uint16_t x, uint16_t y, ImageData image)
    {
        SetWindow(x, y, image.width-1, image.height-1);
        writeCmdToLcd(MEM_SET);
        writeLUTDataToLcd((image.width*image.height)/2, image.data,image.lut);
        // WriteMemSingleCol(w * h, color);
    }

    void drawRec(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color color)
    {
        SetWindow(x, y, w, h);
        WriteMemSingleCol(w * h, color);
    }
    void clearFrame(Color color)
    {
        drawRec(0, 0, xSize, ySize, color);
    }
};

#endif
