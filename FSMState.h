/*
 *  AIState.h
 *  tractionedge
 *
 *  Created by Steven Hamilton on 28/09/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#ifndef FSMSTATE_H__
#define FSMSTATE_H__

#include "defines.h"
#include "Position.h"
#include <vector>

class FSM;
class Creature;

class FSMState {
private:

public:
	int	type;
    FSM *fsm;

	FSMState(){};
	FSMState(FSM *fsm){};
	virtual ~FSMState(){};

	virtual void enter()					{}
	virtual void enter(Creature* creature)	{}
	virtual void update()                   {}
    virtual void update(Creature* creature) {}
	virtual void exit()						{}
	virtual void exit(Creature* creature)	{}

};

enum aiState_t {
	AI_STATE_NONE,
	AI_STATE_PATROL,
	AI_STATE_ATTACK,
    AI_STATE_ATTACK_MELEE,
    AI_STATE_RUN,
    AI_STATE_CHARGE
};
	
class Creature;

class AIStatePatrol : public FSMState {
private:
public:
	AIStatePatrol();
	virtual void enter(Creature* creature);
    virtual void update(){};
    virtual void update(Creature* creature);
	virtual void exit(Creature* creature);
    Position point;
    std::vector<move_t> moves;
    std::vector<move_t>::iterator movesIter;
};

class AIStateCharge : public FSMState {
private:
	Creature * creature;
public:
	AIStateCharge();
	virtual void enter(Creature* creature);
    virtual void update(){};
    virtual void update(Creature* creature);
	virtual void exit(Creature* creature);
    Position point;
    std::vector<move_t> moves;
    std::vector<move_t>::iterator movesIter;
};

class AIStateAttack : public FSMState {
private:
	Creature * creature;
public:
	AIStateAttack();
	virtual void enter(Creature* creature);
    virtual void update(){};
    virtual void update(Creature* creature);
	virtual void exit(Creature* creature);
};

class AIStateAttackMelee : public FSMState {
private:
	Creature * creature;
public:
	AIStateAttackMelee();
	virtual void enter(Creature* creature);
    virtual void update(){};
    virtual void update(Creature* creature);
	virtual void exit(Creature* creature);
};

class AIStateRun : public FSMState {
private:
	Creature * creature;
public:
	AIStateRun();
	virtual void enter(Creature* creature);
    virtual void update(){};
    virtual void update(Creature* creature);
	virtual void exit(Creature* creature);
    std::vector<move_t> moves;
    std::vector<move_t>::iterator movesIter;
};
#endif
