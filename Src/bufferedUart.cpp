#include "bufferedUart.hpp"

BufferedUart::BufferedUart(UART_HandleTypeDef *Core) : listening(false), TxQue(CharBuffer()), RxQue(CharBuffer()), uart(Core) {}

void BufferedUart::send(uint8_t *buf, uint16_t length)
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


uint16_t BufferedUart::getInputSize()
{
    return RxQue.getSize();
}
uint16_t BufferedUart::getOutputSize()
{
    return TxQue.getSize();
}

void BufferedUart::takeFromInbox(uint8_t *buf, uint16_t count)
{
    for (uint16_t i = 0; i < count; i++)
    {
        buf[i] = RxQue.pop();
    }
    
}

#ifdef __BLUE_PILL
const uint16_t TX_Ready = (1 << 7);
const uint16_t TX_Complete = (1 << 6);
const uint16_t RX_Ready = (1 << 5);
const uint16_t TXEIE_Flag = (1 << 7);
const uint16_t RXNEIE_flag = (1 << 5);
void BufferedUart::startSending()
{
    uart->Instance->CR1 |= TXEIE_Flag;
    sending = true;
}
void BufferedUart::stopSending()
{
    uart->Instance->CR1 &= (~TXEIE_Flag);
    sending = false;
}
void BufferedUart::startListening()
{
    uart->Instance->CR1 |= RXNEIE_flag;
    listening = true;
}
void BufferedUart::stopListening()
{
    uart->Instance->CR1 &= (~RXNEIE_flag);
    listening = false;
}
bool BufferedUart::pendingReadInterrupt(){
    return (uart->Instance->SR & TX_Ready);
}
bool BufferedUart::pendingWriteInterrupt(){
    return (uart->Instance->SR & RX_Ready);

}
void BufferedUart::uartHandler()
{
    if (pendingWriteInterrupt())
    {
        if (TxQue.getSize() > 0)
        {
            uart->Instance->DR = TxQue.pop();
        }
        if (sending && TxQue.getSize() == 0)
        {
            stopSending();
        }
    }

    while (pendingReadInterrupt())
    {
        uint8_t data = (uint8_t)uart->Instance->DR;
        RxQue.append(data);
    }
}

#endif

#ifdef __DISCO
const uint16_t TX_Ready = (1 << 7);
const uint16_t TX_Complete = (1 << 6);
const uint16_t RX_Ready = (1 << 5);
const uint16_t TXEIE_Flag = (1 << 7);
const uint16_t RXNEIE_flag = (1 << 5);
void BufferedUart::startSending()
{
    uart->Instance->CR1 |= TXEIE_Flag;
    sending = true;
}
void BufferedUart::stopSending()
{
    uart->Instance->CR1 &= (~TXEIE_Flag);
    sending = false;
}
void BufferedUart::startListening()
{
    uart->Instance->CR1 |= RXNEIE_flag;
    listening = true;
}
void BufferedUart::stopListening()
{
    uart->Instance->CR1 &= (~RXNEIE_flag);
    listening = false;
}
bool BufferedUart::pendingReadInterrupt(){
    return (uart->Instance->ISR & TX_Ready);
}
bool BufferedUart::pendingWriteInterrupt(){
    return (uart->Instance->ISR & RX_Ready);

}
void BufferedUart::uartHandler()
{
    if (pendingWriteInterrupt())
    {
        if (TxQue.getSize() > 0)
        {
            uart->Instance->TDR = TxQue.pop();
        }
        if (sending && TxQue.getSize() == 0)
        {
            stopSending();
        }
    }

    while (pendingReadInterrupt())
    {
        uint8_t data = (uint8_t)uart->Instance->RDR;
        RxQue.append(data);
    }
}

#endif