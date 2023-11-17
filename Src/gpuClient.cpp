#include "gpuClient.hpp"

#define USE_GPU_CLIENT

extern GPU_Client* gpu_client;



#ifdef USE_GPU_CLIENT
// All of the Thing that must be Implemented on the client and server Side
bool FrameIdPkt::actOnPkt() {}
bool LutIdPkt::actOnPkt() {}
bool SpriteIdPkt::actOnPkt() {}
bool NewFramePkt::actOnPkt() {}
bool NewLutPkt::actOnPkt() {}
bool NewSpritePkt::actOnPkt() {}
bool LinkFramePkt::actOnPkt() {}
bool LinkMultiFramePkt::actOnPkt() {}
bool PlaceSpritePkt::actOnPkt() {}
bool AnimateSpritePkt::actOnPkt() {}
bool MoveSpritePkt::actOnPkt() {}
bool LoadFramePkt::actOnPkt() {}
bool LoadLutPkt::actOnPkt() {}
bool GpuResetPkt::actOnPkt()
{
    switch (this->ResetType.data)
    {
    case RST_init:
        gpu_client->init();
        gpu_client->SendPacket(this);
        /* code */
        break;
    case RST_full:
        gpu_client->fullReset();
        /* code */
        break;
    case RST_ClearScreen:
        gpu_client->clearScreen();
        /* code */
        break;
    default:
        break;
    }
}
#endif
