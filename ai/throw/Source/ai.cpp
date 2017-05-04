#include "ai.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

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

    for(auto &unit : Broodwar->self()->getUnits()){
        if(unit->exists()
          && unit->isCompleted()
          && !unit->isConstructing()
          && !unit->isLoaded()
          && !unit->isLockedDown()
          && !unit->isMaelstrommed()
          && unit->isPowered()
          && !unit->isStasised()
          && !unit->isStuck()
          && unit->isIdle()){
            // Build a worker to speed up the throwing process.
            if(unit->getType().isResourceDepot()){
                unit->train(unit->getType().getRace().getWorker());

            // Make all units attack starting resource depot.
            }else{
                unit->attack(Broodwar->getClosestUnit(
                  unit->getPosition(),
                  IsResourceDepot
                ));
            }
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
    Broodwar->setCommandOptimizationLevel(1);

    srand(time(NULL));

    Broodwar->sendText("iterami/SC-AI.cpp throw");
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

