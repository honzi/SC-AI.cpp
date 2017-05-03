#include "ai.h"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

void ai::onEnd(bool isWinner){
}

void ai::onFrame(){
    if(Broodwar->isReplay()
      || Broodwar->isPaused()
      || !Broodwar->self()
      || Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0){
        return;
    }
}

void ai::onNukeDetect(BWAPI::Position target){
}

void ai::onPlayerLeft(BWAPI::Player player){
}

void ai::onReceiveText(BWAPI::Player player, std::string text){
}

void ai::onSaveGame(std::string gameName){
}

void ai::onSendText(std::string text){
    Broodwar->sendText("%s", text.c_str());
}

void ai::onStart(){
    Broodwar->setCommandOptimizationLevel(2);

    Broodwar->sendText('iterami/SC-AI.cpp: blank AI loaded');
}

void ai::onUnitComplete(BWAPI::Unit unit){
}

void ai::onUnitCreate(BWAPI::Unit unit){
}

void ai::onUnitDestroy(BWAPI::Unit unit){
}

void ai::onUnitDiscover(BWAPI::Unit unit){
}

void ai::onUnitEvade(BWAPI::Unit unit){
}

void ai::onUnitHide(BWAPI::Unit unit){
}

void ai::onUnitMorph(BWAPI::Unit unit){
}

void ai::onUnitRenegade(BWAPI::Unit unit){
}

void ai::onUnitShow(BWAPI::Unit unit){
}

