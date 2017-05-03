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
        if(!unit->exists()
          || !unit->isCompleted()
          || unit->isConstructing()
          || unit->isLoaded()
          || unit->isLockedDown()
          || unit->isMaelstrommed()
          || !unit->isPowered()
          || unit->isStasised()
          || unit->isStuck()){
            continue;
        }

        // Handle workers.
        if(unit->getType().isWorker()){
            if(unit->isIdle()){
                // Return resources.
                if(unit->isCarryingMinerals()
                  || unit->isCarryingGas()){
                    unit->returnCargo();

                // Gather resources.
                }else{
                    unit->gather(unit->getClosestUnit(IsMineralField || IsRefinery));
                }
            }

        // Handle Command Centers, Hatcheries, and Nexuses.
        }else if(unit->getType().isResourceDepot()){
            if(unit->isIdle()){
                // Build workers.
                unit->train(unit->getType().getRace().getWorker());
            }

        // Everything else should scout.
        }else if(unit->isIdle()){
            Position position = unit->getPosition();
            position.x += rand() % 501 - 250;
            position.y += rand() % 501 - 250;
            unit->move(position);
        }

        // Check for errors.
        Error lastError = Broodwar->getLastError();

        // Handle insufficient supply error by
        //   building Pylon, building Supply Depot, or training Overlord.
        if(lastError == Errors::Insufficient_Supply){
            UnitType supplyProviderType = unit->getType().getRace().getSupplyProvider();

            if(Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0){
                Unit supplyBuilder = unit->getClosestUnit(GetType == supplyProviderType.whatBuilds().first
                  && (IsIdle || IsGatheringMinerals)
                  && IsOwned);

                if(supplyProviderType.isBuilding()){
                    /*TilePosition targetBuildLocation = Broodwar->getBuildLocation(
                      supplyProviderType,
                      supplyBuilder->getTilePosition()
                    );

                    if(targetBuildLocation){
                        supplyBuilder->build(
                          supplyProviderType,
                          targetBuildLocation
                        );
                    }*/

                }else{
                    supplyBuilder->train(supplyProviderType);
                }
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
    Broodwar->setCommandOptimizationLevel(2);

    srand(time(NULL));

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

