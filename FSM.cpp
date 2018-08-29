/*
 *  AIMachine.cpp
 *  tractionedge
 *
 *  Created by Steven Hamilton on 28/09/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#include "FSM.h"
#include "Logger.h"

FSM::FSM()
{
    currentState=NULL;
}

FSM::~FSM()
{
    states.clear();
}

void FSM::initMachine(int machineType)
{
	type=machineType;
	switch (type) {
		case AI_MACH_GIMP:
			addState(new AIStatePatrol(), true);
			addState(new AIStateAttack(), false);
            addState(new AIStateRun(),false);
			break;
		case AI_MACH_SLITHER:
			addState(new AIStatePatrol(), true);
			addState(new AIStateCharge(), false);
			addState(new AIStateAttackMelee(), false);
            addState(new AIStateRun(),false);
			break;
		default:
			break;
	}
}

void FSM::addState(FSMState * newState, bool makeCurrent)
{
	states.push_back(newState);
    if (makeCurrent){
		currentState=newState;
		currentState->enter();
	}
}

bool FSM::changeState(Creature * creature,int newType)
{
    FSMState * gotoState = NULL;
    for(stateIt=states.begin(); stateIt!=states.end();stateIt++){
        if ((*stateIt)->type == newType){
            gotoState=(*stateIt);
        }
    }
    if (gotoState==NULL)
    {
        //LOG << creature->objectId << " ERROR: FSM Trying to change to non-existent state";
        return false;
    }
    currentState->exit(creature);
    gotoState->enter(creature);
    currentState = gotoState;
	return true;
}

void FSM::update(Creature* creature)
{
	//exit if no state loaded
	if (currentState == NULL) return;
	
	//execute current state
	currentState->update(creature);
}
