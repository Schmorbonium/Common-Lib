
#ifndef __UART_DATA
#define __UART_DATA


#include "bufferedUart.hpp"
#include "charBuffer.hpp"

typedef uint32_t MemAddr_t;
typedef uint32_t MemVal_t;


// ------------------------ Primitive fields ----------------------------------
class PacketField
{
protected:
public:
    PacketField() {}
    virtual void parseFromQue(CharBuffer *que) {}
    virtual void appendToQue(CharBuffer *que) {}
};

class Uint8Field : public PacketField
{
public:
    uint8_t data;
    Uint8Field(uint8_t data) : PacketField(), data(data) {}
    Uint8Field(CharBuffer *que) { parseFromQue(que); }
    virtual void parseFromQue(CharBuffer *que) { data = que->pop(); }
    virtual void appendToQue(CharBuffer *que) { que->append(data); }
};

class Uint16Field : public PacketField
{
public:
    uint16_t data;
    Uint16Field(uint16_t data) : PacketField(), data(data) {}
    Uint16Field(CharBuffer *que) { parseFromQue(que); }
    virtual void parseFromQue(CharBuffer *que)
    {
        data = que->pop_uint16();
    }
    virtual void appendToQue(CharBuffer *que)
    {
        que->append_uint16(data);

    }
};

class Uint32Field : public PacketField
{
protected:
public:
    uint32_t data;
    Uint32Field(uint32_t data) : PacketField(), data(data) {}
    Uint32Field(CharBuffer *que) { parseFromQue(que); }
    virtual void parseFromQue(CharBuffer *que)
    {
        data = que->pop_uint32();
    }
    virtual void appendToQue(CharBuffer *que)
    {
        que->append_uint32(data);
    }
};

class UnusedField8 : public PacketField
{
protected:
public:
    UnusedField8() {}
    UnusedField8(CharBuffer *que) {}
    virtual void parseFromQue(CharBuffer *que)
    {
        que->pop();
    }
    virtual void appendToQue(CharBuffer *que)
    {
        que->append(0);
    }
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

    StringField(CharBuffer *que) : len(que)
    {
        for (uint16_t i = 0; i < len.data; i++)
        {
            string[i] = que->pop();
        }
    }

    ~StringField()
    {
        delete string;
    }

    virtual void parseFromQue(CharBuffer *que)
    {
        len = Uint16Field(que);
        for (uint16_t i = 0; i < len.data; i++)
        {
            string[i] = que->pop();
        }
    }

    virtual void appendToQue(CharBuffer *que)
    {
        len.appendToQue(que);
        for (uint16_t i = 0; i < len.data; i++)
        {
            que->append(string[i]);
        }
    }
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
    Uint8ArrayFeild(CharBuffer *que) : len(que)
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
    virtual void parseFromQue(CharBuffer *que)
    {
        delete data;
        len = Uint16Field(que);
        data = new uint8_t[len.data];

        for (uint16_t i = 0; i < len.data; i++)
        {
            data[i] = que->pop();
        }
    }
    virtual void appendToQue(CharBuffer *que)
    {
        len.appendToQue(que);
        for (uint16_t i = 0; i < len.data; i++)
        {
            que->append(data[i]);
        }
    }
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
    Uint16ArrayFeild(CharBuffer *que) : len(que)
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
    virtual void parseFromQue(CharBuffer *que)
    {
        delete[] data;
        len = Uint16Field(que);
        data = new uint16_t[len.data];

        for (uint16_t i = 0; i < len.data; i++)
        {
            data[i] = que->pop_uint16();
        }
    }
    virtual void appendToQue(CharBuffer *que)
    {
        len.appendToQue(que);
        for (uint16_t i = 0; i < len.data; i++)
        {
            que->append_uint16(data[i]);
        }
    }
};

class Uint32ArrayFeild : public PacketField
{
protected:
    Uint16Field len;
    uint16_t *data;
public:
    Uint32ArrayFeild(uint16_t *_data, uint16_t _len) : len(_len)
    {
        data = new uint16_t[len.data];
        for (uint16_t i = 0; i < len.data; i++)
        {
            this->data[i] = _data[i];
        }
    }
    Uint32ArrayFeild(CharBuffer *que) : len(que)
    {
        data = new uint16_t[len.data];
        for (uint16_t i = 0; i < len.data; i++)
        {
            data[i] = que->pop_uint32();
        }
    }
    ~Uint32ArrayFeild()
    {
        delete[] data;
    }
    virtual void parseFromQue(CharBuffer *que)
    {
        delete[] data;
        len = Uint16Field(que);
        data = new uint16_t[len.data];

        for (uint16_t i = 0; i < len.data; i++)
        {
            data[i] = que->pop_uint32();
        }
    }
    virtual void appendToQue(CharBuffer *que)
    {
        len.appendToQue(que);
        for (uint16_t i = 0; i < len.data; i++)
        {
            que->append_uint32(data[i]);
        }
    }
};

class FlagField : public Uint8Field
{
public:
    FlagField(uint8_t flags) : Uint8Field(flags) {}
    FlagField(CharBuffer *que) : Uint8Field(que) {}
    void setFlag(uint8_t index) { data |= (1 << index); }
    void resetFlag(uint8_t index) { data &= ~(1 << index); }
    bool getFlag(uint8_t index) { return data & (1 << index); };
};


#endif