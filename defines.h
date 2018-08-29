/*
 *  defines.h
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 6/07/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */
#ifndef __DEFINES_H__
#define __DEFINES_H__

enum screenType_t {
	SCREEN_TITLE,
	SCREEN_EMBARK,
	SCREEN_INTRO,
    SCREEN_MATCH,
	SCREEN_RESULT,
	SCREEN_HELP,
	SCREEN_GAMEOVER,
    SCREEN_OPTIONS
};

//creature
enum move_t {
	MOVE_RIGHT=6,
	MOVE_LEFT=4,
	MOVE_UP=8,
	MOVE_DOWN=2,
	MOVE_UP_RIGHT=9,
	MOVE_UP_LEFT=7,
	MOVE_DOWN_RIGHT=3,
	MOVE_DOWN_LEFT=1,
    NO_MOVE=5
};	

enum firingMode_t {
    NONE,
	AIMED,
	SNAPSHOT,
};

enum faction_t {
	FRIEND,
	ENEMY,
	NEUTRAL
};

enum creature_t {
	HUMAN,
    SLITHER,
	GIMP,
    SPIDER,
};

enum parameter_t {
	SQUADSIZE
};

enum gfxId_t{
    GRASS_S,
	DIRT_S,
	STONE_WALL_S,
	COBBLES_S,
	WATER_S,
	RUBBLE_S,
	FENCE_S,
	TREE_S,
	DOOR_S,
	WINDOW_S,
	PLAYER_S,
	SLITHER_S,
	GIMP_S,
	SPIDER_S,
	WEAPON_S,
	CORPSE_S,
	BULLET_S,
	TARGET_S,
	SELECTED_S,
    LOWWALL_S,
    STATUE_S,
    BLANK_S,
    WELL_S
};


//world
int const NUMBEROFMONSTERS = 4;
int const NUMBEROFPLAYERS = 3;
int const NUMBEROFBASEPLAYERS = 8;



//calcs
double const PI=3.14159265;


//macros
#define DISPLAY DisplayManager::instance()
#define WORLD GameWorld::instance()
#define MSG WORLD.sendMessageToConsole
#define PROMPT WORLD.sendMessageToPrompt

#endif
