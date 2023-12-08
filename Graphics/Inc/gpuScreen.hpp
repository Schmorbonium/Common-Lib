
#ifndef _GPU_CLIENT
#define _CPU_CLIENT

#include "gpuCore.hpp"
#include "lcd.hpp"

class gpuScreen
{
private:
    GPU_Channel channel;
public:
    gpuScreen(UART_HandleTypeDef* uart);
    void Respond(uint16_t packAck);
    ~gpuScreen();
};


#endif