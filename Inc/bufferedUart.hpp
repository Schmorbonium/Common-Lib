#ifndef __BUFFERED_UART__
#define __BUFFERED_UART__

#include "zHal.h"
#include "charBuffer.hpp"
#include "ibc.h"


class ISendable
{
public:
    virtual uint8_t appendToQue(CharBuffer *que){return 0;}
    virtual uint16_t getWireSize() {return 0;}
};


class BufferedUart
{
protected:
    bool listening;
    UART_HandleTypeDef* uart;
    bool sending;
    CharBuffer TxQue;
    void startSending();
    void stopSending();
    bool pendingReadInterrupt();
    bool pendingWriteInterrupt();
    IbcResetCallback asyncResetCallback;
    uint16_t resetCountLimit;
    uint16_t zeroCount;
public:
    CharBuffer RxQue;
    BufferedUart(UART_HandleTypeDef* Core);
    ~BufferedUart(){}

    // Delete copy constructor and copy assignment operator
    BufferedUart(const BufferedUart&) = delete;
    BufferedUart& operator=(const BufferedUart&) = delete;

    // Delete move constructor and move assignment operator
    BufferedUart(BufferedUart&&) = delete;
    BufferedUart& operator=(BufferedUart&&) = delete;

    virtual void uartHandler();
    void startListening();
    void stopListening();
    void send(uint8_t* buf, uint16_t length);
    void send(ISendable* buf);
    void takeFromInbox(uint8_t* buf, uint16_t size);
    uint16_t getInputSize();
    uint16_t getOutputSize();
    void setResetCallback(IbcResetCallback resetCallback);
    void setResetCount(uint16_t count);
};


#endif // End __BUFFERED_UART__