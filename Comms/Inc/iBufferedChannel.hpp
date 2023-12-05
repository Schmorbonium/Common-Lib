#ifndef __BUFFERED_CHANNEL__
#define __BUFFERED_CHANNEL__

#include "zHal.h"
#include "iQueue.hpp"
#include "ringCharBuffer.hpp"



class IBufferedChannel
{
protected:
    virtual void startSending();
    virtual void stopSending();
public:
    RingCharBuffer TxQue;
    RingCharBuffer RxQue;
    IBufferedChannel();
    ~IBufferedChannel();

    // Delete copy constructor and copy assignment operator
    IBufferedChannel(const IBufferedChannel&) = delete;
    IBufferedChannel& operator=(const IBufferedChannel&) = delete;

    // Delete move constructor and move assignment operator
    IBufferedChannel(IBufferedChannel&&) = delete;
    IBufferedChannel& operator=(IBufferedChannel&&) = delete;

    virtual void startListening();
    virtual void stopListening();
    virtual void asyncHandler();

    void send(uint8_t* buf, uint16_t length);
    void send(ISendable* buf);
    void takeFromInbox(uint8_t* buf, uint16_t size);
    uint16_t getInputSize();
    uint16_t getOutputSize();
};


#endif // End __BUFFERED_UART__