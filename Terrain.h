/*
 *  Terrain.h
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 5/08/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <string>
#include "defines.h"

class Terrain{
public:
	Terrain();
	~Terrain();
	Terrain(int);

	gfxId_t gfxId;

	//attributes
	std::string name;
	int moveCost;
	int height;
	int	totalHitPoints;
	int currentHitPoints;
	gfxId_t deadTerrain;
	
	bool isPassable;
	bool isTranslucent;
	bool isVisible;
    bool wasVisible;

	//actions
	void setDead();
};

#endif
