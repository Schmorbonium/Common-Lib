#include "bufferedUart.hpp"

BufferedUart::BufferedUart(UART_HandleTypeDef *Core) : listening(false),
                                                       uart(Core),
                                                       sending(false)
{
}

void BufferedUart::startSending()
{
    _ZHAL_UART_SET_TXI(uart->Instance);
    sending = true;
}
void BufferedUart::stopSending()
{
    _ZHAL_UART_RESET_TXI(uart->Instance);
    sending = false;
}
void BufferedUart::startListening()
{
    _ZHAL_UART_SET_RXI(uart->Instance);
    listening = true;
}
void BufferedUart::stopListening()
{
    _ZHAL_UART_RESET_RXI(uart->Instance);
    listening = false;
}
void BufferedUart::asyncHandler()
{
    while (_ZHAL_UART_RX_READY(uart->Instance))
    {
        uint8_t data = _ZHAL_UART_RX_BYTE(uart->Instance);
        RxQue.append(data);
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