/*
 *  UITitleScreen.h
 *  tractionedge
 *
 *  Created by Steven Hamilton on 25/03/10.
 *  Copyright 2010 scorch.org. All rights reserved.
 *
 */

#ifndef __UIMATCHSCREEN_H__
#define	__UIMATCHSCREEN_H__

#include    "UIView.h"
#include	"DisplayManager.h"
#include	"GameWorld.h"
#include	"Map.h"
#include	"Position.h"

enum matchState_t {
	STATE_TARGET,
	STATE_PLAYER,
	STATE_ANIMATE,
	STATE_AI,
	STATE_PROMPT_FIRINGMODE,
    STATE_PROMPT_MELEEATTACK,
    STATE_PROMPT_RESERVE,
    STATE_PROMPT_QUIT
};

class UIMatchScreen : public UIView {
private:
	std::vector<std::string> strings;
	std::vector<Position> positions;
	std::vector<int> colors;
	std::vector<int> sizes;
	std::vector<fontStyle_t> styles;
	std::vector<font_t> fonts;
    bool stateChanged;
	matchState_t matchState;
   
    void drawCharPanels();
    void drawConsole();
    void drawMap();
    void drawPrompt();
    void clearArrays();
	void addToArrays(std::string, Position, int, font_t, fontStyle_t, int);

public:
	UIMatchScreen();
	~UIMatchScreen();
	
	virtual void draw();
    virtual event_t process(event_t);
	
    //Map window
    std::vector<gfxId_t*> gfxIds;
	std::vector<Position*> positions_p;
};


#endif
