#include "ai.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace BWAPI;
using namespace Filter;

// Global variables.
bool supplyNeeded;
int infantryBuildingCheckTimer;
int infantryBuildingCost;
int infantryBuildingLimit;
int infantryCost;
int resourceDepotBuildingCheckTimer;
int resourceDepotBuildingCost;
int resourceDepotBuildingLimit;
int savingMinerals;
int supplyCheckTimer;
int workerLimit;
Race playerRace;
static bool overlordTraining;
static int infantryBuildingChecked;
static int resourceDepotBuildingChecked;
static int supplyChecked;
UnitType infantryBuilding;
UnitType infantryType;
UnitType resourceDepotBuilding;
UnitType supplyProviderType;
UnitType workerType;

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

    int infantryBuildingCount = Broodwar->self()->allUnitCount(infantryBuilding);
    int minerals = Broodwar->self()->minerals();
    int resourceDepotBuildingCount = Broodwar->self()->allUnitCount(resourceDepotBuilding);
    int supplyTotal = Broodwar->self()->supplyTotal();
    int workerCount = Broodwar->self()->allUnitCount(workerType);

    if(supplyTotal < 400
      && supplyTotal - Broodwar->self()->supplyUsed() <= 4
      && Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0
      && !overlordTraining){
        savingMinerals = 100;
        supplyNeeded = true;

    }else{
        savingMinerals = 0;
        supplyNeeded = false;
    }

    if(minerals >= infantryBuildingCost
      && infantryBuildingCount < infantryBuildingLimit){
        savingMinerals += infantryBuildingCost;
    }
    if(minerals >= resourceDepotBuildingCost
      && resourceDepotBuildingCount < resourceDepotBuildingLimit){
        savingMinerals += resourceDepotBuildingCost;
    }

    overlordTraining = false;

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

        // Check for training overlords.
        if(unitType == UnitTypes::Zerg_Egg
          && unit->getBuildType() == BWAPI::UnitTypes::Zerg_Overlord){
            overlordTraining = true;

        // Handle workers.
        }else if(unitType.isWorker()){
            // Handle insufficient supply by building Pylon or building Supply Depot.
            if(playerRace != Races::Zerg
              && supplyNeeded
              && minerals >= 100
              && supplyChecked + supplyCheckTimer < frameCount){
                supplyChecked = frameCount;

                Unit supplyBuilder = unit->getClosestUnit(GetType == supplyProviderType.whatBuilds().first
                  && (IsIdle || IsGatheringMinerals)
                  && IsOwned);

                buildBuilding(
                  supplyBuilder,
                  supplyProviderType
                );

            // Build Command Centers, Hatcheries, and Nexuses.
            }else if(resourceDepotBuildingCount < resourceDepotBuildingLimit
              && minerals >= resourceDepotBuildingCost
              && resourceDepotBuildingChecked + resourceDepotBuildingCheckTimer < frameCount){
                resourceDepotBuildingChecked = frameCount;

                buildBuilding(
                  unit,
                  resourceDepotBuilding
                );

            // Build Barracks/Gateway/Spawning Pool.
            }else if(infantryBuildingCount < infantryBuildingLimit
              && minerals >= infantryBuildingCost
              && infantryBuildingChecked + infantryBuildingCheckTimer < frameCount){
                infantryBuildingChecked = frameCount;

                buildBuilding(
                  unit,
                  infantryBuilding
                );

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
                if(supplyNeeded
                  && playerRace == Races::Zerg
                  &&!overlordTraining
                  && minerals >= 100
                  && supplyChecked + supplyCheckTimer < frameCount){
                    supplyChecked = frameCount;

                    // Train Overlords.
                    unit->train(supplyProviderType);

                }else if(workerCount < workerLimit
                  && minerals >= savingMinerals + 50){
                    // Train workers.
                    unit->train(workerType);

                }else if(playerRace == Races::Zerg
                  && minerals >= savingMinerals + infantryCost){
                    // Train Zerglings.
                    unit->train(infantryType);
                }

            // Handle Barracks and Gateways.
            }else if(playerRace != Races::Zerg
              && unit->canTrain(infantryType)){
                if(minerals >= savingMinerals + infantryCost){
                    // Train Marines and Zealots.
                    unit->train(infantryType);
                }

            // Everything else should attack-move scout.
            }else{
                Position position = unit->getPosition();
                position.x += rand() % 501 - 250;
                position.y += rand() % 501 - 250;
                if(!unit->attack(position)){
                    unit->move(position);
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
    Broodwar->setCommandOptimizationLevel(1);
    srand(time(NULL));

    // Setup global variables.
    infantryBuildingChecked = 0;
    infantryBuildingCheckTimer = 2000;
    overlordTraining = false;
    playerRace = Broodwar->self()->getRace();
    resourceDepotBuildingChecked = 0;
    resourceDepotBuildingCheckTimer = 2000;
    savingMinerals = 0;
    supplyChecked = 0;
    supplyCheckTimer = 600;
    supplyNeeded = false;
    supplyProviderType = playerRace.getSupplyProvider();
    workerLimit = 25;

    workerType = playerRace.getWorker();

    // Handle race-specific stuff.
    if(playerRace == Races::Zerg){
        infantryBuilding = UnitTypes::Zerg_Spawning_Pool;
        infantryBuildingCost = 200;
        infantryBuildingLimit = 1;
        infantryCost = 50;
        infantryType = UnitTypes::Zerg_Zergling;
        resourceDepotBuilding = UnitTypes::Zerg_Hatchery;
        resourceDepotBuildingCost = 300;
        resourceDepotBuildingLimit = 5;

    }else if(playerRace == Races::Terran){
        infantryBuilding = UnitTypes::Terran_Barracks;
        infantryBuildingCost = 150;
        infantryBuildingLimit = 5;
        infantryCost = 50;
        infantryType = UnitTypes::Terran_Marine;
        resourceDepotBuilding = UnitTypes::Terran_Command_Center;
        resourceDepotBuildingCost = 400;
        resourceDepotBuildingLimit = 1;

    }else{
        infantryBuilding = UnitTypes::Protoss_Gateway;
        infantryBuildingCost = 200;
        infantryBuildingLimit = 5;
        infantryCost = 100;
        infantryType = UnitTypes::Protoss_Zealot;
        resourceDepotBuilding = UnitTypes::Protoss_Nexus;
        resourceDepotBuildingCost = 400;
        resourceDepotBuildingLimit = 1;
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
