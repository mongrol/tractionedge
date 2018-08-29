/*
 *  FSM.h
 *  tractionedge
 *
 *  Created by Steven Hamilton on 28/09/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#ifndef FSM_H__
#define FSM_H__

#include <vector>
#include "FSMState.h"

enum aiFSM_t {
	AI_MACH_NONE,
	AI_MACH_SLITHER,
	AI_MACH_GIMP
};

class FSM {
private:
	int type;
    std::vector<FSMState*> states;
    std::vector<FSMState*>::iterator stateIt;
	FSMState * currentState;
public:
	FSM();
	~FSM();
	
	virtual void initMachine(int machineType);
	virtual void update(Creature* creature);
	virtual void addState(FSMState * state, bool makeCurrent);
	virtual bool changeState(Creature* creature,int newState);
};

#endif
