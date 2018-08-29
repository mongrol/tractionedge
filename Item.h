/*
 *  Item.h
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 26/08/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#ifndef __ITEM_H__
#define __ITEM_H__

#include "Position.h"
#include "def_items.h"
#include "defines.h"
#include <string>
#include <map> 

class Item{

public:
	Item();
	~Item();
	Item(itemType_t, int ownerId);
	Item(std::map<std::string,std::string>&, Position, std::map<std::string,int>&);

	int objectId;
    int ownerObjectId;
    bool onMap;

	//attributes
	itemType_t type;
    itemAttackType_t itemAttackType;
    std::string name;
	Position position;
	gfxId_t gfxId;
	std::string skill;
	int weight;
    std::map<std::string,int> properties;
};

#endif
