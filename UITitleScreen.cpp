/*
 *  UITitleScreen.cpp
 *  tractionedge
 *
 *  Created by Steven Hamilton on 25/03/10.
 *  Copyright 2010 scorch.org. All rights reserved.
 *
 */

#include "DisplayManager.h"
#include "UITitleScreen.h"

UITitleScreen::UITitleScreen()
{
}

UITitleScreen::~UITitleScreen(){}

event_t UITitleScreen::process(event_t event)
{
    switch(event){
        case KEY_N:
            event = EVENT_NEW_GAME;
            break;
        case KEY_C:
            event = EVENT_CONTINUE_GAME;
            break;
        case KEY_ESCAPE:
            event = EVENT_OPTIONS;
            break;
        default:
            break;
    }
    return event;
}

void UITitleScreen::draw()
{
	clearArrays();
	
	//replace this with generic UI gfx element.
	DISPLAY.gfxEngine.drawTitleGfx();
	
	std::string str="(N)ew Game";
	Position pos(320,300);
	int color=COLORLIGHTGREY;
	font_t font=BODYMEDIUM;
	fontStyle_t style=REGULAR;
	int size=FONTSIZEMEDIUM;
	addToArrays(str, pos, color, font, style, size);

    if (saver.doesSaveFileExist()){
	    str="(C)ontinue" ;
	    color=COLORLIGHTGREY;
	    pos.setXY(320, 350);
	    addToArrays(str, pos, color, font, style, size);
    }

	str="Copyright 2010 Steven Hamilton";
	color=COLORLIGHTGREY;
	size=FONTSIZESMALL;
	pos.setXY(250, 550);
	addToArrays(str, pos, color, font, style, size);
	
	//send it off to draw
    DISPLAY.gfxEngine.renderStrings(strings, colors, fonts, styles, sizes, positions);
}

void UITitleScreen::clearArrays()
{
	strings.resize(0);
	colors.resize(0);
	positions.resize(0);
	styles.resize(0);
	sizes.resize(0);
	fonts.resize(0);
}

void UITitleScreen::addToArrays(std::string str, Position pos, int col, font_t fnt, fontStyle_t style, int size)
{
	strings.push_back(str);
	positions.push_back(pos);
    colors.push_back(col);
    fonts.push_back(fnt);
    styles.push_back(style);
	sizes.push_back(size);
}
