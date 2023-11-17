#include "bufferedUart.hpp"

void dummyResetCallback() {
    //purposely empty
}

BufferedUart::BufferedUart(UART_HandleTypeDef* Core) :
    listening(false),
    TxQue(CharBuffer()),
    RxQue(CharBuffer()),
    uart(Core),
    asyncResetCallback(dummyResetCallback),
    resetCountLimit(0xFFFF) {
}

void BufferedUart::send(uint8_t* buf, uint16_t length) {
    for (uint16_t i = 0; i < length; i++)
    {
        TxQue.append(buf[i]);
    }
    if (sending == false)
    {
        startSending();
    }
}

void BufferedUart::send(ISendable* sendableObj)
{
    sendableObj->appendToQue(&this->TxQue);
    if (sending == false)
    {
        startSending();
    }
}


uint16_t BufferedUart::getInputSize() {
    return RxQue.getSize();
}

uint16_t BufferedUart::getOutputSize() {
    return TxQue.getSize();
}

void BufferedUart::takeFromInbox(uint8_t* buf, uint16_t count) {
    for (uint16_t i = 0; i < count; i++)
    {
        buf[i] = RxQue.pop();
    }
    
}

void BufferedUart::startSending() {
    _ZHAL_UART_SET_TXI(uart->Instance);
    sending = true;
}
void BufferedUart::stopSending() {
    _ZHAL_UART_RESET_TXI(uart->Instance);
    sending = false;
}
void BufferedUart::startListening() {
    _ZHAL_UART_SET_RXI(uart->Instance);
    listening = true;
}
void BufferedUart::stopListening() {
    _ZHAL_UART_RESET_RXI(uart->Instance);
    listening = false;
}
void BufferedUart::uartHandler() {
    while (_ZHAL_UART_RX_READY(uart->Instance))
    {
        uint8_t data = _ZHAL_UART_RX_BYTE(uart->Instance);
        RxQue.append(data);
        if (data == 0) {
            zeroCount++;
            if (zeroCount >= resetCountLimit) {
                this->asyncResetCallback();
            }
        } else {
            zeroCount = 0;
        }
    }

    if (_ZHAL_UART_TX_READY(uart->Instance))
    {
        if (TxQue.getSize() > 0)
        {
            _ZHAL_UART_TX_BYTE(uart->Instance, TxQue.pop());
        }
        if (sending && TxQue.getSize() == 0)
        {
            stopSending();
        }
    }
}


void BufferedUart::setResetCallback(IbcResetCallback resetCallback) {
    this->asyncResetCallback = resetCallback;
}

void BufferedUart::setResetCount(uint16_t count) {
    this->resetCountLimit = count;
}