
#include "zHal.h"
#include "shiftReg.hpp"
ShiftReg::ShiftReg(uint16_t _chipDepth,
                   GPIO_TypeDef *_clkGPIO,
                   uint16_t _clkPin,
                   GPIO_TypeDef *_latGPIO,
                   uint16_t _latPin,
                   GPIO_TypeDef *_datGPIO,
                   uint16_t _datPin) : chipDepth(_chipDepth),
                                       clkGPIO(_clkGPIO),
                                       clkPin(_clkPin),
                                       latGPIO(_latGPIO),
                                       latPin(_latPin),
                                       datGPIO(_datGPIO),
                                       datPin(_datPin)
{
    data = new uint16_t[chipDepth];
    stagedData = new uint16_t[chipDepth];
    HAL_GPIO_WritePin(clkGPIO, clkPin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(latGPIO, latPin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(datGPIO, datPin, GPIO_PIN_RESET);
}

ShiftReg::~ShiftReg()
{
    delete data;
    delete stagedData;
    HAL_GPIO_WritePin(clkGPIO, clkPin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(latGPIO, latPin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(datGPIO, datPin, GPIO_PIN_RESET);
}

void ShiftReg::setChipMask(uint16_t chipIndex, uint16_t mask)
{
    data[chipIndex] = mask;
    changed = true;
}

void ShiftReg::asyncUpdate()
{
    switch (state)
    {
    case idle:
        if (changed)
        {
            changed = false;
            for (uint16_t i = 0; i < chipDepth; i++)
            {
                stagedData[i] = data[i];
            }
            state = writing;
        }
        break;
    case latch:
        if (lat)
        {
            HAL_GPIO_WritePin(latGPIO, latPin, GPIO_PIN_RESET);
            lat = false;
            state = idle;
        }
        else
        {
            HAL_GPIO_WritePin(latGPIO, latPin, GPIO_PIN_SET);
            lat = true;
        }
        break;
    case writing:
        if (clk)
        {
            HAL_GPIO_WritePin(clkGPIO, clkPin, GPIO_PIN_RESET);
            if (this->place < chipWidth * chipDepth)
            {
                uint16_t chip = this->place / chipWidth;
                uint16_t chipBit = this->place % chipWidth;
                if ((stagedData[chip]) & (1 << chipBit))
                {
                    HAL_GPIO_WritePin(datGPIO, datPin, GPIO_PIN_SET);
                }
                else
                {
                    HAL_GPIO_WritePin(datGPIO, datPin, GPIO_PIN_RESET);
                }
                this->place++;
            }
            else
            {
                this->place = 0;
                state = latch;
            }
        }
        else
        {
            HAL_GPIO_WritePin(clkGPIO, clkPin, GPIO_PIN_SET);
        }
        clk = !clk;
        break;
    }
}

bool ShiftReg::busy(){
    return (state != idle);
}


void ShiftReg::shiftLeft(){
    for (uint16_t i = 0; i < chipDepth; i++)
    {
        data[i] = data[i] << 1;
    }
    this->changed = true;
}

void ShiftReg::setBit(uint16_t chipId, uint16_t bit){
    data[chipId] = data[chipId] | (1<<bit);
    this->changed = true;
}

void ShiftReg::resetBit(uint16_t chipId, uint16_t bit){
    data[chipId] = data[chipId] & ~(1<<bit);
    this->changed = true;
}