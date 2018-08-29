/*
 *  DisplayManager.cpp
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 5/07/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include "Logger.h"
#include "DisplayManager.h"
#include "GameWorld.h"
#include "GfxSFML.h"
#include "UITitleScreen.h"
#include "UIEmbarkScreen.h"
#include "UIGameOverScreen.h"
#include "UIIntroScreen.h"
#include "UIMatchScreen.h"
#include "UIOptionsScreen.h"
#include "UIResultsScreen.h"
#include "Utility.h"

DisplayManager::DisplayManager(){}

DisplayManager::~DisplayManager(){}

//-----------------
//Generic Screens
//-----------------

void DisplayManager::setupScreen(screenType_t screenType)
{
	switch (screenType) {
		case SCREEN_EMBARK:{
			screenController = new UIEmbarkScreen();
			break;
        }
		case SCREEN_INTRO:{
			screenController = new UIIntroScreen();
			break;
        }
		case SCREEN_RESULT:{
			screenController = new UIResultsScreen();
			break;
        }
		case SCREEN_TITLE:{
			screenController = new UITitleScreen();
			break;
        }
        case SCREEN_MATCH:{
            screenController = new UIMatchScreen();
            break;
        }
		case SCREEN_GAMEOVER:{
            screenController = new UIGameOverScreen();
            break;
        }
        case SCREEN_OPTIONS:{
            screenController = new UIOptionsScreen();
            break;
        }
		default:
			break;
	}
	//clear the screen
	gfxEngine.clearScreen();
}

void DisplayManager::saveScreen()
{
    previousScreen = screenController;
}

void DisplayManager::restoreScreen()
{
    screenController = previousScreen;
}

void DisplayManager::drawScreen()
{
	gfxEngine.clearScreen();
    screenController->draw();
	
	//render the screen
	gfxEngine.renderScreen();
}

void DisplayManager::animate()
{
    gfxEngine.animateProjectile();
}

void DisplayManager::deallocScreenController()
{
	//deallocate screen objects.
	delete screenController;
}

void DisplayManager::closeWindow()
{
    DBG << "Calling close window";
    gfxEngine.closeWindow();
}

bool DisplayManager::isOpen()
{
	if (gfxEngine.isOpen()) return true;
	else return false;
}

event_t DisplayManager::getUIEvent()
{
    //get keypresses
	event_t event = gfxEngine.event();
    //send to processing and receive any changes.
    event = screenController->process(event);
	return event;
}

event_t DisplayManager::promptForEvent(std::string str)
{
    MSG(str);
    drawScreen();
    event_t event=gfxEngine.event();
    while (event==EVENT_NULL){
        sf::Sleep(FRAMERATE);
        //std::cout << "Waiting for event..." << std::endl;
        event=gfxEngine.event();
    }
    return (event);
}
