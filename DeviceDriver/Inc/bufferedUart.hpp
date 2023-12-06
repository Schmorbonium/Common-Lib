#ifndef __BUFFERED_UART__
#define __BUFFERED_UART__

#include "zHal.h"
#include "iQueue.hpp"
#include "ringCharBuffer.hpp"
#include "iBufferedChannel.hpp"

class BufferedUart : public IBufferedChannel
{
protected:
    bool listening;
    UART_HandleTypeDef* uart;
    void startSending();
    void stopSending();
    bool pendingReadInterrupt();
    bool pendingWriteInterrupt();
public:
    BufferedUart(UART_HandleTypeDef* Core);
    ~BufferedUart(){}

    // Delete copy constructor and copy assignment operator
    BufferedUart(const BufferedUart&) = delete;
    BufferedUart& operator=(const BufferedUart&) = delete;

    // Delete move constructor and move assignment operator
    BufferedUart(BufferedUart&&) = delete;
    BufferedUart& operator=(BufferedUart&&) = delete;

    virtual void asyncHandler();
    void startListening();
    void stopListening();
};


#endif // End __BUFFERED_UART__