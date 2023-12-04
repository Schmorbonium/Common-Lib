
#ifndef __UART_DATA
#define __UART_DATA

#include "bufferedUart.hpp"
#include "iQueue.hpp"

typedef uint32_t MemAddr_t;
typedef uint32_t MemVal_t;

// ------------------------ Primitive fields ----------------------------------
class PacketField
{
protected:
public:
    PacketField() {}
    virtual void parseFromQue(IQueue *que) {}
    virtual void appendToQue(IQueue *que) {}
    virtual uint16_t getWireSize() { return 0; }
};

class Uint8Field : public PacketField
{
public:
    uint8_t data;
    Uint8Field(uint8_t data) : data(data) {}
    Uint8Field(IQueue *que) { parseFromQue(que); }
    virtual void parseFromQue(IQueue *que) { data = que->pop(); }
    virtual void appendToQue(IQueue *que) { que->append(data); }
    virtual uint16_t getWireSize() { return 1; }
};

class Uint16Field : public PacketField
{
public:
    uint16_t data;
    Uint16Field(uint16_t data) : data(data) {}
    Uint16Field(IQueue *que) { parseFromQue(que); }
    virtual void parseFromQue(IQueue *que)
    {
        data = que->pop_uint16();
    }
    virtual void appendToQue(IQueue *que)
    {
        que->append_uint16(data);
    }
    virtual uint16_t getWireSize() { return 2; }
};

class Uint32Field : public PacketField
{
protected:
public:
    uint32_t data;
    Uint32Field(uint32_t data) : data(data) {}
    Uint32Field(IQueue *que) { parseFromQue(que); }
    virtual void parseFromQue(IQueue *que)
    {
        data = que->pop_uint32();
    }
    virtual void appendToQue(IQueue *que)
    {
        que->append_uint32(data);
    }
    virtual uint16_t getWireSize() { return 4; }
};

class UnusedField8 : public PacketField
{
protected:
public:
    UnusedField8() {}
    UnusedField8(IQueue *que) { que->pop(); }
    virtual void parseFromQue(IQueue *que)
    {
        que->pop();
    }
    virtual void appendToQue(IQueue *que)
    {
        que->append(0);
    }
    virtual uint16_t getWireSize() { return 1; }
};

// ------------------------ Complex fields ----------------------------------
class StringField : public PacketField
{
protected:
    Uint16Field len;
    char *string;

public:
    StringField(char *string, uint16_t strLeng) : len(strLeng)
    {
        string = new char[len.data];
        for (uint16_t i = 0; i < len.data; i++)
        {
            this->string[i] = string[i];
        }
    }

    StringField(IQueue *que) : len(que)
    {
        string = new char[len.data];
        for (uint16_t i = 0; i < len.data; i++)
        {
            string[i] = que->pop();
        }
    }

    ~StringField()
    {
        delete string;
    }

    virtual void parseFromQue(IQueue *que)
    {
        len = Uint16Field(que);
        string = new char[len.data];
        for (uint16_t i = 0; i < len.data; i++)
        {
            string[i] = que->pop();
        }
    }

    virtual void appendToQue(IQueue *que)
    {
        len.appendToQue(que);
        for (uint16_t i = 0; i < len.data; i++)
        {
            que->append(string[i]);
        }
    }
    virtual uint16_t getWireSize() { return len.getWireSize() + len.data * 2; }
};

class Uint8ArrayFeild : public PacketField
{
protected:
    Uint16Field len;
    uint8_t *data;

public:
    Uint8ArrayFeild(uint8_t *_data, uint16_t _len) : len(_len)
    {
        data = new uint8_t[len.data];
        for (uint16_t i = 0; i < len.data; i++)
        {
            this->data[i] = _data[i];
        }
    }
    Uint8ArrayFeild(IQueue *que) : len(que)
    {
        data = new uint8_t[len.data];
        for (uint16_t i = 0; i < len.data; i++)
        {
            data[i] = que->pop();
        }
    }
    ~Uint8ArrayFeild()
    {
        delete data;
    }
    virtual void parseFromQue(IQueue *que)
    {
        delete data;
        len = Uint16Field(que);
        data = new uint8_t[len.data];

        for (uint16_t i = 0; i < len.data; i++)
        {
            data[i] = que->pop();
        }
    }
    virtual void appendToQue(IQueue *que)
    {
        len.appendToQue(que);
        for (uint16_t i = 0; i < len.data; i++)
        {
            que->append(data[i]);
        }
    }
    virtual uint16_t getWireSize() { return len.getWireSize() + len.data * 1; }
};

class Uint16ArrayFeild : public PacketField
{
protected:
    Uint16Field len;
    uint16_t *data;

public:
    Uint16ArrayFeild(uint16_t *_data, uint16_t _len) : len(_len)
    {
        data = new uint16_t[len.data];
        for (uint16_t i = 0; i < len.data; i++)
        {
            this->data[i] = _data[i];
        }
    }
    Uint16ArrayFeild(IQueue *que) : len(que)
    {
        data = new uint16_t[len.data];
        for (uint16_t i = 0; i < len.data; i++)
        {
            data[i] = que->pop_uint16();
        }
    }
    ~Uint16ArrayFeild()
    {
        delete[] data;
    }
    virtual void parseFromQue(IQueue *que)
    {
        delete[] data;
        len = Uint16Field(que);
        data = new uint16_t[len.data];

        for (uint16_t i = 0; i < len.data; i++)
        {
            data[i] = que->pop_uint16();
        }
    }
    virtual void appendToQue(IQueue *que)
    {
        len.appendToQue(que);
        for (uint16_t i = 0; i < len.data; i++)
        {
            que->append_uint16(data[i]);
        }
    }
    virtual uint16_t getWireSize() { return len.getWireSize() + len.data * 2; }
};

class Uint32ArrayFeild : public PacketField
{
protected:
    Uint16Field len;
    uint32_t *data;

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

class FlagField : public Uint8Field
{
public:
    FlagField(uint8_t flags) : Uint8Field(flags) {}
    FlagField(IQueue *que) : Uint8Field(que) {}
    void setFlag(uint8_t index) { data |= (1 << index); }
    void setFlag(uint8_t index, bool value)
    {
        if (value)
        {
            data |= (1 << index);
        }
        else
        {
            data &= ~(1 << index);
        }
    }
    void resetFlag(uint8_t index) { data &= ~(1 << index); }
    bool getFlag(uint8_t index) { return data & (1 << index); };
};

#endif