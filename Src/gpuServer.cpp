#include "gpuCore.hpp"
#include "gpuServer.hpp"


extern GPU_Server gpuServer;


#ifdef USE_GPU_SERVER
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
bool GpuResetPkt::actOnPkt() {
    gpuServer.initialized = true;
}
#endif

