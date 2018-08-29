/*
 *  Item.cpp
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 26/08/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */
#include <iostream>
#include "defines.h"
#include "GameWorld.h"
#include "Item.h"
#include "Logger.h"
#include "Utility.h"

Item::Item()
{
}

Item::~Item()
{
}

Item::Item(itemType_t item, int ownerId)
{
	objectId=WORLD.getObjectId();
    onMap = false;
	ownerObjectId=ownerId;
	switch (item) {
		case CORPSE:
       		type=item;
        	itemAttackType=NO_ATTACK_TYPE;
			gfxId=CORPSE_S;
			name="Corpse";
			weight=180;
			break;
		case PISTOL:
			type=item;
            itemAttackType=RANGED;
            name="Pistol";
			gfxId=WEAPON_S;
			skill="HandGuns";
			weight=40;
			properties["Damage"] = 6;
            properties["APCost"] = 15;
			properties["Range"] = 5;
			break;
		case MUSKET:
			type=item;
            itemAttackType=RANGED;
			name = "Musket";
			gfxId = WEAPON_S;
			skill = "LongGuns";
			weight = 100;
			properties["Damage"] = 14;
            properties["APCost"] = 20;
			properties["Range"] = 15;
			break;
		case MUSKETOON:
			type=item;
            itemAttackType=RANGED;
			name = "Muskatoon";
			gfxId = WEAPON_S;
			skill = "Carbines";
			weight = 70;
			properties["Damage"] = 12;
            properties["APCost"] = 15;
			properties["Range"] = 10;
			break;
        case CUTLASS:
            type=item;
            itemAttackType=MELEE;
            gfxId=WEAPON_S;
            name="Cutlass";
            skill = "ShortSwords";
            weight = 35;
			properties["Damage"] = 6;
            properties["APCost"] = 10;
            break;
        case BILLYCLUB:
            type=item;
            itemAttackType=MELEE;
            gfxId=WEAPON_S;
            name="Billy Club";
            skill = "Clubs";
            weight = 40;
			properties["Damage"] = 4;
            properties["APCost"] = 8;
            break;
        case BRASS_FANG:
            type=item;
            itemAttackType=MELEE;
            gfxId=WEAPON_S;
            name="Brass Fang";
            skill = "Knives";
            weight = 20;
			properties["Damage"] = 8;
            properties["APCost"] = 11;
            break;
        case WOOD_MANDIBLE:
            type=item;
            itemAttackType=MELEE;
            gfxId=WEAPON_S;
            name="Wood Mandible";
            skill = "Knives";
            weight = 20;
			properties["Damage"] = 4;
            properties["APCost"] = 10;
            break;
//		case ARMOUR_IRON:
//			gfxId=ARMOUR_S;
//			name="Iron suit";
//			properties["armourClass"]=4;
//			break;
//		case ARMOUR_LEATHER:
//			gfxId=ARMOUR_S;
//			name="Leather Jacket";
//			properties["armourClass"]=2;
//			break;
		default:
			break;
	}
	LOG << objectId,"Creating Item";
}

Item::Item(std::map<std::string,std::string> &attribs, Position pos, std::map<std::string, int> &propmap)
{
    Utility tool;
    objectId=tool.stringToInt(attribs["objectId"]);
    gfxId=(gfxId_t)tool.stringToInt(attribs["gfxId"]);
    ownerObjectId=tool.stringToInt(attribs["ownerObjectId"]);
    name=attribs["name"];
    type=(itemType_t)tool.stringToInt(attribs["type"]);
    itemAttackType=(itemAttackType_t)tool.stringToInt(attribs["itemAttackType"]);
    onMap=(bool)tool.stringToInt(attribs["onMap"]);
    skill=attribs["skill"];
    weight=tool.stringToInt(attribs["weight"]);

    position = pos;
    properties = propmap;
}
