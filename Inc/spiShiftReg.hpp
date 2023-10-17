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
    void startup();
    virtual void DrawTick();
};

class DirectSpiShiftReg : public SpiShiftReg_Core
{
    uint16_t *data;

protected:
    void refreshDisplay();

public:
    DirectSpiShiftReg(uint16_t _chipDepth,
                      SPI_HandleTypeDef *_hspi,
                      TIM_HandleTypeDef *_pwmTim,
                      uint32_t tim_channel,
                      GPIO_TypeDef *_latchGpio,
                      uint16_t _latchPin);
    virtual ~DirectSpiShiftReg();
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
    uint16_t *data;
    uint8_t frameDepth;
    uint8_t currentFrame;
    uint16_t *pwmVals;

protected:
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
    void setChipValue(uint8_t frame, uint16_t chipIndex, uint16_t value);
    void setBrightness(uint8_t frame, uint8_t newValue);
    virtual void DrawTick();
};