#include "ai.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace BWAPI;
using namespace Filter;

// Global variables.
bool infantryBuildingNeeded;
bool supplyNeeded;
int savingMinerals;
Race playerRace;
static int supplyChecked;
static int infantryBuildingChecked;
UnitType infantryBuilding;

void ai::onEnd(bool isWinner){
}

void ai::onFrame(){
    int frameCount = Broodwar->getFrameCount();

    if(!Broodwar->self()
      || frameCount % Broodwar->getLatencyFrames() != 0
      || Broodwar->isPaused()
      || Broodwar->isReplay()){
        return;
    }

    int minerals = Broodwar->self()->minerals();
    int supplyTotal = Broodwar->self()->supplyTotal();

    if(supplyTotal < 200
      && supplyTotal - Broodwar->self()->supplyUsed() <= 2){
        savingMinerals = 100;
        supplyNeeded = true;

    }else{
        savingMinerals = 0;
        supplyNeeded = false;
    }

    if(minerals > 200){
        infantryBuildingNeeded = true;
        savingMinerals = 200;
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

        // Setup unit information variables.
        UnitType unitType = unit->getType();

        // Handle workers.
        if(unitType.isWorker()){
            // Handle insufficient supply by building Pylon, building Supply Depot, or training Overlord.
            if(supplyNeeded
              && minerals >= savingMinerals
              && supplyChecked + 500 < frameCount){
                supplyChecked = frameCount;
                UnitType supplyProviderType = unitType.getRace().getSupplyProvider();

                if(Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0){
                    Unit supplyBuilder = unit->getClosestUnit(GetType == supplyProviderType.whatBuilds().first
                      && (IsIdle || IsGatheringMinerals)
                      && IsOwned);

                    if(supplyProviderType.isBuilding()){
                        buildBuilding(
                          supplyBuilder,
                          supplyProviderType
                        );

                    }else{
                        supplyBuilder->train(supplyProviderType);
                    }
                }

            // Build Barracks/Gateway/Spawning Pool.
            }else if(infantryBuildingNeeded
              && minerals >= savingMinerals
              && infantryBuildingChecked + 1000 < frameCount){
                infantryBuildingChecked = frameCount;

                if(buildBuilding(
                  unit,
                  infantryBuilding
                )){
                    infantryBuildingNeeded = false;
                    savingMinerals = 0;
                }

            }else if(unit->isIdle()){
                // Return resources.
                if(unit->isCarryingMinerals()
                  || unit->isCarryingGas()){
                    unit->returnCargo();

                // Gather resources.
                }else{
                    unit->gather(unit->getClosestUnit(IsMineralField || IsRefinery));
                }
            }

        }else if(unit->isIdle()){
            // Handle Command Centers, Hatcheries, and Nexuses.
            if(unitType.isResourceDepot()){
                if(minerals >= savingMinerals + 50){
                    // Train workers.
                    unit->train(unitType.getRace().getWorker());
                }

            // Everything else should scout.
            }else{
                Position position = unit->getPosition();
                position.x += rand() % 501 - 250;
                position.y += rand() % 501 - 250;
                unit->move(position);
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

    // Setup global variables.
    infantryBuildingChecked = 0;
    infantryBuildingNeeded = false;
    playerRace = Broodwar->self()->getRace();
    savingMinerals = 0;
    supplyChecked = 0;
    supplyNeeded = false;

    // Handle race-specific stuff.
    if(playerRace == Races::Zerg){
        infantryBuilding = UnitTypes::Zerg_Spawning_Pool;

    }else if(playerRace == Races::Terran){
        infantryBuilding = UnitTypes::Terran_Barracks;

    }else{
        infantryBuilding = UnitTypes::Protoss_Gateway;
    }

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

bool buildBuilding(Unit builder, UnitType building){
    TilePosition targetBuildLocation = Broodwar->getBuildLocation(
      builing,
      builder->getTilePosition()
    );

    if(targetBuildLocation){
        return builder->build(
          building,
          targetBuildLocation
        );
    }

    return false;
}

