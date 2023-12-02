#ifndef __BUFFERED_UART__
#define __BUFFERED_UART__


#include "zHal.h"
#include "charBuffer.hpp"
#include "ibc.h"


class BufferedUart
{
protected:
    bool listening;
    bool sending;
    CharBuffer TxQue;
    CharBuffer RxQue;
    UART_HandleTypeDef* uart;
    void startSending();
    void stopSending();
    bool pendingReadInterrupt();
    bool pendingWriteInterrupt();
    IbcResetCallback asyncResetCallback;
    uint16_t resetCountLimit;
    uint16_t zeroCount;
public:
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
    void takeFromInbox(uint8_t* buf, uint16_t size);
    uint16_t getInputSize();
    uint16_t getOutputSize();
    void setResetCallback(IbcResetCallback resetCallback);
    void setResetCount(uint16_t count);
};


#endif // End __BUFFERED_UART__