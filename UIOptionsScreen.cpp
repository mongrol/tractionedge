/*
 *  UIOptionsScreen.cpp
 *  tractionedge
 *
 *  Created by Steven Hamilton on 15/05/10.
 *  Copyright 2010 scorch.org. All rights reserved.
 *
 */

#include "DisplayManager.h"
#include "GameWorld.h"
#include "UIOptionsScreen.h"
#include "Utility.h"

UIOptionsScreen::UIOptionsScreen()
{
	defaultColor=COLORLIGHTGREY;
	defaultFont=BODYSMALL;
	defaultStyle=REGULAR;
	defaultSize=FONTSIZESMALL;
    
    options.push_back("Resume");
	options.push_back("Save and Quit");
	options.push_back("Quit");
    optionsIt = options.begin();
    selectedOptionIt = optionsIt;
}

UIOptionsScreen::~UIOptionsScreen(){}

event_t UIOptionsScreen::process(event_t event)
{
    switch(event){
        case KEY_RETURN:
            if ((*selectedOptionIt)=="Resume") event = EVENT_CONTINUE;
            if ((*selectedOptionIt)=="Save and Quit") event = EVENT_SAVE_AND_QUIT;
            if ((*selectedOptionIt)=="Quit") event = EVENT_QUIT;
            break;
        case KEY_DOWN:
            event = EVENT_REDRAW;
            if (selectedOptionIt==--options.end())break;
            selectedOptionIt++;
            break;
        case KEY_UP:
            event = EVENT_REDRAW;
            if (selectedOptionIt==options.begin())break;
            selectedOptionIt--;
            break;
        default:
            break;
    }
    return event;
}

void UIOptionsScreen::draw()
{
	clearArrays();

    Utility tool;
	std::string str;
    Position pos(80,80);
    std::cout << " drawing options" << std::endl;
    for (optionsIt=options.begin();optionsIt!=options.end();++optionsIt){
		str=(*optionsIt);
        fontStyle_t style;
		if (selectedOptionIt==optionsIt){
            style=BOLD;
        } else {
            style=defaultStyle;
        }
        addToArrays(str, pos, defaultColor, defaultFont, style, defaultSize);
		pos.modXY(0,FONTSIZESMALL*2);
	}
	//send it off to draw
    DISPLAY.gfxEngine.renderStrings(strings, colors, fonts, styles, sizes, positions);
}

void UIOptionsScreen::clearArrays()
{
	strings.resize(0);
	colors.resize(0);
	positions.resize(0);
	styles.resize(0);
	sizes.resize(0);
	fonts.resize(0);
}

void UIOptionsScreen::addToArrays(std::string str, Position pos, int col, font_t fnt, fontStyle_t style, int size)
{
	strings.push_back(str);
	positions.push_back(pos);
    colors.push_back(col);
    fonts.push_back(fnt);
    styles.push_back(style);
	sizes.push_back(size);
}
