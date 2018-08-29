/*
 *  UIEmbarkScreen.h
 *  tractionedge
 *
 *  Created by Steven Hamilton on 27/03/10.
 *  Copyright 2010 scorch.org. All rights reserved.
 *
 */

#ifndef __UIEMBARKSCREEN_H__
#define __UIEMBARKSCREEN_H__

#include "UIView.h"
#include "Utility.h"
#include "GameWorld.h"

enum embarkScreenState_t{
    SELECTMAN,
	BASEINVENTORY,
    MANINVENTORY,
    SELECTMANSKILL
};

class Creature;

class UIEmbarkScreen : public UIView {
private:
	Utility	tool;
	
	//standard UI data
    std::vector<std::string> strings;
	std::vector<Position> positions;
	std::vector<int> colors;
	std::vector<int> sizes;
	std::vector<fontStyle_t> styles;
	std::vector<font_t> fonts;
	
	int defaultColor;
	font_t defaultFont;
	fontStyle_t defaultStyle;
	int defaultSize;
    
    void clearArrays();
	void addToArrays(std::string, Position, int, font_t, fontStyle_t, int);
    void drawBaseSquad();
	void drawCreatureDetail();
    void drawInventory();
    void drawItemDetail();
    void drawTooltip();
	void makeItemStacks();
	
    //Game Data specific
    std::vector<Creature*> squad;
	std::vector<Creature*>::iterator creatureIt;
	Creature * selectedCreature;

    std::vector<Item*> items;
	std::vector<Item*>::iterator itemIt;
	std::map<std::string,int> itemStacks;
	std::map<std::string,int>::iterator selectedStackIt;
	std::vector<Item*> manItems;
	std::vector<Item*>::iterator manItemIt;
	Item * selectedItem;
	std::map<std::string, int>::iterator selectedSkillIt;

    embarkScreenState_t state;
public:
	UIEmbarkScreen();
	~UIEmbarkScreen();
	
	virtual void draw();
    virtual event_t process(event_t);
};

#endif
