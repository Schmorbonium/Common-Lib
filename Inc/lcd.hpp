#ifndef __LCD_CONTROL
#define __LCD_CONTROL

#include "zHal.h"
#include "imageData.h"
#include "bufferedUart.hpp"

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

enum LCD_Dir_t
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

typedef struct
{
    uint16_t x,y,w,h;
} BoundingBox_t;

class IDrawable {
    public:
    virtual BoundingBox_t GetBoundingBox();
    virtual void draw(Color* colorBuffer, uint32_t StartIndex, uint32_t RequestSize);
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

    void writeCmdToLcd(uint8_t cmd);
    void writeDataToLcd(uint8_t data);
    void writeDataToLcd(uint32_t dataLen, uint8_t *data);
    void writeLUTDataToLcd(uint32_t dataLen, uint8_t *data, uint8_t *LUT);
    void writeColorDataToLcd(uint32_t dataLen, Color color);
    void writeCmdToLcd(uint32_t cmd, uint32_t dataLen, uint8_t *data);
    void writeCmdToLcd(uint8_t cmd, uint8_t data);
    void HWReset();
    void SetWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void WriteMem(uint16_t len, uint8_t *data);
    void WriteMemSingleCol(uint32_t len, Color color);
    void SWReset();

public:
    void DisOn();
    void DisOff();

    void LCD_Dir(LCD_Dir_t direction);

    LCD_Controller(
        SPI_HandleTypeDef *Spi,
        GPIO_TypeDef *DataComSel_GPIO,
        uint16_t DataComSel_pin,
        GPIO_TypeDef *chipSel_GPIO,
        uint16_t chipSel_pin,
        GPIO_TypeDef *LcdRst_GPIO,
        uint16_t LcdRst_pin,
        GPIO_TypeDef *LcdBackLed_GPIO,
        uint16_t LcdBackLed_pin);
    void initLcd();
    void BackLightEnable();
    void BackLightDisable();
    void Inv();
    void UnInv();
    void drawImage(uint16_t x, uint16_t y, ImageData image);
    void drawRec(uint16_t x, uint16_t y, uint16_t w, uint16_t h, Color color);
    void clearFrame(Color color);
};



class ColorField : public ISendable
{
public:
    Color value;
    // Uint8Field r,g,b;

    ColorField(Color color);
    
    ColorField(CharBuffer *que);
    uint16_t getWireSize();
    virtual void appendToQue(CharBuffer *que);
    Color getColor();
};


#endif
