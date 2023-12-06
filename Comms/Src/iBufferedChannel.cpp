#include "iBufferedChannel.hpp"

void IBufferedChannel::startSending() {}
void IBufferedChannel::stopSending() {}
IBufferedChannel::IBufferedChannel() {
    sending = false;
}
IBufferedChannel::~IBufferedChannel() {}
void IBufferedChannel::startListening() {}
void IBufferedChannel::stopListening() {}
void IBufferedChannel::asyncHandler()
{
    while (this->TxQue.getSize() > 0)
    {
        RxQue.append(TxQue.pop());
    }
}

void IBufferedChannel::send(uint8_t *buf, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        TxQue.append(buf[i]);
    }
    if (sending == false)
    {
        startSending();
    }
}

void IBufferedChannel::send(ISendable *buf)
{
    buf->appendToQue(&this->TxQue);
    if (sending == false)
    {
        startSending();
    }
}

void IBufferedChannel::takeFromInbox(uint8_t *buf, uint16_t size)
{
    for (uint16_t i = 0; i < size; i++)
    {
        buf[i] = RxQue.pop();
    }
}

uint16_t IBufferedChannel::getInputSize()
{
    return this->RxQue.getSize();
}

uint16_t IBufferedChannel::getOutputSize()
{
    return this->RxQue.getSize();
}
