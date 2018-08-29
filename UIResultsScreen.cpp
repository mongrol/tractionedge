/*
 *  UIResultsScreen.cpp
 *  tractionedge
 *
 *  Created by Steven Hamilton on 10/05/10.
 *  Copyright 2010 scorch.org. All rights reserved.
 *
 */

#include "DisplayManager.h"
#include "GameWorld.h"
#include "UIResultsScreen.h"
#include "Utility.h"

UIResultsScreen::UIResultsScreen()
{
	defaultColor=COLORLIGHTGREY;
	defaultFont=BODYSMALL;
	defaultStyle=REGULAR;
	defaultSize=FONTSIZESMALL;	
}

UIResultsScreen::~UIResultsScreen(){}

event_t UIResultsScreen::process(event_t event)
{
    switch(event){
        case KEY_RETURN:
            event = EVENT_CONTINUE;
            std::cout << "Results processing events" << std::endl;
            break;
        default:
            break;
    }
    return event;
}

void UIResultsScreen::draw()
{
	clearArrays();

    Utility tool;

	//std::string str=WORLD.map.resultText;
    //std::cout << "RESULT TEXT" << str << std::endl;
    //convert \'s to newlines
    //std::string::size_type loc = str.find("\\",0);
    //while(loc!=std::string::npos)
    //{
      //  std::cout << "found \\ at " << loc << std::endl;
        //str.replace(loc,1,"\n");
        //loc = str.find("\\",0);
     //   }
    
    std::string str="Congratulations Detective. You've cleared the countryside\nof a deadly scourge!\n \nLet's move on";

    //std::string str=tool.stringFromInt(WORLD.getScore());

	Position pos(80,80);
	addToArrays(str, pos, defaultColor, defaultFont, defaultStyle, defaultSize);
	
	//send it off to draw
    DISPLAY.gfxEngine.renderStrings(strings, colors, fonts, styles, sizes, positions);
}

void UIResultsScreen::clearArrays()
{
	strings.resize(0);
	colors.resize(0);
	positions.resize(0);
	styles.resize(0);
	sizes.resize(0);
	fonts.resize(0);
}

void UIResultsScreen::addToArrays(std::string str, Position pos, int col, font_t fnt, fontStyle_t style, int size)
{
	strings.push_back(str);
	positions.push_back(pos);
    colors.push_back(col);
    fonts.push_back(fnt);
    styles.push_back(style);
	sizes.push_back(size);
}
