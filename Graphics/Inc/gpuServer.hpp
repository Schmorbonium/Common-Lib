#ifndef _GPU_SERVER
#define _GPU_SERVER

#include "gpuCore.hpp"
class GPU_Server : public GPU_Channel
{
    public:
    bool initialized = false;
    GPU_Server(UART_HandleTypeDef *Core)
        : GPU_Channel(Core)
    {
    }

    void waitOnInit()
    {
        while (!initialized)
        {
            if (channel->getInputSize() >= 4)
            {
                GpuCommand cmd = (GpuCommand)this->peekCommand();
                if (cmd != Cmd_ResetGpu)
                {
                    channel->RxQue.pop();
                    continue;
                }

                uint16_t cmdLen = channel->RxQue.peak_uint16(2);
                if (channel->RxQue.getSize() >= cmdLen)
                {
                    GpuResetPkt resetCommand(&channel->RxQue);
                    resetCommand.actOnPkt();
                }
            }
        }
    }

    void init()
    {
        channel->startListening();
        GpuResetPkt rstPkt(RST_init);
        this->SendPacket(&rstPkt);
        waitOnInit();
    }

    void fullReset()
    {
    }

    void clearScreen()
    {
    }

    void setBackgroundColor(Color color){
        FillBackGroundPkt commandPkt(color);
        this->SendPacket(&commandPkt);
    }
};

#endif