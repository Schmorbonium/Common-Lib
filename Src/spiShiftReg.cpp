#include "zHal.h"
#include "spiShiftReg.hpp"

void SpiShiftReg_Core::latchData()
{
    HAL_GPIO_WritePin(latchGpio, latchPin, GPIO_PIN_SET);
    // HAL_Delay(1);
    HAL_GPIO_WritePin(latchGpio, latchPin, GPIO_PIN_RESET);
}

void SpiShiftReg_Core::setPWM(uint8_t newValue)
{
    pwmTim->Instance->CCR4 = newValue;
}

void SpiShiftReg_Core::FeedDataOverSpi(uint16_t *data)
{
    for (uint16_t i = 0; i < chipDepth; i++)
    {
        stagedData[i] = data[i];
    }
    changePending = false;
    HAL_SPI_Transmit(ledSpi,
                     (uint8_t *)this->stagedData,
                     this->chipDepth,
                     HAL_MAX_DELAY);
    latchData();
}

SpiShiftReg_Core::SpiShiftReg_Core(
    uint16_t _chipDepth,
    SPI_HandleTypeDef *_hspi,
    TIM_HandleTypeDef *_pwmTim,
    uint32_t tim_channel,
    GPIO_TypeDef *_latchGpio,
    uint16_t _latchPin) : ledSpi(_hspi),
                          pwmTim(_pwmTim),
                          pwmTimChn(tim_channel),
                          latchGpio(_latchGpio),
                          latchPin(_latchPin),
                          stagedData(new uint16_t[_chipDepth]),
                          chipDepth(_chipDepth)
{
}

SpiShiftReg_Core::~SpiShiftReg_Core()
{
    delete[] stagedData;
}

void SpiShiftReg_Core::startup()
{
    HAL_TIM_PWM_Start(pwmTim, pwmTimChn);
}

void SpiShiftReg_Core::DrawTick() {}

void DirectSpiShiftReg::refreshDisplay()
{
    FeedDataOverSpi(data);
}

DirectSpiShiftReg::DirectSpiShiftReg(
    uint16_t _chipDepth,
    SPI_HandleTypeDef *_hspi,
    TIM_HandleTypeDef *_pwmTim,
    uint32_t tim_channel,
    GPIO_TypeDef *_latchGpio,
    uint16_t _latchPin) : SpiShiftReg_Core(_chipDepth,
                                           _hspi,
                                           _pwmTim,
                                           tim_channel,
                                           _latchGpio,
                                           _latchPin)
{
    data = new uint16_t[_chipDepth];
}
DirectSpiShiftReg::~DirectSpiShiftReg()
{
    delete[] data;
}

void DirectSpiShiftReg::setChipValue(uint16_t chipIndex, uint16_t value)
{
    data[chipIndex] = value;
}

void DirectSpiShiftReg::setBrightness(uint8_t newValue)
{
    setPWM(newValue);
}

// void shiftLeft();
void DirectSpiShiftReg::setBit(uint16_t bitIndex)
{
    uint16_t chipId = bitIndex / 16;
    uint16_t bit = bitIndex % 16;

    data[chipId] |= (1 << bit);
}

void DirectSpiShiftReg::resetBit(uint16_t bitIndex)
{
    uint16_t chipId = bitIndex / 16;
    uint16_t bit = bitIndex % 16;

    data[chipId] &= ~(1 << bit);
}

void DirectSpiShiftReg::DrawTick()
{
    this->refreshDisplay();
}

// Note there should be a dedicated Timer to run MultiFrameShiftReg, this is so an active frame rate can be maintained
uint16_t *MultiFrameSiftReg::framePointer(uint8_t frame)
{
    return &(data[this->chipDepth * frame]);
}
void MultiFrameSiftReg::refreshDisplay()
{
    uint16_t *FramePointer = framePointer(currentFrame);
    FeedDataOverSpi(FramePointer);
}

void MultiFrameSiftReg::advanceFrame()
{
    currentFrame++;
    if (currentFrame >= frameDepth)
    {
        currentFrame = 0;
    }
}

MultiFrameSiftReg::MultiFrameSiftReg(
    uint16_t _chipDepth,
    SPI_HandleTypeDef *_hspi,
    TIM_HandleTypeDef *_pwmTim,
    uint32_t tim_channel,
    GPIO_TypeDef *_latchGpio,
    uint16_t _latchPin,
    uint8_t frameDepth) : SpiShiftReg_Core(_chipDepth,
                                           _hspi,
                                           _pwmTim,
                                           tim_channel,
                                           _latchGpio,
                                           _latchPin),
                          frameDepth(frameDepth)
{
    data = new uint16_t[_chipDepth * frameDepth];
    pwmVals = new uint16_t[frameDepth];
}

MultiFrameSiftReg::~MultiFrameSiftReg()
{
    delete[] data;
    delete[] pwmVals;
}

void MultiFrameSiftReg::setChipValue(uint8_t frame, uint16_t chipIndex, uint16_t value)
{
    framePointer(frame)[chipIndex] = value;
}

void MultiFrameSiftReg::setBrightness(uint8_t frame, uint8_t newValue)
{
    pwmVals[frame] = newValue;
}

void MultiFrameSiftReg::DrawTick()
{
    this->advanceFrame();
    this->refreshDisplay();
    this->setPWM(pwmVals[currentFrame]);
}