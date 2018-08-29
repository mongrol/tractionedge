/*
 *  UIGameOverScreen.cpp
 *  tractionedge
 *
 *  Created by Steven Hamilton on 8/09/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#include "DisplayManager.h"
#include "GameWorld.h"
#include "UIGameOverScreen.h"

UIGameOverScreen::UIGameOverScreen()
{
	defaultColor=COLORLIGHTGREY;
	defaultFont=BODYSMALL;
	defaultStyle=REGULAR;
	defaultSize=FONTSIZESMALL;	
}

UIGameOverScreen::~UIGameOverScreen(){}

event_t UIGameOverScreen::process(event_t event)
{
    switch(event){
        case KEY_RETURN:
            event = EVENT_CONTINUE;
            break;
        default:
            break;
    }
    return event;
}

void UIGameOverScreen::draw()
{
	clearArrays();
	
	std::string str="Game Over. Thank you for playing Traction Edge.";
    	
	Position pos(80,80);
	addToArrays(str, pos, defaultColor, defaultFont, defaultStyle, defaultSize);
	
	//send it off to draw
    DISPLAY.gfxEngine.renderStrings(strings, colors, fonts, styles, sizes, positions);
}

void UIGameOverScreen::clearArrays()
{
	strings.resize(0);
	colors.resize(0);
	positions.resize(0);
	styles.resize(0);
	sizes.resize(0);
	fonts.resize(0);
}

void UIGameOverScreen::addToArrays(std::string str, Position pos, int col, font_t fnt, fontStyle_t style, int size)
{
	strings.push_back(str);
	positions.push_back(pos);
    colors.push_back(col);
    fonts.push_back(fnt);
    styles.push_back(style);
	sizes.push_back(size);
}
