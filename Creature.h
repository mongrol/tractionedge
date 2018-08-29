/*
 *  Creature.h
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 25/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __CREATURE_H__
#define __CREATURE_H__

#include <map>
#include <vector>
#include <string>
#include "FSM.h"
#include "FSMState.h"
#include "DisplayManager.h"
#include "Position.h"
#include "Terrain.h"
#include "Item.h"
#include "def_creatures.h"
#include "def_items.h"

class GameWorld;

class Creature{
public:
	Creature();
	~Creature();
	Creature(creature_t, faction_t);
    //init string for Saver to load in Creatures
    Creature(std::map<std::string, std::string>&, Position, std::map<std::string, int>&);

    int objectId;
	creature_t type;

	//map data
	gfxId_t	gfxId;
	Position position;
	Position target;
	std::vector<std::vector<int> > fovMap;
	std::vector<Position> visibleEnemies;
	int visibleEnemiesIndex;
    bool onMap;

	//attributes
	std::string name;
    int level;
    int spareAttributePoints;
	int xp;
	int xpValue;
    int oldXP; //used to save current XP when entering a level
    int spareSkillPoints; //amount of skill points to spend
	int strength;
	int dexterity;
	int intelligence;
	faction_t faction;
	int	totalHitPoints;
	int totalActionPoints;
	int currentHitPoints;
	int currentActionPoints;

    //skills
	std::map<std::string, int> skills;
	void generateSkills(creature_t);

	//inventory & related modes
    int wielding;
    int capacity();
	bool addToInventory(Item * item);
	std::string encumberanceString();
	
	//management
	void resetAP();
	void resetHP();
    void resetOldXP();
	void processFOV();
	void processReaction();
	
	//actions
	bool move(move_t);
	int damage(int);
	
	//creature modes
	void setDead();
	
	//checks
	bool isAlive;
	bool hasActionPoints(int);
    bool isWielding();
    bool isWielding(itemAttackType_t);
    itemAttackType_t wieldingAttackType();


    //Actions
    bool attack(itemAttackType_t, move_t);
    double modifyAimAngle(double);
    void setFiringMode(firingMode_t);
    float firingModeMod;
    void setReserve(firingMode_t);
    bool reserveOn;
    firingMode_t getReserveMode();
    int getFiringCost();
    int getReserveCost();

    firingMode_t firingMode;
    firingMode_t reserveMode;

	//AI
	FSM aiFSM;
    bool endTurn;

    Position destination;
    int destinationMoveCost;

	bool aiFindTarget();
	Position aiFindSafety();
    Position aiFindPatrolPoint();
	void aiMoveTo(std::vector<move_t>);
};

#endif
