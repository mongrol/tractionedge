/*
 *  Map.cpp
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 20/11/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#include <fstream>
#include <iostream>
#include <string>
#include "defines.h"
#include "Map.h"
#include "MapGenerator.h"
#include "tinyxml.h"
#include "Utility.h"

#define TIXML_USE_STL

Map::Map()
{
	creatureSchema["human"]=HUMAN;
    creatureSchema["slither"]=SLITHER;
	creatureSchema["gimp"]=GIMP;
	parameterMap["squadsize"]=2;
	parameterMap["visibility"]=8;
}

Map::~Map()
{
}

void Map::loadMap(int mapNumber)
{
	Utility tool;

	objectId=WORLD.getObjectId();
	//std::cout << "Map objectId = " << objectId << std::endl;
	
    //initialise map
    //loadMapTerrain(mapNumber);
    loadRandomTerrain();
    
    //set fixed start zone. Move this into .tmx properties!
	startx1=1;
	starty1=5;
	startx2=4;
	starty2=20;
}

void Map::loadMapParameters(int mapNumber)
{
    //YES I know this is repeated above!
    //load map parameter file
    Utility tool;
    std::string paramURL = "maps/map"+tool.stringFromInt(mapNumber)+".xml";
	TiXmlDocument paramFile(paramURL.c_str());
	bool loadOkay = paramFile.LoadFile();
	if (loadOkay)
	{
		std::cout << "Map.cpp:loaded " << paramURL << std::endl; 
	}
	else
	{
		std::cout << "Map.cpp:failed to load " << paramURL << std::endl; 
        exit(0);
	}

	//set paramaters
	TiXmlHandle paramHandle(&paramFile);
    for(TiXmlElement * itemNode=paramHandle.FirstChild("parameters").FirstChild().ToElement(); itemNode; itemNode=itemNode->NextSiblingElement() )
	{
		parameterMap[itemNode->Value()]=tool.stringToInt(itemNode->GetText());
		//std::cout << "Setting " << itemNode->Value() << " to " << parameterMap[itemNode->Value()] << std::endl;
	}
    
    //process texts
	TiXmlElement * introTextNode=paramHandle.FirstChild("introtext").ToElement();
	introText.append(introTextNode->GetText());
	TiXmlElement * resultTextNode=paramHandle.FirstChild("resulttext").ToElement();
    resultText.append(resultTextNode->GetText());
}

void Map::loadRandomTerrain()
{
    MapGenerator mapGen;
    map = mapGen.getMap(LANDSCAPE_FARM);
}

void Map::loadMapTerrain(int level)
{
	Utility tool;
    
    map.clear();
    map.resize(MAPHEIGHT);
	for (int i=0; i < MAPHEIGHT; i++)
		map[i].resize(MAPWIDTH);
    //std::cout << "BING" << std::endl;	
    //load map tmx file
	std::string mapURL = "maps/map"+tool.stringFromInt(level)+".tmx";
	TiXmlDocument mapFile(mapURL.c_str());
	bool loadOkay = mapFile.LoadFile();
	if (loadOkay)
	{
		std::cout << "loaded " << mapURL << std::endl; // defined later in the tutorial
	}
	else
	{
		std::cout << "failed to load " << mapURL << std::endl;
		exit(0);
	}
	
	TiXmlHandle mapHandle(&mapFile);
	
	TiXmlElement * tileElement=mapHandle.FirstChild("map").FirstChild("layer").FirstChild("data").FirstChild("tile").ToElement();
	
	for (int y=0;y < map.size(); y++)
		for (int x=0;x < MAPWIDTH; x++){
			if (tileElement){
				//std::cout << "gid id=" << tileElement->Attribute("gid") << std::endl;
				int terrainId=tool.stringToInt(tileElement->Attribute("gid"));
				Terrain terrain(terrainId-1);
                terrain.isVisible=false;
				map[y][x] = terrain;
	//			std::cout << map[y][x].gfxId;
				
				tileElement=tileElement->NextSiblingElement();
			}
		}
	}

void Map::replaceMapTerrain(std::vector<int> &gids)
{
	//replaces map terrains. Usually used by Saver.cpp for loading
    std::vector<int>::iterator it;
    it=gids.begin();
    for (int y=0;y < map.size(); y++){
		for (int x=0;x < MAPWIDTH; x++){
			//std::cout << "gid id=" << tileElement->Attribute("gid") << std::endl;
			Terrain terrain((*it));
            terrain.isVisible=false;
			map[y][x] = terrain;
            it++;
			//std::cout << map[y][x].gfxId;
		}
	}
}

bool Map::isTileTranslucent(int x, int y)
{
	//if tile is height 1 or less then we can see over it.
	if (map[y][x].isTranslucent) return true;
	else return false;
}

bool Map::isTileVisible(Position position)
{
	if (map[position.y][position.x].isVisible) return true;
	else return false;
}

bool Map::setTileVisibility(Position position, bool flag)
{
	map[position.y][position.x].isVisible = flag;
    if (flag) map[position.y][position.x].wasVisible = flag;
}

bool Map::isTerrainPassable(Position position)
{
	if (map[position.y][position.x].height<=1)	return true;
	return false;
}

void Map::setTilePassable(Position position, bool flag)
{
	map[position.y][position.x].isPassable = flag;
}

int Map::getMoveCost(Position position)
{
	return map[position.y][position.x].moveCost;
}

int Map::getHeight(Position position)
{
    return map[position.y][position.x].height;
}

int Map::damage(Position position,  int damage)
{
    int resultantDamage = damage;
	Terrain* tile=&map[position.y][position.x];
	if (tile->height>0) {
	tile->currentHitPoints-=damage;
		if (tile->currentHitPoints<=0) destroyTerrain(position);
	}
    //std::cout << "Tile damage: " << damage << std::endl;
    return resultantDamage;
}

void Map::destroyTerrain(Position position)
{
	Utility tool;
    Terrain & tile=map[position.y][position.x];
	Terrain deadTerrain(tile.deadTerrain);
    //std::cout << "Replacing tile " << tile.name << " with " << tool.stringFromInt(tile.deadTerrain) << std::endl;
	map[position.y][position.x] = deadTerrain;
}

std::string Map::terrainNameAt(Position position)
{
	return map[position.y][position.x].name;
}
