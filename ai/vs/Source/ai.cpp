#include "ai.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace BWAPI;
using namespace Filter;

// Global variables.
bool infantryBuildingNeeded;
bool supplyNeeded;
bool supplyProviderTypeIsBuilding;
int infantryBuildingCheckTimer;
int savingMinerals;
int supplyCheckTimer;
Race playerRace;
static int infantryBuildingChecked;
static int supplyChecked;
UnitType infantryBuilding;
UnitType infantryType;
UnitType supplyProviderType;

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
      && supplyTotal - Broodwar->self()->supplyUsed() <= 2
      && Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0){
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
        bool unitIsIdle = unit->isIdle();
        UnitType unitType = unit->getType();

        // Handle workers.
        if(unitType.isWorker()){
            // Handle insufficient supply by building Pylon, building Supply Depot, or training Overlord.
            if(supplyNeeded
              && minerals >= savingMinerals
              && supplyChecked + supplyCheckTimer < frameCount){
                supplyChecked = frameCount;

                Unit supplyBuilder = unit->getClosestUnit(GetType == supplyProviderType.whatBuilds().first
                  && (IsIdle || IsGatheringMinerals)
                  && IsOwned);

                if(supplyProviderTypeIsBuilding){
                    buildBuilding(
                      supplyBuilder,
                      supplyProviderType
                    );

                }else{
                    supplyBuilder->train(supplyProviderType);
                }

            // Build Barracks/Gateway/Spawning Pool.
            }else if(infantryBuildingNeeded
              && minerals >= savingMinerals
              && infantryBuildingChecked + infantryBuildingCheckTimer < frameCount){
                infantryBuildingChecked = frameCount;

                if(buildBuilding(
                  unit,
                  infantryBuilding
                )){
                    infantryBuildingNeeded = false;
                    savingMinerals = 0;
                }

            }else if(unitIsIdle){
                // Return resources.
                if(unit->isCarryingMinerals()
                  || unit->isCarryingGas()){
                    unit->returnCargo();

                // Gather resources.
                }else{
                    unit->gather(unit->getClosestUnit(IsMineralField || IsRefinery));
                }
            }

        }else if(unitIsIdle){
            // Handle Command Centers, Hatcheries, and Nexuses.
            if(unitType.isResourceDepot()){
                if(minerals >= savingMinerals + 50){
                    // Train workers.
                    unit->train(playerRace.getWorker());
                }

            // Handle Barracks and Gateways.
            }else if(unit->canTrain(infantryType)){
                if(minerals >= savingMinerals + 100){
                    // Train Marines and Zealots.
                    unit->train(infantryType);
                }

            // Everything else should attack-move scout.
            }else{
                Position position = unit->getPosition();
                position.x += rand() % 501 - 250;
                position.y += rand() % 501 - 250;
                unit->attack(position);
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
    infantryBuildingCheckTimer = 2000;
    infantryBuildingNeeded = false;
    playerRace = Broodwar->self()->getRace();
    savingMinerals = 0;
    supplyChecked = 0;
    supplyCheckTimer = 600;
    supplyNeeded = false;
    supplyProviderType = playerRace.getSupplyProvider();

    supplyProviderTypeIsBuilding = supplyProviderType.isBuilding();

    // Handle race-specific stuff.
    if(playerRace == Races::Zerg){
        infantryBuilding = UnitTypes::Zerg_Spawning_Pool;
        infantryType = UnitTypes::Zerg_Zergling;

    }else if(playerRace == Races::Terran){
        infantryBuilding = UnitTypes::Terran_Barracks;
        infantryType = UnitTypes::Terran_Marine;

    }else{
        infantryBuilding = UnitTypes::Protoss_Gateway;
        infantryType = UnitTypes::Protoss_Zealot;
    }

    Broodwar->sendText("glhf");
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

