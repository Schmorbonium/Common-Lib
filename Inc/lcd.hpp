#ifndef __LCD_CONTROL
#define __LCD_CONTROL

#include "zHal.h"

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
        HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_RESET); // Signals that it is a command being written
        HAL_SPI_Transmit(Spi, &data, 1, 0);
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
    }

    void writeDataToLcd(uint8_t dataMSB, uint8_t dataLSB)
    {
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(DataComSel_GPIO, DataComSel_pin, GPIO_PIN_RESET); // Signals that it is a command being written
        HAL_SPI_Transmit(Spi, &dataMSB, 1, 0);
        HAL_SPI_Transmit(Spi, &dataLSB, 1, 0);
        HAL_GPIO_WritePin(chipSel_GPIO, chipSel_pin, GPIO_PIN_SET);
    }

    void writeCmdToLcd(uint8_t cmd, uint8_t dataLen, uint8_t *data)
    {
        writeCmdToLcd(cmd);
        for (uint8_t i = 0; i < dataLen; i++)
        {
            writeDataToLcd(data[i]);
        }
    }

    void HWReset()
    {
        HAL_GPIO_WritePin(LcdRst_GPIO, LcdRst_pin, GPIO_PIN_RESET);
        HAL_Delay(100);
        HAL_GPIO_WritePin(LcdRst_GPIO, LcdRst_pin, GPIO_PIN_SET);
        HAL_Delay(50);
    }
    void SWReset()
    {
        writeCmdToLcd(SW_RST);
        HAL_Delay(120);
        writeCmdToLcd(0xCF); // Power Control B
        writeDataToLcd(0x00);                       //
        writeDataToLcd(0xC9);                       // C1
        writeDataToLcd(0X30);
        writeCmdToLcd(0xED); // Power On sequence control
        writeDataToLcd(0x64);
        writeDataToLcd(0x03);
        writeDataToLcd(0X12);
        writeDataToLcd(0X81);
        writeCmdToLcd(0xE8); // Driver timing control A
        writeDataToLcd(0x85);
        writeDataToLcd(0x10);
        writeDataToLcd(0x7A);
        writeCmdToLcd(0xCB); // Power Control A
        writeDataToLcd(0x39);   // Default
        writeDataToLcd(0x2C);   // Default
        writeDataToLcd(0x00);   // Default
        writeDataToLcd(0x34);   // Default
        writeDataToLcd(0x02);   // Default
        writeCmdToLcd(0xF7); // Pump ratio control
        writeDataToLcd(0x20);
        writeCmdToLcd(0xEA); // Driver timing control B
        writeDataToLcd(0x00);
        writeDataToLcd(0x00);
        writeCmdToLcd(0xC0); // Power control
        writeDataToLcd(0x1B);   // VRH[5:0]  -> 4.2V ?
        writeCmdToLcd(0xC1); // Power control
        writeDataToLcd(0x00);   // SAP[2:0];BT[3:0] 01
        writeCmdToLcd(0xC5); // VCM control
        writeDataToLcd(0x30);   // 3F
        writeDataToLcd(0x30);   // 3C
        writeCmdToLcd(0xC7); // VCM control2
        writeDataToLcd(0XB7);
        writeCmdToLcd(0x36); // Memory Access Control
        writeDataToLcd(0x08);
        writeCmdToLcd(0x3A); // Pixel Format
        writeDataToLcd(0x55);   // 16Bit Dpi 16bit Dbi
        writeCmdToLcd(0xB1); // Frame Rate Control
        writeDataToLcd(0x00);   // No Div
        writeDataToLcd(0x1A);   // 73 Hz
        writeCmdToLcd(0xB6); // Display Function Control
        writeDataToLcd(0x0A);   // internal scan -- A_Gnd Default
        writeDataToLcd(0xA2);
        writeCmdToLcd(0xF2); // 3Gamma Function Disable
        writeDataToLcd(0x00);
        writeCmdToLcd(0x26); // Gamma curve selected
        writeDataToLcd(0x01);
        writeCmdToLcd(0xE0); // Set Gamma
        writeDataToLcd(0x0F);
        writeDataToLcd(0x2A);
        writeDataToLcd(0x28);
        writeDataToLcd(0x08);
        writeDataToLcd(0x0E);
        writeDataToLcd(0x08);
        writeDataToLcd(0x54);
        writeDataToLcd(0XA9);
        writeDataToLcd(0x43);
        writeDataToLcd(0x0A);
        writeDataToLcd(0x0F);
        writeDataToLcd(0x00);
        writeDataToLcd(0x00);
        writeDataToLcd(0x00);
        writeDataToLcd(0x00);
        writeCmdToLcd(0XE1); // Set Gamma
        writeDataToLcd(0x00);
        writeDataToLcd(0x15);
        writeDataToLcd(0x17);
        writeDataToLcd(0x07);
        writeDataToLcd(0x11);
        writeDataToLcd(0x06);
        writeDataToLcd(0x2B);
        writeDataToLcd(0x56);
        writeDataToLcd(0x3C);
        writeDataToLcd(0x05);
        writeDataToLcd(0x10);
        writeDataToLcd(0x0F);
        writeDataToLcd(0x3F);
        writeDataToLcd(0x3F);
        writeDataToLcd(0x0F);
        writeCmdToLcd(0x2B); // Set Page Address
        writeDataToLcd(0x00);
        writeDataToLcd(0x00);
        writeDataToLcd(0x01);
        writeDataToLcd(0x3f);
        writeCmdToLcd(0x2A); // Set Column Address
        writeDataToLcd(0x00);
        writeDataToLcd(0x00);
        writeDataToLcd(0x00);
        writeDataToLcd(0xef);
        writeCmdToLcd(0x11); // Exit Sleep
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

    void writeImageDate(uint16_t uint16_t, uint8_t *frame)
    {
        HAL_GPIO_WritePin(LcdBackLed_GPIO, LcdBackLed_pin, GPIO_PIN_RESET);
    }
};

#endif
