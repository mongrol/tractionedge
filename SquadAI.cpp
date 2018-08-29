/*
 *  AI.cpp
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 3/09/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#include <iostream>
#include "GameWorld.h"
#include "SquadAI.h"
#include "DisplayManager.h"
#include "defines.h"
#include "Logger.h"
#include "Utility.h"

void SquadAI::initAI()
{
	//reset all our monsters
	creatureList = WORLD.getCreatures(ENEMY, true);

	for (int i=0;i < creatureList.size();i++){
		creatureList[i]->resetAP();
        creatureList[i]->endTurn=false;
        creatureList[i]->setFiringMode(SNAPSHOT);
    }
    creatureIt = creatureList.begin();
    endTurn=false;
}

void SquadAI::setInitialAIState()
{
	//We need to change the state of all monsters to PatrolState()
	creatureList = WORLD.getCreatures(ENEMY, true);
	for (int i=0;i < creatureList.size();i++){
		creatureList[i]->aiFSM.changeState(creatureList[i], AI_STATE_PATROL);
	}
}

bool SquadAI::processTurn()
{
	Utility tool;
	if ((*creatureIt)->isAlive && !(*creatureIt)->endTurn)
    {
	    std::cout << "------Processing-id:" << (*creatureIt)->objectId << " AP:" << (*creatureIt)->currentActionPoints << std::endl;
			
		//update state 
		(*creatureIt)->aiFSM.update((*creatureIt));
        
        //process reaction fire. PLacement of this here is a test. It may have
        //to move inside states.
        WORLD.checkReaction(FRIEND);        

    } else {
        std::cout << "------End Turn id:" << (*creatureIt)->objectId << std::endl;
        creatureIt++;
    }
    if (creatureIt==creatureList.end()) endTurn=true;
	return endTurn;
}
