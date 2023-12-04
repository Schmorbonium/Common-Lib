#include "zHal.h"
#include "iBasicQue.hpp"

void I_Basic_Que::append(uint8_t c) {

}
uint8_t I_Basic_Que::pop() {
    return 0;
}
uint8_t I_Basic_Que::peak(uint16_t) const {
    return 0;
}

void I_Basic_Que::append_uint16(uint16_t c)
{
    uint8_t lsb = c & 0xFF; 
    uint8_t msb = (c >> 8) & 0xFF; 
    this->append(lsb);
    this->append(msb);
}

uint16_t I_Basic_Que::pop_uint16()
{
    uint16_t lsb = this->pop();
    uint16_t msb = this->pop();
    return (lsb | (msb<<8));
}

uint16_t I_Basic_Que::peak_uint16(uint16_t startingIndex) const
{
    uint16_t lsb = this->peak(startingIndex);
    uint16_t msb = this->peak(startingIndex+1);
    return (lsb | (msb<<8));
}

uint16_t I_Basic_Que::peak_uint16() const
{
    return peak_uint16(0);
}

void I_Basic_Que::append_uint32(uint32_t c)
{
    uint16_t lsW = c & 0xFFFF; 
    uint16_t msW = (c >> 16) & 0xFFFF; 
    this->append_uint16(lsW);
    this->append_uint16(msW);
}

uint32_t I_Basic_Que::pop_uint32()
{
    uint32_t lsW = pop_uint16(); 
    uint32_t msW = pop_uint16(); 
    return (lsW | msW << 16);
}

uint32_t I_Basic_Que::peak_uint32() const
{
    uint16_t lsW = this->peak_uint16(0);
    uint16_t msW = this->peak_uint16(2);
    return (lsW | (msW<<16));
}