#include "gpuCore.hpp"
#include "gpuServer.hpp"


extern GPU_Server gpuServer;


#ifdef USE_GPU_SERVER
// All of the Thing that must be Implemented on the client and server Side
bool FrameIdPkt::actOnPkt() {return false;}
bool LutIdPkt::actOnPkt() {return false;}
bool SpriteIdPkt::actOnPkt() {return false;}
bool NewFramePkt::actOnPkt() {return false;}
bool NewLutPkt::actOnPkt() {return false;}
bool NewSpritePkt::actOnPkt() {return false;}
bool LinkFramePkt::actOnPkt() {return false;}
bool LinkMultiFramePkt::actOnPkt() {return false;}
bool PlaceSpritePkt::actOnPkt() {return false;}
bool AnimateSpritePkt::actOnPkt() {return false;}
bool MoveSpritePkt::actOnPkt() {return false;}
bool LoadFramePkt::actOnPkt() {return false;}
bool LoadLutPkt::actOnPkt() {return false;}
bool FillBackGroundPkt::actOnPkt() {return false;}
bool GpuResetPkt::actOnPkt() {
    gpuServer.initialized = true;
    return true;
}

// Implement in client and server
// Implement in client and server
// Implement in client and server
bool NewShapePkt::actOnPkt(){return false;}
bool SetShapePkt::actOnPkt(){return false;}
bool MoveShapePkt::actOnPkt(){return false;}

#endif

