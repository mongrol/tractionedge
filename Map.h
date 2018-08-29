/*
 *  Map.h
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 20/11/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#ifndef __MAP_H__
#define __MAP_H__

#include <map>
#include <vector>
#include <string>
#include "Creature.h"
#include "Map.h"
#include "Position.h"
#include "Terrain.h"
#include "defines.h"
#include "Item.h"

class Map{
private:
	std::map<std::string, creature_t> creatureSchema;
public:
	Map();
	~Map();
	
    int objectId;
	std::vector<std::vector<Terrain> > map;
    
    //parameters
	std::map<std::string, int> parameterMap;
    std::string introText;
    std::string resultText;
	
	int startx1;
	int starty1;
	int	startx2;
	int	starty2;

    //functions
	void loadMap(int);
    void loadRandomTerrain();
    void loadMapTerrain(int);
    void replaceMapTerrain(std::vector<int>&);
    void loadMapParameters(int);
    bool isTileTranslucent(int,int);
	bool isTerrainPassable(Position);
	void setTilePassable(Position, bool);
	bool isTileVisible(Position);
	bool setTileVisibility(Position, bool);
	int getMoveCost(Position);
    int getHeight(Position);

	//actions
	int damage(Position, int);
	void destroyTerrain(Position);
	
	//queries
	std::string terrainNameAt(Position);
};

#endif
