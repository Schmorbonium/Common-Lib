#ifndef __BUFFERED_UART__
#define __BUFFERED_UART__


#include "zHal.h"
#include "charBuffer.hpp"

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
public:
    BufferedUart(UART_HandleTypeDef* Core);
    ~BufferedUart(){}
    virtual void uartHandler();
    void startListening();
    void stopListening();
    void send(uint8_t* buf, uint16_t length);
    void takeFromInbox(uint8_t* buf, uint16_t size);
    uint16_t getInputSize();
    uint16_t getOutputSize();
};


#endif // End __BUFFERED_UART__