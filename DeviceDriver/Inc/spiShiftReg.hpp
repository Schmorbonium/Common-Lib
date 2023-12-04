#ifndef __SPI_SHIFT_REG_H
#define __SPI_SHIFT_REG_H

#include "zHal.h"

class SpiShiftReg_Core
{
    SPI_HandleTypeDef *ledSpi;
    TIM_HandleTypeDef *pwmTim;
    uint32_t pwmTimChn;
    GPIO_TypeDef *latchGpio;
    uint16_t latchPin;
    uint16_t *stagedData;
    bool changePending = true;
    bool spiActive = false;

protected:
    uint16_t chipDepth;
    void latchData();
    void setPWM(uint8_t newValue);
    void FeedDataOverSpi(uint16_t *data);

public:
    SpiShiftReg_Core(uint16_t _chipDepth,
                     SPI_HandleTypeDef *_hspi,
                     TIM_HandleTypeDef *_pwmTim,
                     uint32_t tim_channel,
                     GPIO_TypeDef *_latchGpio,
                     uint16_t _latchPin);

    virtual ~SpiShiftReg_Core();

    // Delete copy constructor and copy assignment operator
    SpiShiftReg_Core(const SpiShiftReg_Core&) = delete;
    SpiShiftReg_Core& operator=(const SpiShiftReg_Core&) = delete;

    // Delete move constructor and move assignment operator
    SpiShiftReg_Core(SpiShiftReg_Core&&) = delete;
    SpiShiftReg_Core& operator=(SpiShiftReg_Core&&) = delete;

    void startup();
    virtual void DrawTick();
};

class DirectSpiShiftReg : public SpiShiftReg_Core
{

protected:
    uint16_t *data;
    void refreshDisplay();

public:
    DirectSpiShiftReg(uint16_t _chipDepth,
                      SPI_HandleTypeDef *_hspi,
                      TIM_HandleTypeDef *_pwmTim,
                      uint32_t tim_channel,
                      GPIO_TypeDef *_latchGpio,
                      uint16_t _latchPin);
    virtual ~DirectSpiShiftReg();
    // Delete copy constructor and copy assignment operator
    DirectSpiShiftReg(const DirectSpiShiftReg&) = delete;
    DirectSpiShiftReg& operator=(const DirectSpiShiftReg&) = delete;

    // Delete move constructor and move assignment operator
    DirectSpiShiftReg(DirectSpiShiftReg&&) = delete;
    DirectSpiShiftReg& operator=(DirectSpiShiftReg&&) = delete;

    void setChipValue(uint16_t chipIndex, uint16_t value);
    void setBrightness(uint8_t newValue);
    // void shiftLeft();
    void setBit(uint16_t bitIndex);
    void resetBit(uint16_t bitIndex);
    virtual void DrawTick();
};

// Note there should be a dedicated Timer to run MultiFrameShiftReg, this is so an active frame rate can be maintained
class MultiFrameSiftReg : public SpiShiftReg_Core
{
    uint8_t frameDepth;
    uint8_t currentFrame;
    uint16_t *pwmVals;

protected:
    uint16_t *data;
    uint16_t *framePointer(uint8_t frame);
    void refreshDisplay();
    void advanceFrame();

public:
    MultiFrameSiftReg(uint16_t _chipDepth,
                      SPI_HandleTypeDef *_hspi,
                      TIM_HandleTypeDef *_pwmTim,
                      uint32_t tim_channel,
                      GPIO_TypeDef *_latchGpio,
                      uint16_t _latchPin,
                      uint8_t frameDepth);
    virtual ~MultiFrameSiftReg();

    // Delete copy constructor and copy assignment operator
    MultiFrameSiftReg(const MultiFrameSiftReg&) = delete;
    MultiFrameSiftReg& operator=(const MultiFrameSiftReg&) = delete;

    // Delete move constructor and move assignment operator
    MultiFrameSiftReg(MultiFrameSiftReg&&) = delete;
    MultiFrameSiftReg& operator=(MultiFrameSiftReg&&) = delete;


    void setChipValue(uint8_t frame, uint16_t chipIndex, uint16_t value);
    void setBrightness(uint8_t frame, uint8_t newValue);
    virtual void DrawTick();
};

#endif