#include "zHal.h"
enum State
{
    writing,
    latch,
    idle
};

class SpiShiftReg
{
    State state = idle;
    uint16_t chipDepth;

    
    SPI_HandleTypeDef *ledSpi;
    TIM_HandleTypeDef *pwmTim;

    uint16_t *stagedData;
    uint32_t place = 0;
    bool changed = 0;
    bool clk = 0;
    bool lat = 0;

    public:
    SpiShiftReg(uint16_t _chipDepth,
             SPI_HandleTypeDef *hspi,
             TIM_HandleTypeDef *pwmTim){
                ledSpi = hspi;
                pwmTim = pwmTim;
             }

    ~SpiShiftReg();
    void setChipMask(uint16_t chipIndex, uint16_t mask);

    void asyncUpdate();
    bool busy();
    void shiftLeft();

    void setBit(uint16_t chipId, uint16_t bit);
    void resetBit(uint16_t chipId, uint16_t bit);
};