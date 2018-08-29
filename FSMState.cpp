/*
 *  FSMState.cpp
 *  tractionedge
 *
 *  Created by Steven Hamilton on 28/09/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#include "FSMState.h"
#include "Logger.h"
#include "Utility.h"

AIStatePatrol::AIStatePatrol()
{
	type=AI_STATE_PATROL;
}

void AIStatePatrol::enter(Creature* creature)
{
    Utility tool;
	LOG << creature->objectId << ", Entering Patrol State";
	//choose a location to move to.
	//must be reachable, within distance, non-blocked.
    //firstly just pick a random spot within 10 blocks.
    //store a movelist to get there. 
    point=creature->aiFindPatrolPoint();
    if (point.validatePosition()){
        moves=tool.findPath(creature->position, point);
        movesIter = moves.end();
    }
    if (creature->isWielding(RANGED)) creature->setReserve(SNAPSHOT);
    if (creature->isWielding(MELEE)) creature->setReserve(NONE);
    LOG << creature->objectId << " at " << creature->position.string() << " is patrolling to " << point.string();
}

void AIStatePatrol::update(Creature* creature)
{
    //Patrol State will pick a suitable patrol point and move towards it.
	Utility tool;
    LOG << creature->objectId << " Updating Patrol State";
    
    //check we have AP to do anything. If not, end Turn.
    if (creature->currentActionPoints==0){
        creature->endTurn=true;
        return;
    }

    //update awareness
    creature->processFOV();

    //if we can see an enemy do something 
    if (creature->visibleEnemies.size()>0){
        //check whether ranged or melee weapon
        if (creature->wieldingAttackType()==RANGED){ 
            //first look for a safe spot to run to and get the cost of that move.
            //If we can attack and still reach safety then do so. If not, just run
            //for it.
            LOG << creature->objectId << "Spotted Enemy, finding Safety";
            Position safety=creature->aiFindSafety(); 
            if (safety.x==-1 && safety.y==-1){
                LOG << creature->objectId << " No Safety Found. AARGH";
            }
            LOG << creature->objectId << " safety: " << safety.string();        
            int safetyMoveCost=tool.costForMove(creature->position, safety);
            //this needs refactored out. firing costs should be in creature.
            creature->setFiringMode(SNAPSHOT);
            creature->destination = safety;
            creature->destinationMoveCost = safetyMoveCost;
            if (creature->hasActionPoints(creature->getFiringCost()+safetyMoveCost)){
                LOG << creature->objectId << " Have AP's. Will attack";
                creature->aiFSM.changeState(creature, AI_STATE_ATTACK);
            } else {
                LOG << creature->objectId << " Not enough AP's, will run";
                creature->aiFSM.changeState(creature, AI_STATE_RUN);
            }
            return;
        } else if (creature->wieldingAttackType()==MELEE){
            //melee creatures will charge regardless of safety
            creature->aiFSM.changeState(creature, AI_STATE_CHARGE);
            return;            
        }

    }   

    //if we have reached patrol point pick a new destination 
    if (creature->position==point){
        point=creature->aiFindPatrolPoint();
        if (point.validatePosition()){
            moves=tool.findPath(creature->position, point);
            movesIter = moves.end();
		    LOG << creature->objectId << " Reached patrolPoint, now point is " << point.string();
        } else {
        //nowhere to go. Mark end of turn and idle;
        creature->endTurn=true;
        return;
        }
    }

    //if we're not stuck, and our point is valid then move.
    if (point.validatePosition()){
        //check we have enough AP for the next move and move towards patrol point
        movesIter--;
        Position nextPosition=tool.getPositionFromDirection(creature->position, (*movesIter));
        int moveCost=WORLD.map.getMoveCost(nextPosition);
        if (creature->hasActionPoints(moveCost))
        {
            LOG << creature->objectId << " at position " << creature->position.string() << " with AP " << tool.stringFromInt(creature->currentActionPoints)
                     << " target is "+point.string();
            //if we can't move for whatever reason end the creatures turn. Probably got stuck somewhere.
            //movesIter--;
            bool moved=creature->move((*movesIter));
            if (!moved) {
                creature->endTurn=true;
                LOG << creature->objectId << "could not move. Ending Turn";
                movesIter++;
                return;
            }
            LOG << creature->objectId << "moving in direction " << tool.stringFromInt(*movesIter) << " Position is now " << creature->position.string();
        } else {
            //No AP's so end turn. Ensure we increment the movesIter so we pick up
            //the correct move on the next turn.
            movesIter++;
            creature->endTurn=true;
            LOG <<creature->objectId << "No ActionPoints: ending turn";
        }
    } else {
        creature->endTurn=true;
    }
}

void AIStatePatrol::exit(Creature* creature)
{
	LOG << creature->objectId << "Exiting Patrol State";
}

//-------
AIStateAttack::AIStateAttack()
{
	type=AI_STATE_ATTACK;
}

void AIStateAttack::enter(Creature* creature)
{
	LOG << creature->objectId << "Entering Attack State";
}

void AIStateAttack::update(Creature* creature)
{
	LOG << creature->objectId << "Updating Attack State";
    
    //update awareness
    creature->processFOV();

    //verify we can still see a target
    if (creature->visibleEnemies.size()>0){
        creature->aiFindTarget();
        //attack while we have points then run.
        creature->setFiringMode(SNAPSHOT);
        while (creature->hasActionPoints(creature->getFiringCost()+creature->destinationMoveCost)){
            creature->attack(RANGED, NO_MOVE);
        }
        creature->aiFSM.changeState(creature, AI_STATE_RUN);
        LOG << creature->objectId << " Out of AP. Running";
    } else {
        //no visible enemies so return to Patrol state
        creature->aiFSM.changeState(creature, AI_STATE_PATROL);
        LOG << creature->objectId << " No visibleEnemies: return to Patrol";
    }
}

void AIStateAttack::exit(Creature* creature)
{
	LOG << creature->objectId << " Exiting Attack State";
}

//-------
AIStateAttackMelee::AIStateAttackMelee()
{
	type=AI_STATE_ATTACK_MELEE;
}

void AIStateAttackMelee::enter(Creature* creature)
{
	LOG << creature->objectId << " Entering Attack Melee State";
}

void AIStateAttackMelee::update(Creature* creature)
{
	LOG << creature->objectId << "Updating Attack Melee State";
    
    //update awareness
    creature->processFOV();

    //verify we can still see a target
    if (creature->visibleEnemies.size()>0){
        creature->aiFindTarget();
        //attack while we have points then run.
        creature->setFiringMode(SNAPSHOT);
        while (creature->hasActionPoints(creature->getFiringCost()+creature->destinationMoveCost)){
            creature->attack(RANGED, NO_MOVE);
        }
        creature->aiFSM.changeState(creature, AI_STATE_RUN);
        LOG << creature->objectId << "Out of AP. Running";
    } else {
        //no visible enemies so return to Patrol state
        creature->aiFSM.changeState(creature, AI_STATE_PATROL);
        LOG << creature->objectId << "No visibleEnemies: return to Patrol";
    }
}

void AIStateAttackMelee::exit(Creature* creature)
{
	LOG << creature->objectId << "Exiting Attack Melee State";
}

//-------
AIStateCharge::AIStateCharge()
{
	type=AI_STATE_CHARGE;
}

void AIStateCharge::enter(Creature* creature)
{
    LOG << creature->objectId << " Entering Charge State";
    Utility tool;
    //set target to first visible enemy (change this to nearest at some point
    creature->processFOV();
    creature->aiFindTarget();
    if (creature->visibleEnemies.size()>0){
        point=creature->visibleEnemies[0];
        moves=tool.findPath(creature->position, point);
        movesIter = moves.end();
        LOG << creature->objectId << " at " << creature->position.string() << " is charging to " << point.string();
    }
}

void AIStateCharge::update(Creature* creature)
{
	Utility tool;
    
    //Charge towards target with no regard of safety.
    LOG << creature->objectId << " Updating Charge State";
    
    //first check we can reach our charge point and abort if not.
    if (moves.size()==0){
		LOG << creature->objectId << " Cannot reach chargePoint. IDLING in CHARGE_STATE";
        creature->endTurn=true;
        return;
    }

    //check we have AP to do anything. If not, end Turn.
    if (creature->currentActionPoints==0){
        creature->endTurn=true;
        return;
    }

    //update awareness
    creature->processFOV();

    //if we have reached charge point then our attack has been successful.
    //Change back to patrol state.
    if (creature->position==point){
        creature->aiFSM.changeState(creature, AI_STATE_PATROL);
		LOG << creature->objectId << " Reached chargePoint. Changing to Patrol state";
        return;
    }

    //check we have enough AP for the next move and move towards charge point
    movesIter--;
    Position nextPosition=tool.getPositionFromDirection(creature->position, (*movesIter));
	int moveCost=WORLD.map.getMoveCost(nextPosition);
    if (creature->hasActionPoints(moveCost))
    {
		LOG << creature->objectId << "at position " << creature->position.string() << " with AP " << tool.stringFromInt(creature->currentActionPoints)
			 << " target is " << point.string();
		//if we can't move for whatever reason end the creatures turn. Probably got stuck somewhere.
        bool moved=creature->move((*movesIter));
		if (!moved) {
			creature->endTurn=true;
            LOG << creature->objectId << "could not move. Ending Turn";
            movesIter++;
            return;
		}
        LOG << creature->objectId << "moving in direction " << tool.stringFromInt(*movesIter) << " Position is now " << creature->position.string();
    } else {
        //No AP's so end turn. Ensure we increment the movesIter so we pick up
        //the correct move on the next turn.
        movesIter++;
        creature->endTurn=true;
        LOG << creature->objectId << " No ActionPoints: ending turn";
    }
}

void AIStateCharge::exit(Creature* creature)
{
	LOG << creature->objectId << " Exiting Charge State";
}

//-------
AIStateRun::AIStateRun()
{
	type=AI_STATE_RUN;
}

void AIStateRun::enter(Creature* creature)
{
    LOG << creature->objectId << " Entering Run State";
	Utility tool;
	type=AI_STATE_RUN;
    moves=tool.findPath(creature->position, creature->destination);
    movesIter = moves.end();
    creature->setReserve(NONE);
    LOG << creature->objectId << " at " << creature->position.string() << " is running to " << creature->destination.string();
}

void AIStateRun::update(Creature* creature)
{
	LOG << creature->objectId << " Updating Run State";
	Utility tool;
    //update awareness
    creature->processFOV();
    
    //return to Patrol if we've reached safety.
    if (creature->position==creature->destination){
        LOG << creature->objectId << " Reached safety: returning to Patrol";
        creature->aiFSM.changeState(creature, AI_STATE_PATROL);
        return;
    }

    //check we have enough AP for the next move and move towards safety
    movesIter--;
    Position nextPosition=tool.getPositionFromDirection(creature->position, (*movesIter));
	int moveCost=WORLD.map.getMoveCost(nextPosition);
    if (creature->hasActionPoints(moveCost))
    {
		LOG << creature->objectId << " at position " << creature->position.string() << " with AP " << tool.stringFromInt(creature->currentActionPoints)
			 << " safety is " << creature->destination.string();
		//if we can't move for whatever reason end the creatures turn. Probably got stuck somewhere. Do something better later.
        bool moved=creature->move((*movesIter));
		if (!moved) {
            movesIter++;
			creature->endTurn=true;
            LOG << creature->objectId << "Could not move: ending turn";
		}
        LOG << creature->objectId << " moving in direction " << tool.stringFromInt(*movesIter) << " Position is now " << creature->position.string();
    } else {
        //No AP's so end turn. Ensure we increment the movesIter so we pick up
        //the correct move on the next turn.
        movesIter++;
        creature->endTurn=true;
        LOG << creature->objectId << " No ActionPoints: ending turn";
    }
}

void AIStateRun::exit(Creature* creature)
{
	LOG << creature->objectId << " Exiting Run State";
}
