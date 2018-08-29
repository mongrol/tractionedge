/*
 *  Terrain.cpp
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 5/08/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#include "defines.h"
#include "Terrain.h"
#include <iostream>

Terrain::Terrain()
{
}

Terrain::~Terrain()
{
}

Terrain::Terrain(int terrainType)
{
	wasVisible=false;
	isVisible=false;
	
	switch (terrainType) {
		case BLANK_S:
			name="BLANK";
			moveCost=0;
			height=0;
			totalHitPoints=10;
			currentHitPoints=totalHitPoints;
			gfxId=BLANK_S;
			isPassable=true;
			isTranslucent=true;
			deadTerrain=BLANK_S;
			break;
		case GRASS_S:
			name="Grass";
			moveCost=1;
			height=0;
			totalHitPoints=10;
			currentHitPoints=totalHitPoints;
			gfxId=GRASS_S;
			isPassable=true;
			isTranslucent=true;
			deadTerrain=GRASS_S;
			break;
		case STONE_WALL_S:
			name="Stone Wall";
			moveCost=0;
			height=2;
			totalHitPoints=50;
			currentHitPoints=totalHitPoints;
			gfxId=STONE_WALL_S;
			isPassable=false;
			isTranslucent=false;
			deadTerrain=RUBBLE_S;
			break;
		case RUBBLE_S:
			moveCost=3;
			name="Rubble";
			height=1;
			totalHitPoints=10;
			currentHitPoints=totalHitPoints;
			gfxId=RUBBLE_S;
			isPassable=true;
			isTranslucent=true;
			deadTerrain=COBBLES_S;
			break;
		case WATER_S:
			name="Water";
			moveCost=5;
			height=0;
			totalHitPoints=0;
			currentHitPoints=totalHitPoints;
			gfxId=WATER_S;
			isPassable=true;
			isTranslucent=true;
			deadTerrain=WATER_S;
			break;
		case COBBLES_S:
			name="Cobbles";
			moveCost=1;
			height=0;
			totalHitPoints=0;
			currentHitPoints=totalHitPoints;
			gfxId=COBBLES_S;
			isPassable=true;
			isTranslucent=true;
			deadTerrain=COBBLES_S;
			break;
		case DIRT_S:
			name="Dirt";
			moveCost=1;
			height=0;
			totalHitPoints=10;
			currentHitPoints=totalHitPoints;
			gfxId=DIRT_S;
			isPassable=true;
			isTranslucent=true;
			deadTerrain=DIRT_S;
			break;
		case FENCE_S:
			name="Fence";
			moveCost=10;
			height=1;
			totalHitPoints=5;
			currentHitPoints=totalHitPoints;
			gfxId=FENCE_S;
			isPassable=true;
			isTranslucent=true;
			deadTerrain=GRASS_S;
			break;
		case TREE_S:
			name="Tree";
			moveCost=0;
			height=2;
			totalHitPoints=30;
			currentHitPoints=totalHitPoints;
			gfxId=TREE_S;
			isPassable=false;
			isTranslucent=false;
			deadTerrain=GRASS_S;
			break;
		case DOOR_S:
			name="Door";
			moveCost=20;
			height=2;
			totalHitPoints=15;
			currentHitPoints=totalHitPoints;
			gfxId=DOOR_S;
			isPassable=false;
			isTranslucent=false;
			deadTerrain=COBBLES_S;
			break;
		case WINDOW_S:
			name="Window";
			moveCost=20;
			height=2;
			totalHitPoints=5;
			currentHitPoints=totalHitPoints;
			gfxId=WINDOW_S;
			isPassable=false;
			isTranslucent=true;
			deadTerrain=RUBBLE_S;
			break;
		case LOWWALL_S:
			name="Low Wall";
			moveCost=6;
			height=1;
			totalHitPoints=10;
			currentHitPoints=totalHitPoints;
			gfxId=LOWWALL_S;
			isPassable=true;
			isTranslucent=true;
			deadTerrain=RUBBLE_S;
			break;
		case STATUE_S:
			name="Statue";
			moveCost=20;
			height=2;
			totalHitPoints=5;
			currentHitPoints=totalHitPoints;
			gfxId=STATUE_S;
			isPassable=false;
			isTranslucent=false;
			deadTerrain=RUBBLE_S;
			break;
		case WELL_S:
			name="Well";
			moveCost=6;
			height=1;
			totalHitPoints=30;
			currentHitPoints=totalHitPoints;
			gfxId=WELL_S;
			isPassable=true;
			isTranslucent=true;
			deadTerrain=RUBBLE_S;
			break;
		default:
			std::cout << "Error loading map with :" << terrainType << std::endl;
			break;
	}
}
