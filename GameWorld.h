/*
 *  GameWorld.h
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 21/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __GAMEWORLD_H__
#define __GAMEWORLD_H__

#include "defines.h"
#include "Creature.h"
#include "DisplayManager.h"
#include "Map.h"
#include "Position.h"
#include "SquadAI.h"
#include "AnimManager.h"
#include <vector>

class ScreenTile;

class GameWorld {
private:
	GameWorld();
	GameWorld(const GameWorld&);
	GameWorld& operator=(const GameWorld&);
    
    void populateLevel();

    std::vector<Creature> creatures;
	std::vector<Creature>::iterator creatureIt;
	std::vector<Item> items;
	std::vector<Item>::iterator itemIt;
	std::map<int, creature_t > creatureXPMap;
    std::vector<std::vector<creature_t> > levelPopulations;

    int score;

    int getXPForLevel(int);

public:
	~GameWorld();
    int objectId;
    int objectIdIndex;
    
    void initWorld();

	static GameWorld& instance(){
		static GameWorld world;
		return world;
	}
	
    //console support
    std::vector<std::string> consoleBuffer;
	void sendMessageToConsole(std::string);
	void clearConsole();

    std::string promptString;
    void sendMessageToPrompt(std::string);
    void clearPrompt();

	//global game stats
	int level;
	int finalLevel;
    event_t checkMatchStatus(event_t);
    int getScore();

    //creature Big List management & queries
	void regeneratePlayers();
	void generateItems();
    int getObjectId();
    void addCreature(Creature&);
    void delCreature(Creature*);
    std::vector<Creature*> getCreatures();
    std::vector<Creature*> getCreatures(faction_t);
    std::vector<Creature*> getCreatures(faction_t,bool mapStatus);
    std::vector<Creature*> getCreatures(bool mapStatus);
    Creature & getCreature(Position position);
    void toggleCreatureToMap(Creature*);
	int getNumberOfCreatures(faction_t,bool mapStatus);

    //item Big List management
    Item * generateItem(itemType_t, int ownerId);
	void addItem(Item&);
	Item * getItemForObjectId(int objectId);
    std::vector<Item*> getItems();
    std::vector<Item*> getItems(int objectId);
    std::vector<Item*> getItems(itemType_t);
    std::vector<Item*> getItems(itemType_t,bool mapStatus);
    std::vector<Item*> getItems(bool mapStatus);
    void toggleItemToMap(Item*);
    int getNumberOfItems(bool mapStatus);

	// map functions
	Map map;
	void generateMap();
	void updateMapVisibility();
	std::string getVisibleElementNameAt(Position);
	bool isTileOccupied(Position);
	bool isTileBlocked(Position);
    bool haveHitHalfHeightTile(Position);

	// action functions
	void attack(Creature * creature, double angle);
    void attack(Creature * creature, move_t direction);
	int damageEntityAtPosition(Creature *, Position, Item&, itemAttackType_t);
    void checkReaction(faction_t faction);

	//action subroutines
	Position fireAtAngle(Position src ,double angle);
	
	// selection functions
	void initSelectionCursor();
	void endSelectionCursor();
	void moveSelectionCursor(int);
	void moveSelectionCursorTo(Position);
	Position target;
	gfxId_t selectionGfxId;
	void targetNextEnemy(Creature*);
            
	//player functions
	Creature * selectedPlayer;
	void positionPlayers();
	void selectFirstPlayer();
	void selectNextPlayer();
	void selectLivePlayer();
	void initPlayers();
	void resetPlayers();
    void updatePlayerLevel();

    //AI
	SquadAI squadAI;

	//animation - refactor this out somehow
	AnimManager	anim;

};



#endif
