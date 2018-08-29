/*
 *  GameWorld.cpp
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 21/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include "Creature.h"
#include "DisplayManager.h"
#include "fov.h"
#include "Map.h"
#include "NameGenerator.h"
#include "Terrain.h"
#include "GameWorld.h"
#include "Utility.h"
#include "defines.h"
#include "Logger.h"

GameWorld::GameWorld()
{
	objectIdIndex=0;
    consoleBuffer.resize(CONSOLEPANELHEIGHT);
	selectionGfxId=TARGET_S;
}

GameWorld::~GameWorld()
{
}

void GameWorld::initWorld()
{
    std::cout << "Initilising world with level 0" << std::endl;
    //since the World owns objects that noone else owns it needs an objectId.
    objectId=getObjectId();
    level=0;
    score=0;
    finalLevel=4;
    items.resize(0);
    creatures.resize(0);

    //new populate code. Move this into xml!
    std::vector<creature_t> level1;
    level1.resize(0);
    level1.push_back(SPIDER);
    levelPopulations.push_back(level1);
    
    std::vector<creature_t> level2;
    level2.resize(0);
    level2.push_back(SPIDER);
    level2.push_back(SLITHER);
    levelPopulations.push_back(level2);
    
    std::vector<creature_t> level3;
    level3.resize(0);
    level3.push_back(SLITHER);
    level3.push_back(GIMP);
    levelPopulations.push_back(level3);
    
    std::vector<creature_t> level4;
    level4.resize(0);
    level4.push_back(GIMP);
    levelPopulations.push_back(level4);
}

void GameWorld::populateLevel()
{
    Utility tool;
    //somehow decide how many mobs we need and then what size of mob
    int numberOfMobs = tool.randNum(5,7);
   
    //get what mobs we can use for this level
    std::vector<creature_t> mobs = levelPopulations[level-1];
    int mobIndex = mobs.size();
    std::cout << "populateLevel " << level <<":choosing from " << mobIndex << " mobs" << std::endl;

    //create mobs until totalMobXP is reached
    for (int i = 0 ; i < numberOfMobs; i++){
        int n=0;
        // workaround for randNum(0,1);
        if (mobIndex > 1) n = (tool.randNum(1,mobIndex*10))/10;
        
        Creature creature(mobs[n], ENEMY);
        creature.onMap = true;
		addCreature(creature);
        std::cout << "adding " << creature.name << std::endl;
    }
}

event_t	GameWorld::checkMatchStatus(event_t event)
{
	std::vector<Creature*> creats;
	creats=getCreatures(FRIEND,true);
	if (creats.size()==0) {
		event=EVENT_MATCH_LOST;
	}
	creats=getCreatures(ENEMY,true);
	if (creats.size()==0) {
		event=EVENT_MATCH_WIN;
	}
	return event;
}

int GameWorld::getScore()
{
    return score;
}

int GameWorld::getObjectId()
{
	//returns the next objectId;
	//std::cout << "objectIdIndex is " << objectIdIndex << std::endl;
    return objectIdIndex++;
}

void GameWorld::generateMap()
{	
	map=Map();
	map.loadMap(level);	
	if (level>2) map.parameterMap["squadsize"]=3;
    populateLevel();
}

void GameWorld::generateItems()
{
    for (int i=-0;i<3;i++){
        Item * item=generateItem(PISTOL, objectId);
    }
//	for (int i=-0;i<1;i++){
  //      Item * item=generateItem(MUSKET, objectId);
//    }
//	for (int i=-0;i<2;i++){
//        Item * item=generateItem(MUSKETOON, objectId);
 //   }
//	for (int i=-0;i<5;i++){
//        Item * item=generateItem(CUTLASS, objectId);
//    }
	for (int i=-0;i<5;i++){
        Item * item=generateItem(BILLYCLUB, objectId);
    }
}

void GameWorld::regeneratePlayers()
{
    //this checks the global number of humans alive and tops them up
    Utility tool;
    srand (time(NULL));
	std::vector<Creature*> humans = getCreatures(FRIEND);
	std::cout << "Humans = " << humans.size() << std::endl;
//	NameGenerator nameGen;
    if (humans.size() < NUMBEROFBASEPLAYERS) {
		int need = NUMBEROFBASEPLAYERS-humans.size();
		std::cout << "Topping up with " << need << " humans" << std::endl;
		for (int i = 0;i < need; i++){
			faction_t faction = FRIEND;
			Creature creature(HUMAN, faction);
			addCreature(creature);
        }
	}
    std::cout << "Now creatures = " << creatures.size() << std::endl;
}

void GameWorld::addCreature(Creature & creature)
{
    creatures.push_back(creature);
}

void GameWorld::delCreature(Creature  * creature)
{
    std::vector<Creature>::iterator it;

    for (it=creatures.begin();it<creatures.end();++it){
        std::cout << "Matching creature " << creature->name << " to " << it->name <<  std::endl;
        Creature * creat = &*it;
        if (creat==creature){
            std::cout << " and erasing" << std::endl;
            creatures.erase(it);
			break;
        }
    }
}

std::vector<Creature*> GameWorld::getCreatures()
{
    std::vector<Creature*> results;
    //returns all creature for specified faction
    for (int i=0;i < creatures.size();i++){
        results.push_back(&WORLD.creatures[i]);
    }
    return results;
}

std::vector<Creature*> GameWorld::getCreatures(faction_t faction)
{
    std::vector<Creature*> results;
    //returns the creature for specified faction
    for (int i=0;i < creatures.size();i++){
        //iterate and check faction
        if (creatures[i].faction==faction && creatures[i].isAlive) results.push_back(&WORLD.creatures[i]);
    }
    return results;
}


std::vector<Creature*> GameWorld::getCreatures(faction_t faction, bool mapStatus)
{
    std::vector<Creature*> results;
    //returns the creature for specified faction
    for (int i=0;i < creatures.size();i++){
        //iterate and check faction
        if (creatures[i].faction==faction && creatures[i].onMap==mapStatus && creatures[i].isAlive) results.push_back(&WORLD.creatures[i]);
    }
    return results;
}

std::vector<Creature*> GameWorld::getCreatures(bool mapStatus)
{
    std::vector<Creature*> results;
    //returns the creature for specified faction
    for (int i=0;i < creatures.size();i++){
        //iterate and check faction
        if (creatures[i].onMap==mapStatus && creatures[i].isAlive) results.push_back(&WORLD.creatures[i]);
    }
    return results;
}

Creature & GameWorld::getCreature(Position pos)
{
    for (int i=0;i < creatures.size();i++){
        //iterate and check faction
        if (creatures[i].position==pos && creatures[i].isAlive){
            Creature &result=creatures[i];
            return result;
        }
    }
}

int GameWorld::getNumberOfCreatures(faction_t faction, bool mapStatus)
{
	std::vector<Creature*> results=getCreatures(faction, mapStatus);
    return results.size();
}

Item * GameWorld::generateItem(itemType_t itemType, int ownerId)
{
    Item* item=new Item(itemType, ownerId);
    items.push_back(*item);
    return item;
}

void GameWorld::addItem(Item & item)
{
    items.push_back(item);
}

Item * GameWorld::getItemForObjectId(int objectId)
{
	//returns the item pointer for objectId
	Item * item;
	for (int i=0;i < items.size();i++){
        if (items[i].objectId==objectId) item=&items[i];
    }
    return item;
}

std::vector<Item*> GameWorld::getItems()
{
    std::vector<Item*> results;
    //returns all Items 
    for (int i=0;i < items.size();i++){
        results.push_back(&WORLD.items[i]);
    }
    return results;
}

std::vector<Item*> GameWorld::getItems(int objectId)
{
    std::vector<Item*> results;
    //returns the Items owned by the specified objectId 
    for (int i=0;i < items.size();i++){
        //iterate and check faction
//        std::cout << "item.ownerObjectId= " << items[i].ownerObjectId << std::endl;
        if (items[i].ownerObjectId==objectId) results.push_back(&WORLD.items[i]);
    }
    return results;
}

std::vector<Item*> GameWorld::getItems(itemType_t itemType)
{
    std::vector<Item*> results;
    //returns the Items for specified itemType
    for (int i=0;i < items.size();i++){
        //iterate and check faction
        if (items[i].type==itemType) results.push_back(&WORLD.items[i]);
    }
    return results;
}


std::vector<Item*> GameWorld::getItems(itemType_t itemType, bool mapStatus)
{
    std::vector<Item*> results;
    //returns the Items for specified itemType
    for (int i=0;i < items.size();i++){
        //iterate and check faction
        if (items[i].type==itemType && items[i].onMap==mapStatus) results.push_back(&WORLD.items[i]);
    }
    return results;
}

std::vector<Item*> GameWorld::getItems(bool mapStatus)
{
    std::vector<Item*> results;
    //returns the Items for the mapStatus  
    for (int i=0;i < items.size();i++){
        if (items[i].onMap==mapStatus) results.push_back(&WORLD.items[i]);
    }
    return results;
}

int GameWorld::getNumberOfItems(bool mapStatus)
{
	std::vector<Item*> results=getItems(mapStatus);
    return results.size();
}

//---DISPLAY CODE---
void GameWorld::updateMapVisibility()
{
//THIS SHOULD ALL BE MOVED INSIDE CREATURES AND ONLY TRIGGERED ON A MOVE.
	
	//mark all tiles not visible.
	for (int n=0; n< MAPHEIGHT; n++){
		for (int m=0; m< MAPWIDTH; m++){
			Position position(m,n);
			map.setTileVisibility(position, false);
		}
	}
	
	//begin new FOV code. Beware x & y coords are reversed in the MRPAS fov code.
	std::vector<Creature*> creatureList = getCreatures(FRIEND, true);
	
	for (int p = 0;p < creatureList.size();p++){
		if (creatureList[p]->isAlive && creatureList[p]->onMap){
			//tell player to fetch it's FOV
			creatureList[p]->processFOV();
			//set the players visible tiles on map
			for (int i=0;i < MAPWIDTH;i++){
				for (int j=0;j < MAPHEIGHT;j++){
					Position position(i,j);
					if (creatureList[p]->fovMap[j][i]) map.setTileVisibility(position,true);
				}
			}
		}
	}
}
	
//---END DISPLAY CODE---

std::string GameWorld::getVisibleElementNameAt(Position position)
{
	//we check entities in this order.
	//Players, Monsters, Items, Terrain and return the first positive.
	std::vector<Creature*> creatureList = getCreatures(true);

	for (int p = 0;p < creatureList.size();p++){
		if (creatureList[p]->position==position) {
			return creatureList[p]->name;
		}
	}
	//for (int p = 0;p < map.monsterList.size();p++){
//		if (map.monsterList[p].position==position) {
//			return map.monsterList[p].name;
//		}
//	}
	for (int p = 0;p < items.size();p++){
		if (items[p].position==position) {
			return items[p].name;
		}
	}
	return map.terrainNameAt(position);
}

bool GameWorld::isTileOccupied(Position position)
{
	//used to check if a creature is on a tile
	std::vector<Creature*> creatureList = getCreatures(true);

	for (int p = 0;p < creatureList.size();p++){
		if (creatureList[p]->position==position) {
		    LOG << "Creature name:" << creatureList[p]->name << " occupies " << position.string();
            return true;
		}
	}
	return false;
}

bool GameWorld::isTileBlocked(Position position)
{
	if (isTileOccupied(position)) return true;
	if (map.isTerrainPassable(position)) return false;
	return true;
}

bool GameWorld::haveHitHalfHeightTile(Position position)
{
    Utility tool;
    int height = map.getHeight(position);
    // std::cout << "Tile height = " << height << std::endl;
    if (height==1){
        int roll = tool.rollDice(1,100);
            if (roll<50) return true;
    }
    return false;
}

int GameWorld::damageEntityAtPosition(Creature * sourceCreature, Position destPos, Item &weapon, itemAttackType_t attackType)
{
    Utility tool;
    int resultantDamage = 0;
    Position sourcePos = sourceCreature->position;
    int damage = weapon.properties["Damage"];
    if (attackType == RANGED){
        int distance = (int)tool.getDistance(sourcePos, destPos);
        int baseRange = weapon.properties["Range"];
        int modDamage; 
        //add a random modifier of 60-140%
        float randMod = (float)tool.randNum(90,140)/100;
        std::cout << "randDamageMod:" << randMod << std::endl;    
        //if distance is less than range then reset it to range. This stop
        //overpowered point blank shots. (must be better way of doing this
        //curve.
        if (distance<baseRange) distance=baseRange;
        float mod = (float)baseRange/(float)distance*(float)damage*randMod;
        modDamage = (int)mod;
        LOG << "Hit Distance= " << tool.stringFromDouble(distance) << " modDamage=" << tool.stringFromInt(modDamage);
        damage = modDamage;
    }
    if (attackType == MELEE){
        int modDamage = damage*sourceCreature->strength/10;
        damage = modDamage;
    }

	//we check entities in this order
	//Players, Monsters, Terrain and damage in that order.
	std::vector<Creature*> creatureList = getCreatures(true);

	for (int p = 0;p < creatureList.size();p++){
		if (creatureList[p]->position==destPos && creatureList[p]->isAlive) {
			//found creature so lets damage it
            resultantDamage = creatureList[p]->damage(damage);
			
            //if creature is killed
			if (creatureList[p]->currentHitPoints<1) {
				//replace with corpse
                MSG(creatureList[p]->name + " endures an noble end");
				//Item* corpse=generateItem(CORPSE, WORLD.map.objectId);
				//corpse.name="Corpse of"+playerList[p].name; <- potential string length issue
				//corpse->position=creatureList[p]->position;
				//corpse->onMap=true;
				map.setTilePassable(creatureList[p]->position, true);
                
                //harvest his xp for the killer
				selectedPlayer->xp+=creatureList[p]->xpValue;

				//set creature dead. Do not delete, we may resurrect later.
                creatureList[p]->setDead();
				
				return resultantDamage;
			}
			return resultantDamage;
		}
	}
	resultantDamage = map.damage(destPos, damage);
    return resultantDamage;
}

void GameWorld::checkReaction(faction_t faction)
{
    //process reactions of creatures.
    std::vector<Creature*> creatures=getCreatures(faction);
    for (int i=0; i < creatures.size();i++)
    {
        creatures[i]->processReaction();
    }
}

void GameWorld::initPlayers()
{
	//reset action & hit points
	std::vector<Creature*> creatureList = getCreatures(true);
	
	for (int i=0; i < creatureList.size();i++)
	{
		Creature *player = creatureList[i];
		if (player->isAlive){
			player->resetAP();
			player->resetHP();
            player->resetOldXP();
            player->setFiringMode(NONE);
        } 
	}
}

void GameWorld::resetPlayers()
{
	//reset action points
	std::vector<Creature*> creatureList = getCreatures(true);

	for (int i=0; i < creatureList.size();i++)
	 {
		Creature *player = creatureList[i];
		if (player->isAlive){
			player->resetAP();
		} else {
			//delete player;
			//insert corpse;
		}
	 }
}

void GameWorld::updatePlayerLevel()
{
    //update players level, xp, skill stats after match end
	std::vector<Creature*> creatureList = getCreatures(FRIEND, true);
	for (int i=0; i < creatureList.size();i++)
	 {
		Creature *player = creatureList[i];
		if (player->isAlive){
            //update level
            int xpNeeded = getXPForLevel(player->level+1);
            if (player->xp>=xpNeeded){
                player->level++;
                player->spareAttributePoints++;
            }
            
            //update skillpoints
			player->spareSkillPoints=(player->xp-player->oldXP)/100;
		} else {
			//delete player;
			//insert corpse;
		}
	 }
}

int GameWorld::getXPForLevel(int level)
{
    int xp = level*100;
    return xp;
}

void GameWorld::selectLivePlayer()
{
	//first check if any players still live
	std::vector<Creature*> creatureList = getCreatures(FRIEND, true);
	bool playOn=false;
	
	for (int i=0; i < creatureList.size();i++)
	{
		Creature *player = creatureList[i];
		if (player->isAlive){
			playOn=true;
		} else {
			return;
		}
	}
	if (playOn){
		if (!selectedPlayer->isAlive) selectNextPlayer();
	}
}

void GameWorld::positionPlayers()
{
    Utility tool;
	int x1=map.startx1;
	int y1=map.starty1;
	int x2=map.startx2;
	int y2=map.starty2;
	std::vector<Creature*> squad=getCreatures(FRIEND,true);
    //std::cout << "squad size = " << squad.size() << std::endl;
	std::vector<Creature*>::iterator creatureIt;
	for (creatureIt=squad.begin(); creatureIt!=squad.end(); ++creatureIt) {
		Creature * creat=*creatureIt;
        Position pos;
		pos.x=tool.randNum(x1,x2);
		pos.y=tool.randNum(y1,y2);
        do {
		    pos.x=tool.randNum(x1,x2);
		    pos.y=tool.randNum(y1,y2);
        } while (!map.isTerrainPassable(pos) || isTileOccupied(pos));
        creat->position=pos;
        LOG << "Positioning " << creat->name+" at " << creat->position.string();
	}
}

void GameWorld::selectFirstPlayer()
{	
	//select first player that's onMap and Alive
	creatureIt = creatures.begin();
	
	bool found=false;
	while (!found) {
		if ((creatureIt->onMap && creatureIt->isAlive && creatureIt->faction==FRIEND)){
			found = true;
			selectedPlayer=&*creatureIt;
			break;
		}
		++creatureIt;
	}
	//std::cout << "selectedPlayer is " << selectedPlayer->name << std::endl;
}

void GameWorld::selectNextPlayer()
{
	//select next player that's onMap and Alive
	bool found=false;
	while (!found) {
		++creatureIt;
		if (creatureIt==creatures.end()) creatureIt=creatures.begin();
		if ((creatureIt->onMap && creatureIt->isAlive && creatureIt->faction==FRIEND)){
			found = true;
			selectedPlayer=&*creatureIt;
		} else {
		
		}

	}
	//std::cout << "selectedPlayer is now " << selectedPlayer->name << std::endl;
}

//--selection and targetting--

void GameWorld::initSelectionCursor()
{
	target = selectedPlayer->position;
}

void GameWorld::endSelectionCursor()
{
	//selectedPlayer->isTargetting=false;
}

void GameWorld::moveSelectionCursor(int direction)
{	
	//our destination coordinates
	int destx, desty;
	Position newPosition;
	
	//set the coords of the tile we're moving to.
	switch (direction){
		case MOVE_RIGHT:
			newPosition.setXY(std::min(target.x+1,MAPWIDTH-1), target.y);
			break;
		case MOVE_LEFT:
			newPosition.setXY(std::max(target.x-1,0), target.y);
			break;
		case MOVE_UP:
			newPosition.setXY(target.x, std::max(target.y-1,0));
			break;
		case MOVE_DOWN:
			newPosition.setXY(target.x, std::min(MAPHEIGHT-1,target.y+1));
			break;
		case MOVE_UP_RIGHT:
			newPosition.setXY(std::min(target.x+1,MAPWIDTH-1), std::max(target.y-1,0));
			break;
		case MOVE_UP_LEFT:
			newPosition.setXY(std::max(target.x-1,0), std::max(target.y-1,0));
			break;
		case MOVE_DOWN_RIGHT:
			newPosition.setXY(std::min(target.x+1,MAPWIDTH-1), std::min(MAPHEIGHT-1,target.y+1));
			break;
		case MOVE_DOWN_LEFT:
			newPosition.setXY(std::max(target.x-1,0), std::min(MAPHEIGHT-1,target.y+1));
			break;
	}
	//move cursor onto new tile
	target=newPosition;
    LOG << target.string();
}

void GameWorld::moveSelectionCursorTo(Position pos)
{
	//move cursor onto new tile
	target=pos;
}

void GameWorld::targetNextEnemy(Creature * creature)
{
	//iterates around visible enemies
	if (!creature->visibleEnemies.empty()){
		creature->visibleEnemiesIndex++;
		if (creature->visibleEnemiesIndex > creature->visibleEnemies.size()-1) creature->visibleEnemiesIndex = 0;
		WORLD.moveSelectionCursorTo(creature->visibleEnemies[creature->visibleEnemiesIndex]);
		//std::cout << "Selecting " << creature->visibleEnemiesIndex << "from " << creature->visibleEnemies.size() << std::endl;
	}
}

void GameWorld::sendMessageToConsole(std::string message)
{
	//add new message to the end of the buffer and remove the oldest from the front
	consoleBuffer.push_back(message);
	consoleBuffer.erase(consoleBuffer.begin());
}

void GameWorld::clearConsole()
{
	//std::cout << "Clearing consoleBuffer" << std::endl;
	consoleBuffer.clear();
	consoleBuffer.resize(CONSOLEPANELHEIGHT);
}

void GameWorld::sendMessageToPrompt(std::string message)
{
    promptString=message;
}

void GameWorld::clearPrompt()
{
    promptString="";
}

//--attack actions--

void GameWorld::attack(Creature * creature, double angle)
{
    //called from creatures Attack function once mod angle is calc'd
	Utility tool;
	WORLD.anim.initialiseProjectile();
	
	//only attack if targetting something and we have action points

	//get the Position we've hit.
	Position hitTile = WORLD.fireAtAngle(creature->position, angle);

    LOG << "hitTile="+hitTile.string();

	//Draw project and oop until anim is finished.
	PROMPT("Bang!");
	DISPLAY.drawScreen();
	DISPLAY.drawScreen(); //draw twice to work around ATI double buffer error
	DISPLAY.animate();

	//chance damage on it.
	Item * item = getItemForObjectId(creature->wielding);
    int resultantDamage = WORLD.damageEntityAtPosition(creature, hitTile, *item, RANGED);
    std::string msg = creature->name + " hits for " + tool.stringFromInt(resultantDamage);
    //MSG(msg);
    //update display with damaged target
    DISPLAY.drawScreen();
}

void GameWorld::attack(Creature * creature, move_t direction)
{
    //melee attack
    Utility tool;
    Position hitTile = creature->position;
	Item * item = getItemForObjectId(creature->wielding);
    switch (direction){
        case MOVE_UP:{
            hitTile.modXY(0,-1);
            break;
            }
        case MOVE_DOWN:{
            hitTile.modXY(0,1);
            break;
            }
        case MOVE_LEFT:{
            hitTile.modXY(-1,0);
            break;
            }
        case MOVE_RIGHT:{
            hitTile.modXY(1,0);
            break;
            }
        case MOVE_UP_RIGHT:{
            hitTile.modXY(1,-1);
            break;
            }
        case MOVE_UP_LEFT:{
            hitTile.modXY(-1,-1);
            break;
            }
        case MOVE_DOWN_RIGHT:{
            hitTile.modXY(1,1);
            break;
            }
        case MOVE_DOWN_LEFT:{
            hitTile.modXY(-1,1);
            break;
            }
        default:
            break;
        }
    LOG << "Pos=" << creature->position.string() << ": hitTile=" << hitTile.string();
    int resultantDamage = WORLD.damageEntityAtPosition(creature, hitTile, *item, MELEE);
    std::string msg = creature->name + " hits for " + tool.stringFromInt(resultantDamage);
    MSG(msg);
}

Position GameWorld::fireAtAngle(Position src, double modangle)
{
	Utility tool;
	int x1 = src.x;
	int y1 = src.y;
	
	//convert back to slope
	double modslope=tan(modangle*PI/180);
	//	std::cout << "mod slope=" << tool.stringFromDouble(modslope) << std::endl;
	
	//work out our quadrant
    bool done = false;
	if (modangle>=-45 && modangle<=45){
		//we're on the x axis going right
		//find the slope to target
		double slope = modslope;
		int i=0;
		Position checkPosition;
		do{
			i++;
			//calc true y position;
			double j = i*slope;
			//convert to rough tile y position
			int ty = (int)(j + (j < 0 ? -0.5 : 0.5))+y1;
			if (ty>=MAPHEIGHT || ty<0) break;
			int tx = i+x1;
			if (tx>=MAPWIDTH) break;
			//std::cout << "tx=" << tx << " ty=" << tool.stringFromInt(ty) << " from " << tool.stringFromDouble(j) << std::endl;
			checkPosition.x=tx;
			checkPosition.y=ty;
			if (ty<MAPHEIGHT && ty>=0){
				WORLD.anim.coords.push_back(checkPosition);
				WORLD.anim.gfxIds.push_back(BULLET_S);
			}
            if (WORLD.isTileBlocked(checkPosition) || WORLD.haveHitHalfHeightTile(checkPosition)) done = true;
			//std::cout << checkPosition.string() << " blocked = " << WORLD.isTileBlocked(checkPosition) << std::endl;
		} while (!done);
	}
	if (modangle>=135 || modangle<=-135){
		//we're on x axise going to the left
		//find the slope to target
		double slope = modslope;
		int i=0;
		Position checkPosition;
		do{
			i--;
			//calc true y position;
			double j = i*slope;
			//convert to rough tile y position
			int ty = (int)(j + (j < 0 ? -0.5 : 0.5))+y1;
			if (ty>=MAPHEIGHT || ty<0) break;
			int tx = x1+i;
			if (tx>=MAPWIDTH || tx<0) break;
			//std::cout << "tx=" << tx << " ty=" << tool.stringFromInt(ty) << " from " << tool.stringFromDouble(j) << std::endl;
			checkPosition.x=tx;
			checkPosition.y=ty;
			if (ty<MAPHEIGHT && ty>=0){
				WORLD.anim.coords.push_back(checkPosition);
				WORLD.anim.gfxIds.push_back(BULLET_S);
			}
            if (WORLD.isTileBlocked(checkPosition) || WORLD.haveHitHalfHeightTile(checkPosition)) done = true;
			//std::cout << checkPosition.string() << " blocked = " << WORLD.isTileBlocked(checkPosition) << std::endl;
		} while (!done);
	}
	if (modangle>45 && modangle<135){
		//we're on the y axis going down
		//WORLD.projectile.setVertical();
		//find the slope to target
		double slope = 1/modslope;
		int j=0;
		Position checkPosition;
		do{
			j++;
			//calc true x position;
			double i = j*slope;
			//convert to rough tile x position
			int tx = (int)(i + (i < 0 ? -0.5 : 0.5))+x1;
			if (tx>=MAPWIDTH || tx<0) break;
			int ty = j+y1;
			if (ty>=MAPHEIGHT) break;
			//std::cout << "tx=" << tool.stringFromInt(tx) << " ty=" << ty << " from " << tool.stringFromDouble(i) << std::endl;
			checkPosition.y=ty;
			checkPosition.x=tx;
			if (tx<MAPWIDTH && tx>=0){
				WORLD.anim.coords.push_back(checkPosition);
				WORLD.anim.gfxIds.push_back(BULLET_S);
			}
            if (WORLD.isTileBlocked(checkPosition) || WORLD.haveHitHalfHeightTile(checkPosition)) done = true;
			//std::cout << checkPosition.string() << " blocked = " << WORLD.isTileBlocked(checkPosition) << std::endl;
		} while (!done);
	}
	if (modangle<-45 && modangle>-135){
		//we're going up
		//WORLD.projectile.setVertical();
		//find the slope to target
		double slope = 1/modslope;
		int j=0;
		Position checkPosition;
		do{
			j--;
			//calc true x position;
			double i = j*slope;
			//convert to rough tile x position
			int tx = (int)(i + (i < 0 ? -0.5 : 0.5))+x1;
			if (tx>=MAPWIDTH || tx<0) break;
			int ty = y1+j;
			if (ty<0) break;
			//std::cout << "tx=" << tool.stringFromInt(tx) << " ty=" << ty << " from " << tool.stringFromDouble(i) << std::endl;
			checkPosition.y=ty;
			checkPosition.x=tx;
			if (tx<MAPWIDTH && tx>=0){
				WORLD.anim.coords.push_back(checkPosition);
				WORLD.anim.gfxIds.push_back(BULLET_S);
			}
            if (WORLD.isTileBlocked(checkPosition) || WORLD.haveHitHalfHeightTile(checkPosition)) done = true;
			//std::cout << checkPosition.string() << " blocked = " << WORLD.isTileBlocked(checkPosition) << std::endl;
		} while (!done);
		
	}
	
	//Return the last tile position
	return WORLD.anim.coords.back();
}

void GameWorld::toggleCreatureToMap(Creature *creature)
{
	//fetch list of creatures onMap already
	std::vector<Creature*> mapCreatures = WORLD.getCreatures(FRIEND, true);
	std::cout << "Creatures onMap= " << mapCreatures.size() << std::endl;
	
	if (creature->onMap){
		creature->onMap = false;
	} else {
		if (mapCreatures.size()<WORLD.map.parameterMap["squadsize"]){
			creature->onMap = true;
		}
	}
}


