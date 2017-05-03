#include "ai.h"
#include <iostream>

using namespace BWAPI;
using namespace Filter;

void ai::onEnd(bool isWinner){
}

void ai::onFrame(){
    if(!Broodwar->self()
      || Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0
      || Broodwar->isPaused()
      || Broodwar->isReplay()){
        return;
    }

    for(auto &u : Broodwar->self()->getUnits()){
        if(!u->exists()
          || !u->isCompleted()
          || u->isConstructing()
          || u->isLoaded()
          || u->isLockedDown()
          || u->isMaelstrommed()
          || !u->isPowered()
          || u->isStasised()
          || u->isStuck()){
            continue;
        }

        // Handle workers.
        if(u->getType().isWorker()){
            if(!u->isIdle()){
                continue;
            }

            if(u->isCarryingMinerals()
              || u->isCarryingGas()){
                u->returnCargo();

            }else{
                u->gather(u->getClosestUnit(IsMineralField || IsRefinery));
            }

        // Handle Command Centers, Hatcheries, and Nexuses.
        }else if(u->getType().isResourceDepot()){
            if(!u->isIdle()){
                continue
            }

            // Build workers.
            u->train(u->getType().getRace().getWorker());
        }
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

    Broodwar->sendText("iterami/SC-AI.cpp vs");
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

