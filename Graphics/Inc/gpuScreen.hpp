
#ifndef _GPU_SCREEN
#define _CPU_SCREEN

#include "gpuCore.hpp"
#include "lcd.hpp"

class gpuScreen
{
private:
    GPU_Channel channel;
public:
    Color backgroundColor;
    gpuScreen(UART_HandleTypeDef* uart);
    void Respond(uint16_t packAck);
    ~gpuScreen();
    void asyncHandler();
    void processInbox();
    void Clear();
};


#endif