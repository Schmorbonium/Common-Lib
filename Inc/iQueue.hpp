#ifndef __Basic_Que_I__
#define __Basic_Que_I__


#include "zHal.h"


class IQueue
{
public:
    virtual void append(uint8_t c);
    virtual uint8_t pop();
    virtual uint8_t peak(uint16_t) const;

    // Append a new Character to the end of the buffer
    void append_uint16(uint16_t c);
    // Append a new Character to the end of the buffer
    void append_uint32(uint32_t c);
    // Remove and return the first 2 characters in the buffer
    uint16_t pop_uint16();
    // Remove and return the first 4 characters in the buffer
    uint32_t pop_uint32();

    // Peaks at the value at a given index

    uint16_t peak_uint16(uint16_t) const;
    uint16_t peak_uint16() const;
    uint32_t peak_uint32() const;
};

class ISendable
{
public:
    virtual void appendToQue(IQueue *que);
    virtual uint16_t getWireSize() ;
};
#endif