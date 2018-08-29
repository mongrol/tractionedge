/*
 *  AI.h
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 3/09/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#ifndef __AI_H__
#define __AI_H__

#include <vector>
#include "DisplayManager.h"
#include "GameWorld.h"

class Creature;

class SquadAI {
private:
	std::vector<Creature*> creatureList;
    std::vector<Creature*>::iterator creatureIt;
    bool endTurn;
public:
	
	void initAI();
	void setInitialAIState();
	bool processTurn();
	bool findTarget(int x, int y, int x1, int y1);
};


#endif
