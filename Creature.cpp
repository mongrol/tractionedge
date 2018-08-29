/*
 *  Creature.cpp
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 25/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.Creature
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include "Creature.h"
#include "DisplayManager.h"
#include "fov.h"
#include "GameWorld.h"
#include "Logger.h"
#include "NameGenerator.h"
#include "Utility.h"
#include "Item.h"

class GameWorld;

Creature::Creature()
{
	isAlive = true;
	faction = FRIEND;
	gfxId=PLAYER_S;
	fovMap.resize(MAPHEIGHT);
    endTurn = false;
    firingMode=NONE;
    for (int i=0; i < MAPHEIGHT; ++i)
		fovMap[i].resize(MAPWIDTH);
	//redraw=true;
}

Creature::~Creature()
{
}

Creature::Creature(creature_t creatureType, faction_t fact)
{
    Utility tool;
	NameGenerator nameGen;

    objectId=WORLD.getObjectId();
	type=creatureType;
    endTurn = false;
    reserveMode=NONE;
    onMap=false;
	fovMap.resize(MAPHEIGHT);
    for (int i=0; i < MAPHEIGHT; ++i)
		fovMap[i].resize(MAPWIDTH);

    level = 1;
	xp = 0;
    oldXP=0;
    spareSkillPoints=0;
    //initial skills list. Will be replaced by skills object in world.
    skills["Clubs"]=0;
    skills["Daggers"]=0;
    skills["HandGuns"]=0;
    skills["Carbines"]=0;
    skills["LongGuns"]=0;
    skills["ShortSwords"]=0;
    generateSkills(type);
	
    switch (type) {
        case HUMAN:{
			name = nameGen.name();
			xpValue=100;
			strength=tool.rollDice(3,6);
			dexterity=tool.rollDice(3,6);
			intelligence=tool.rollDice(3,6);
			totalHitPoints = strength + tool.rollDice(1,10);
			currentHitPoints = totalHitPoints;
			totalActionPoints = dexterity + tool.rollDice(4,6);
			currentActionPoints = totalActionPoints;
			isAlive = true;
			faction = fact;
			gfxId=PLAYER_S;
            wielding=0;
		}
			break;
		case SPIDER:{
			name = "Arach";
			xpValue=MONSTER_SPIDER_XP;
			strength=tool.rollDice(2,6);
			dexterity=tool.rollDice(3,6);
			intelligence=tool.rollDice(1,6);
			totalHitPoints = 7;
			currentHitPoints = totalHitPoints;
			totalActionPoints = 15;
			currentActionPoints = totalActionPoints;
			isAlive = true;
			faction = fact;
			gfxId=SPIDER_S;
			position.x = (rand() % (MAPWIDTH-10))+10;
			position.y = rand() % MAPHEIGHT;
			
			//init AI
			aiFSM.initMachine(AI_MACH_SLITHER);
			
            //create a weapon for it
			Item* weapon=WORLD.generateItem(WOOD_MANDIBLE, objectId);
            weapon->onMap=true;
			wielding = weapon->objectId;
		}
			break;
		case SLITHER:{
			name="Slither";
			xpValue=MONSTER_SLITHER_XP;
			strength=tool.rollDice(2,6);
			dexterity=tool.rollDice(3,6);
			intelligence=tool.rollDice(1,6);
			totalHitPoints=12;
			currentHitPoints=totalHitPoints;
			totalActionPoints=16;
			currentActionPoints=totalActionPoints;
			isAlive=true;
			faction=fact;
			gfxId=SLITHER_S;
			position.x=(rand() % (MAPWIDTH-10))+10;
			position.y=rand() % MAPHEIGHT;

			aiFSM.initMachine(AI_MACH_SLITHER);
			
            //create a weapon for it
			Item* weapon=WORLD.generateItem(BRASS_FANG, objectId);
            weapon->onMap=true;
            weapon->ownerObjectId=objectId;
			wielding = weapon->objectId;
		}
			break;
		case GIMP:{
			name = "Gimp";
			xpValue=MONSTER_GIMP_XP;
			strength=tool.rollDice(3,6);
			dexterity=tool.rollDice(3,6);
			intelligence=tool.rollDice(3,6);
			totalHitPoints = 20;
			currentHitPoints = totalHitPoints;
			totalActionPoints = 20;
			currentActionPoints = totalActionPoints;
			isAlive = true;
			faction = fact;
			gfxId=GIMP_S;
			position.x = (rand() % (MAPWIDTH-10))+10;
			position.y = rand() % MAPHEIGHT;
			
			//init AI
			aiFSM.initMachine(AI_MACH_GIMP);
			
            //create a weapon for it
			Item* weapon=WORLD.generateItem(MUSKETOON, objectId);
            weapon->onMap=true;
			wielding = weapon->objectId;
		}
			break;
		default:
			break;
	}
}

Creature::Creature(std::map<std::string, std::string> &attribs, Position pos,std::map<std::string, int> &skillmap)
{
    Utility tool;

    reserveMode=NONE;
	fovMap.resize(MAPHEIGHT);
    for (int i=0; i < MAPHEIGHT; ++i)
		fovMap[i].resize(MAPWIDTH);
    
    //update attributes
    objectId=tool.stringToInt(attribs["objectId"]);
    gfxId=(gfxId_t)tool.stringToInt(attribs["gfxId"]);
    name=attribs["name"];
    type=(creature_t)tool.stringToInt(attribs["type"]);
    faction=(faction_t)tool.stringToInt(attribs["faction"]);
    xp=tool.stringToInt(attribs["xp"]);
    xpValue=tool.stringToInt(attribs["xpValue"]);
    oldXP=tool.stringToInt(attribs["oldXP"]);
    spareSkillPoints=tool.stringToInt(attribs["spareSkillPoints"]);
    strength=tool.stringToInt(attribs["strength"]);
    dexterity=tool.stringToInt(attribs["dexterity"]);
    intelligence=tool.stringToInt(attribs["intelligence"]);
    totalHitPoints=tool.stringToInt(attribs["totalHitPoints"]);
    totalActionPoints=tool.stringToInt(attribs["totalActionPoints"]);
    currentHitPoints=tool.stringToInt(attribs["currentHitPoints"]);
    currentActionPoints=tool.stringToInt(attribs["currentActionPoints"]);
    wielding=tool.stringToInt(attribs["wielding"]);
    onMap=(bool)tool.stringToInt(attribs["onMap"]);
    isAlive=(bool)tool.stringToInt(attribs["isAlive"]);
    std::cout << "Creating custom creature : " << name << " wielding= " << wielding << std::endl;

    switch (type){
        case SPIDER:{
            //init AI
		    aiFSM.initMachine(AI_MACH_SLITHER);
            break;
            }
        case SLITHER:{
            //init AI
		    aiFSM.initMachine(AI_MACH_SLITHER);
            break;
            }
        case GIMP:{
		    aiFSM.initMachine(AI_MACH_GIMP);
            break;
            }
        default:
            break;
    }
    //update position
    position = pos;

    //update skills
    skills = skillmap;
}

void Creature::generateSkills(creature_t type)
{
    Utility tool;
	std::map<std::string, int>::iterator it;
    for (it=skills.begin();it!=skills.end();++it){
         it->second=tool.rollDice(4,10);
    }
    switch (type){
        case HUMAN:{
            break;
                   }
        case SLITHER:{
            break;
                   }
        case GIMP:{
            skills["Pistol"]=tool.rollDice(4,12);
            break;
                   }
        default:
        break;
    }
}

int Creature::capacity()
{
    return strength*STR_ENC_MODIFIER;
}

bool Creature::addToInventory(Item * item)
{
	//we check weight of object and current encumberance.
	std::vector<Item*> items=WORLD.getItems(objectId);
	std::vector<Item*>::iterator it;
	int encumberance=0;
	for (it=items.begin();it<items.end();it++){
		Item * item2=*it;
		encumberance+=item2->weight;
	}
	if (item->weight<=capacity()-encumberance){
		item->ownerObjectId=objectId;
		return true;
	}
	return false;
}

std::string Creature::encumberanceString()
{
	Utility tool;
	std::vector<Item*> items=WORLD.getItems(objectId);
	std::vector<Item*>::iterator it;
	int encumberance=0;
	for (it=items.begin();it<items.end();it++){
		Item * item=*it;
		encumberance+=item->weight;
	}
	std::string str=tool.stringFromInt(encumberance)+"/"+tool.stringFromInt(capacity());
	return str;
}

bool Creature::move(move_t direction)
{	
	bool moved=false;
	//validate the position is on map
    Utility tool;
    //validate move direction
    if (direction > 9){
        LOG << objectId << " Invalid move_t";
        return moved;
    }

    //get our new position
    Position newPosition=tool.getPositionFromDirection(position, direction);
    
    //check its on the map
    if (newPosition.validatePosition()){

        if (isWielding(MELEE)){
        //if someone occupies the new tile then attack
        if (WORLD.isTileOccupied(newPosition)){
            Creature mob = WORLD.getCreature(newPosition);
            if (faction != mob.faction) attack(MELEE,direction);
        }
        //if we're moving onto a block terrain type then attack it
        if (!WORLD.map.isTerrainPassable(newPosition)) attack(MELEE,direction);
        }
        //if noone occupies it then attempt a move
        if (!WORLD.isTileOccupied(newPosition)){
		    //first check how much a move costs
            int moveCost=WORLD.map.getMoveCost(newPosition);
            int reserveCost;
            if (reserveOn){
                reserveCost=getReserveCost();
            } else {
                reserveCost=0;
            }
			bool canMove = hasActionPoints(moveCost+reserveCost);
            LOG <<objectId << " moveCost:" << tool.stringFromInt(moveCost) << " onto "+newPosition.string();
            LOG <<objectId << "reserveCost:" << tool.stringFromInt(reserveCost) << " onto "+newPosition.string();
		    if (!canMove && reserveCost>0){
                if (faction==FRIEND) PROMPT("AP Reserved");
            } else if (!canMove){
                if (faction==FRIEND) PROMPT("Not enough AP");
            }

			//first ask the map if the tile is passable and we can afford to move
			if(WORLD.map.isTerrainPassable(newPosition) && canMove){
				//deduct AP
				currentActionPoints -= moveCost;
				//clear the tile we're leaving
				WORLD.map.setTilePassable(position, true);
				//move creature onto new tile
				position=newPosition;
				//make it unpassable
				WORLD.map.setTilePassable(position, false);
				moved=true;
			 }
        } 
	}
	return moved;
}

int Creature::damage(int damage)
{
    int resultantDamage = damage;
	currentHitPoints-=damage;
	if (currentHitPoints<=0){
		currentHitPoints=0;
		setDead();
	}
    Utility tool;
    std::cout << name << " is hit for " << damage << " and has " << currentHitPoints << " left " << std::endl;
    MSG(name + " is hit for " + tool.stringFromInt(damage)); 
    return resultantDamage;
}

void Creature::setDead()
{
	isAlive=false;
	onMap=false;
}

bool Creature::hasActionPoints(int cost)
{
	if (currentActionPoints >= cost) {
		return true;
	} else {
		return false;
	}
}

bool Creature::isWielding()
{
    bool result = false;
    if (wielding) result = true;
    //std::cout << "Creature Check : " << name << " wielding= " << wielding << std::endl;
    return result;
}

bool Creature::isWielding(itemAttackType_t type)
{
    bool result = false;
    if (wielding){
        Item * item = WORLD.getItemForObjectId(wielding);
        if (item->itemAttackType==type) result = true;
    }
    return result;
}

itemAttackType_t Creature::wieldingAttackType()
{
    itemAttackType_t result;
    if (isWielding(MELEE)) result = MELEE;
    else result = RANGED;
    return result;
}

void Creature::resetAP()
{
	currentActionPoints = totalActionPoints;
}

void Creature::resetHP()
{
	currentHitPoints = totalHitPoints;
}

void Creature::resetOldXP()
{
    oldXP=xp;
}

void Creature::processFOV()
{
	//updates creatures awareness
    Utility tool;
	Fov::prepareTransparencyMap();
	Fov::calculate (position.x, position.y, WORLD.map.parameterMap["visibility"]);
	//copy visible tiles into creatures fovMap
	for (int i=0;i < MAPWIDTH;i++){
		for (int j=0;j < MAPHEIGHT;j++){
			fovMap[j][i] = Fov::fovMap[i][j];
			//std::cout << fovMap[j][i];
		}
		//std::cout << std::endl;
	}
	
	//scan map for enemies and save their positions
	visibleEnemies.resize(0);
	//iterate the worlds onMap creatures and find the one's that matches a visible tile position and are enemies
    //get a list of all onMap creatures
    std::vector<Creature*> creatureList=WORLD.getCreatures(true);
	for (int p = 0;p < creatureList.size();p++){
		if (fovMap[creatureList[p]->position.y][creatureList[p]->position.x] && creatureList[p]->faction!=faction && creatureList[p]->isAlive) {
			visibleEnemies.push_back(creatureList[p]->position);
			//std::cout << "id:" << objectId << " " << name << " is adding enemy id:" << creatureList[p]->objectId << " " << creatureList[p]->name <<" "<< creatureList[p]->position.string() << std::endl;
		}
	}
	//std::cout << "visibleEnemies.size=" << tool.stringFromInt(visibleEnemies.size()) << std::endl;
}

void Creature::processReaction()
{
    Utility tool;
    //process our reaction to recent events. THis does not contain initiative
    //or chance code
    //first update awareness
    processFOV();
    WORLD.updateMapVisibility();
    //find a target and check we're wielding
    if(aiFindTarget() && isWielding(RANGED)){
        //roll for initiative
        //fetch targets DEX
        int targetDex;
        Creature target=WORLD.getCreature(WORLD.target);
        targetDex=target.dexterity;
        int toHit = (float)dexterity/(float)targetDex*100/2;
        int rollToDefend = tool.rollDice(1,100);
        //std::cout << "Reaction Roll: targetDex:" << targetDex << " dex:" << dexterity << " rollTODefend:" << rollToDefend << " against toHit:" << toHit << std::endl;
        if (toHit>rollToDefend){
            LOG << objectId << " Wins reaction roll. Attacking!";
            attack(RANGED, NO_MOVE);
        }
    }
}

// ACTION CODE

bool Creature::attack(itemAttackType_t attackType, move_t direction)
{
    LOG << "Creature::attack";
    Utility tool;
    bool attacked=false;
    //set firingMode mod
	switch (firingMode) {
		case AIMED:
			firingModeMod=SKILL_AIMED_MODIFIER;
			break;
		case SNAPSHOT:
			firingModeMod=1/SKILL_SNAPSHOT_MODIFIER;
		default:
			break;
	}
	//check we have AP to attack
    int firingCost=getFiringCost();
    LOG << objectId << " Attempting attack with AP:" << tool.stringFromInt(currentActionPoints) << " and firingCost:" << tool.stringFromInt(firingCost);
	//only attack if targetting something and we have action points
    if (hasActionPoints(firingCost)){
        switch (attackType){
            case RANGED:
                //don't shoot ourselves. :)
	            if (!(position==WORLD.target)){
		            //remove AP 
		            currentActionPoints -=firingCost;
		            //get exact angle to the target position
		            double angle = tool.getAngle(position, WORLD.target);
	
                    //LOG <<"angle:"+tool.stringFromDouble(angle));

		            //apply modifiers
		            double modangle = modifyAimAngle(angle);
                    // LOG <<"modangle:"+tool.stringFromDouble(modangle));
                    WORLD.attack(this, modangle);
                    LOG << objectId << name << " AP after attack is:" << tool.stringFromInt(currentActionPoints);
                    attacked=true;
                }
                break;
            case MELEE:{
                Item * item = WORLD.getItemForObjectId(wielding);
                int cost = item->properties["APCost"];
                currentActionPoints -= cost;
                WORLD.attack(this, direction);
                attacked=true;
                break;
            }
            default:
                break;
        }
    } else {
		LOG << objectId << " No AP for attack";
        attacked=false;
    }
    return attacked;
}

double Creature::modifyAimAngle(double targetAngle)
{
	Utility tool;
	
    //get the weapon we're attacking with
	Item * weapon = WORLD.getItemForObjectId(wielding);
    //std::cout << "Wielded weapon is:" << weapon->name << " using skill: " << weapon->skill << std::endl;

	
	//calc base range constant for selected weapon. This is the angle that
    //would definitely hit the target tile at "Range"
	float slope=0.5/weapon->properties["Range"];
	std::cout << "slope angle=" << tool.stringFromDouble(slope) << " range:" << weapon->properties["Range"] << std::endl;
	//get angle in degrees (0 is right)
    float toleranceAngle = atan(slope) * 180/PI * 2;
	std::cout << "tolerance angle=" << tool.stringFromDouble(toleranceAngle) << std::endl;
	
	//add skill mod
	float skillMod=(float)skills[weapon->skill]/100;
	skillMod=1/skillMod/2;
	toleranceAngle*=skillMod;
	std::cout << "tolerance angle after skillmod=" << tool.stringFromDouble(toleranceAngle) << " skillMod:" << tool.stringFromDouble(skillMod) << std::endl;	
	
 
	toleranceAngle*=firingModeMod;
	std::cout << "tolerance angle after firingModMod=" << tool.stringFromDouble(toleranceAngle) << " firingModeMod:" << tool.stringFromDouble(firingModeMod) << std::endl;	

	//add DEX mod
    //// this is modded against STR.
    float strMod=(float)strength/20;
    float dexMod=(float)dexterity/20*(100/weapon->weight)*strMod;
    toleranceAngle*=dexMod;
	std::cout << "tolerance angle after dexMod=" << tool.stringFromDouble(toleranceAngle) << " dexMod:" << tool.stringFromDouble(dexMod) << std::endl;	

	//modify our target angle by a random amount inside our tolerance.
	double angle=targetAngle+tool.randFloat(toleranceAngle*-1, toleranceAngle);
	std::cout << "final firing angle=" << tool.stringFromDouble(angle) << std::endl;
	return angle;
}

int Creature::getFiringCost()
{
    //returns the AP cost to firing the current weapon
	Item * item = WORLD.getItemForObjectId(wielding);
    int apCost=item->properties["APCost"];	
    int cost;
    switch (firingMode) {
		case SNAPSHOT:
			cost=apCost*SKILL_SNAPSHOT_MODIFIER;
		default:
            //AIMED or melee
		    cost=apCost*SKILL_AIMED_MODIFIER;
			break;
	}
    return cost;
}

void Creature::setFiringMode(firingMode_t mode)
{
	firingMode = mode;
}

void Creature::setReserve(firingMode_t mode)
{
    reserveMode=mode;
    if (mode==NONE){
        reserveOn=false;
    } else {
        reserveOn=true;
    }
    std::cout << "reserveOn= " << reserveOn << std::endl;
}

firingMode_t Creature::getReserveMode()
{
    return reserveMode;
}

int Creature::getReserveCost()
{
    //returns the AP cost to firing the current weapon
	Item * item = WORLD.getItemForObjectId(wielding);
    int apCost=item->properties["APCost"];	
    int cost;
    switch (reserveMode) {
		case AIMED:
		    cost=apCost*SKILL_AIMED_MODIFIER;
			break;
		case SNAPSHOT:
			cost=apCost*SKILL_SNAPSHOT_MODIFIER;
		default:
			break;
	}
    return cost;
}

//--------------------------------
//---------AI CODE BELOW HERE-----
//--------------------------------

bool Creature::aiFindTarget()
{
	Utility tool;
	double distance = 9999;
    bool targetting=false;

	//iterate through visibleEnemies and select the nearest valid one as target
	for (int i=0;i < visibleEnemies.size();i++){
		std::cout << "visibleEnemies.size=" << visibleEnemies.size() << std::endl;
		double newDistance=tool.getDistance(position, visibleEnemies[i]);
		if (newDistance<distance) {
			WORLD.target=visibleEnemies[i];
			//setTarget();
			std::cout << name << " is targetting " << visibleEnemies[i].x << ":" << visibleEnemies[i].y << " at distance " << newDistance << std::endl;
			return true;
		}
	}
	return targetting;
}

Position Creature::aiFindSafety()
{

	Utility tool;
	
	std::vector<Position> positions;
	positions.resize(0);
	int positionsIndex=0;
	Position goodPosition;
	bool foundSafety=false;
	
	//fetch a pathWeight map for our current position
	std::vector<std::vector<Position> > pathWeights=tool.pathWeight(position);
	
	//iterate map and select the one's within range
	for (int i=0; i<MAPHEIGHT;i++){
		for (int j=0; j<MAPWIDTH;j++){
			Position selectedPosition = pathWeights[i][j];
			if (selectedPosition.getPathWeight() <= currentActionPoints+1 && selectedPosition.getPathWeight()!=0 && selectedPosition.getPathWeight()!=1) positions.push_back(selectedPosition);
		}
	}
	
	//we then find the closest ones that we can't see our visible enemies from. To do this we must fetch a FOV map for each tile.
	
	//sort the list in order of pathweight so we start from the middle
	std::sort(positions.begin(), positions.end());
	
	//start iterating through all reachable positions.
	Position currentPosition;
retry:
	while (!foundSafety){
		currentPosition=positions[positionsIndex];
		//fetch FOV map from that position: NOTE! Remove monster positions from map before processing. Then they won't hide behind themselves.
		Fov::prepareTransparencyMap();
		//modify transparencyMap removing our monsters so they don't block vision
		std::vector<Creature*> creatureList = WORLD.getCreatures(ENEMY, true);

		for (int m=0;m<creatureList.size();m++){
			Fov::transparencyMap[creatureList[m]->position.x][creatureList[m]->position.y]=1;
		}
		//fetch FOV
		Fov::calculate (currentPosition.x, currentPosition.y, VISIBILITY_RANGE);
		
		//Iterate through all players;
        creatureList = WORLD.getCreatures(FRIEND, true);
		for (int p=0; p<creatureList.size();++p) {
			//if player's position is in the fovMap then it's visible. Jump out and try for next tile;
			if (Fov::fovMap[creatureList[p]->position.x][creatureList[p]->position.y]) {
				positionsIndex++;
                if (positionsIndex==positions.size()){
                        goodPosition.setXY(-1,-1);
                        goto giveup;
                        }
				goto retry;
			}
		}
		goodPosition=currentPosition;
		foundSafety=true;
	}
	
giveup:
	//print some debug
	//std::cout << "Found the following positions" << std::endl;
	//for (int i=0;i<20;i++){
	//	std::cout << positions[i].string() << std::endl;
	//}
	return goodPosition;
}

Position Creature::aiFindPatrolPoint()
{
    Utility tool;

    //returns a valid patrol point.
    Position point;
    std::vector<move_t> moves;
    int attempt=0;
    do {
	    point.x = rand() % MAPWIDTH;
	    point.y = rand() % MAPHEIGHT;
        //check we can reach it or it's not valid.
        moves=tool.findPath(position, point);
        attempt++;
    }
    while (moves.size()==0 && attempt < 20);
    //ensure we return an invalid position is the find failed;
    if (attempt>19) point.setXY(-1,-1);
    return point;
}

void Creature::aiMoveTo(std::vector<move_t> moves)
{
	//iterate moves list, move one square then update display until end
	for (int i=moves.size()-1; i > -1; i--) {
		move(moves[i]);
		std::cout << moves[i];
		DISPLAY.drawScreen();
		sf::Sleep(0.2f);
	}
	std::cout << std::endl;
}
