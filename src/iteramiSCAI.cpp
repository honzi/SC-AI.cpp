#include "iteramiSCAI.h"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

void iteramiSCAI::onEnd(bool isWinner){
}

void iteramiSCAI::onFrame(){
    if(Broodwar->isReplay()
      || Broodwar->isPaused()
      || !Broodwar->self()
      || Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0){
        return;
    }
}

void iteramiSCAI::onNukeDetect(BWAPI::Position target){
}

void iteramiSCAI::onPlayerLeft(BWAPI::Player player){
}

void iteramiSCAI::onReceiveText(BWAPI::Player player, std::string text){
}

void iteramiSCAI::onSaveGame(std::string gameName){
}

void iteramiSCAI::onSendText(std::string text){
    Broodwar->sendText("%s", text.c_str());
}

void iteramiSCAI::onStart(){
    Broodwar->setCommandOptimizationLevel(2);
}

void iteramiSCAI::onUnitComplete(BWAPI::Unit unit){
}

void iteramiSCAI::onUnitCreate(BWAPI::Unit unit){
}

void iteramiSCAI::onUnitDestroy(BWAPI::Unit unit){
}

void iteramiSCAI::onUnitDiscover(BWAPI::Unit unit){
}

void iteramiSCAI::onUnitEvade(BWAPI::Unit unit){
}

void iteramiSCAI::onUnitHide(BWAPI::Unit unit){
}

void iteramiSCAI::onUnitMorph(BWAPI::Unit unit){
}

void iteramiSCAI::onUnitRenegade(BWAPI::Unit unit){
}

void iteramiSCAI::onUnitShow(BWAPI::Unit unit){
}

