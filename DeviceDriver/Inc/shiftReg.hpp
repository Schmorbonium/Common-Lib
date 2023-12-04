
#include "zHal.h"
enum State
{
    writing,
    latch,
    idle
};

class ShiftReg
{
    State state = idle;
    uint16_t chipDepth;
    uint16_t chipWidth = 16;

    GPIO_TypeDef *clkGPIO;
    uint16_t clkPin;
    GPIO_TypeDef *latGPIO;
    uint16_t latPin;
    GPIO_TypeDef *datGPIO;
    uint16_t datPin;

    uint16_t *data;
    uint16_t *stagedData;
    uint32_t place = 0;
    bool changed = 0;
    bool clk = 0;
    bool lat = 0;

    public:
    ShiftReg(uint16_t _chipDepth,
             GPIO_TypeDef *_clkGPIO,
             uint16_t _clkPin,
             GPIO_TypeDef *_latGPIO,
             uint16_t _latPin,
             GPIO_TypeDef *_datGPIO,
             uint16_t _datPin);

    ~ShiftReg();
    void setChipMask(uint16_t chipIndex, uint16_t mask);

    void asyncUpdate();
    bool busy();
    void shiftLeft();

    void setBit(uint16_t chipId, uint16_t bit);
    void resetBit(uint16_t chipId, uint16_t bit);
};