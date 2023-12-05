#ifndef __BUFFERED_UART__
#define __BUFFERED_UART__

#include "zHal.h"
#include "iQueue.hpp"
#include "ringCharBuffer.hpp"



class BufferedUart
{
protected:
    bool listening;
    UART_HandleTypeDef* uart;
    bool sending;
    RingCharBuffer TxQue;
    void startSending();
    void stopSending();
    bool pendingReadInterrupt();
    bool pendingWriteInterrupt();
public:
    RingCharBuffer RxQue;
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
};


#endif // End __BUFFERED_UART__