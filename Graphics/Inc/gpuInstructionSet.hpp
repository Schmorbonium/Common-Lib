#ifndef __GPU_Inst__
#define __GPU_Inst__
#include "isaacsTypes.hpp"
#include "uartData.hpp"
#include "lcd.hpp"
#include "uartPacket.hpp"



class GpuInstSet : public PacketField
{
    ArrayList<Gpu_Inst> set;
};

public:
    Uint32ArrayFeild(uint32_t *_data, uint16_t _len) : len(_len)
    {
        data = new uint32_t[len.data];
        for (uint16_t i = 0; i < len.data; i++)
        {
            this->data[i] = _data[i];
        }
    }
    Uint32ArrayFeild(IQueue *que) : len(que)
    {
        data = new uint32_t[len.data];
        for (uint16_t i = 0; i < len.data; i++)
        {
            data[i] = que->pop_uint32();
        }
    }
    ~Uint32ArrayFeild()
    {
        delete[] data;
    }
    virtual void parseFromQue(IQueue *que)
    {
        delete[] data;
        len = Uint16Field(que);
        data = new uint32_t[len.data];

        for (uint16_t i = 0; i < len.data; i++)
        {
            data[i] = que->pop_uint32();
        }
    }
    virtual void appendToQue(IQueue *que)
    {
        len.appendToQue(que);
        for (uint16_t i = 0; i < len.data; i++)
        {
            que->append_uint32(data[i]);
        }
    }
    virtual uint16_t getWireSize() { return len.getWireSize() + len.data * 4; }
};




#endif