#include "gpuScreen.hpp"
#include "bufferedUart.hpp"

gpuScreen *screen;
extern LCD_Controller display;

gpuScreen::gpuScreen(UART_HandleTypeDef *uart)
    : channel(new BufferedUart(uart))
{
    this->backgroundColor.R = 0x0;
    this->backgroundColor.G = 0x0;
    this->backgroundColor.B = 0xFF;
}

void gpuScreen::Respond(uint16_t packAck)
{
    GPUPacket *pkt = new ResponseGPUPkt(packAck);
    channel.SendPacket(pkt);
    delete pkt;
}
gpuScreen::~gpuScreen()
{
}

void gpuScreen::asyncHandler(){
    this->channel.channel->asyncHandler();
}

void gpuScreen::processInbox(){
    while(this->channel.PacketReady()){
        this->channel.processNextPacket();
    }
}

void gpuScreen::Clear(){
    display.clearFrame(this->backgroundColor);
}

bool ResetGPUPkt::actOnPkt()
{
    // Color backgroundColor;
    // backgroundColor.R = 0xFF;
    // backgroundColor.G = 0x0;
    // backgroundColor.B = 0x0;
    // this->clearFrame(backgroundColor);

    display.initLcd();
    screen->Clear();
    screen->Respond(this->pktCnt.data);
    return false;
}

bool SetBackground::actOnPkt()
{
    screen->backgroundColor = this->setColor.getColor();
    screen->Respond(this->pktCnt.data);
    return false;
}

bool DrawRectPkt::actOnPkt()
{
    uint16_t x = this->x.data;
    uint16_t y = this->y.data;
    uint16_t w = this->w.data;
    uint16_t h = this->h.data;
    display.drawRec(x, y, w, h, this->setColor.getColor());
    // display.backgroundColor = this->setColor.getColor();
    screen->Respond(this->pktCnt.data);
    return false;
}

bool DrawStrPkt::actOnPkt()
{
    // TODO Make strings work?
    screen->Respond(this->pktCnt.data);
    return false;
}

bool ResponseGPUPkt::actOnPkt()
{
    return false;
}

// To Implement in the Client and server respetivly