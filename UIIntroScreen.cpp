/*
 *  UIIntroScreen.cpp
 *  tractionedge
 *
 *  Created by Steven Hamilton on 15/05/10.
 *  Copyright 2010 scorch.org. All rights reserved.
 *
 */

#include "DisplayManager.h"
#include "GameWorld.h"
#include "UIIntroScreen.h"
#include "Utility.h"

UIIntroScreen::UIIntroScreen()
{
	defaultColor=COLORLIGHTGREY;
	defaultFont=BODYSMALL;
	defaultStyle=REGULAR;
	defaultSize=FONTSIZESMALL;	
}

UIIntroScreen::~UIIntroScreen(){}

event_t UIIntroScreen::process(event_t event)
{
    switch(event){
        case KEY_RETURN:
            event = EVENT_CONTINUE;
            std::cout << "Title processing events" << std::endl;
            break;
        case KEY_ESCAPE:
            event = EVENT_OPTIONS;
            break;
        default:
            break;
    }
    return event;
}

void UIIntroScreen::draw()
{
	clearArrays();

    Utility tool;
	std::string str=WORLD.map.introText;
    
    //convert \'s to newlines
    std::string::size_type loc = str.find("\\",0);
    while(loc!=std::string::npos)
    {
       //std::cout << "found \\ at " << loc << std::endl;
        str.replace(loc,1,"\n");
        loc = str.find("\\",0);
        }

    Position pos(80,80);
	addToArrays(str, pos, defaultColor, defaultFont, defaultStyle, defaultSize);
	
	//send it off to draw
    DISPLAY.gfxEngine.renderStrings(strings, colors, fonts, styles, sizes, positions);
}

void UIIntroScreen::clearArrays()
{
	strings.resize(0);
	colors.resize(0);
	positions.resize(0);
	styles.resize(0);
	sizes.resize(0);
	fonts.resize(0);
}

void UIIntroScreen::addToArrays(std::string str, Position pos, int col, font_t fnt, fontStyle_t style, int size)
{
	strings.push_back(str);
	positions.push_back(pos);
    colors.push_back(col);
    fonts.push_back(fnt);
    styles.push_back(style);
	sizes.push_back(size);
}
