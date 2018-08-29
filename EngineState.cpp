/*
 *  EngineState.cpp
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 19/11/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#include "DisplayManager.h"
#include "Engine.h"
#include "EngineState.h"
#include "Map.h"
#include "Saver.h"

//-----------------------------------
//Options state - overlays previous state
//-----------------------------------
EngineStateOptions* EngineStateOptions::instance()
{
	static EngineStateOptions instance;
	return &instance;
}

void EngineStateOptions::enter(Engine* engine)
{
	std::cout << "Entering Options State" << std::endl;
    screenType=SCREEN_OPTIONS;
	DISPLAY.setupScreen(screenType);
	DISPLAY.drawScreen();
}

void EngineStateOptions::execute(Engine* engine)
{	
	event_t event = DISPLAY.getUIEvent();
	if (event == EVENT_CONTINUE){
		std::cout << "Restoring previous state" << std::endl;
		engine->restorePreviousState();
		DISPLAY.drawScreen();
        return;
	}
    if (event == EVENT_QUIT){
       DISPLAY.closeWindow();
    }
    if (event == EVENT_SAVE_AND_QUIT){
        Saver saver;
        saver.saveGame(engine);
        DISPLAY.closeWindow();
    }
	if (event==EVENT_REDRAW){
        DISPLAY.drawScreen();
    }
	if (event==EVENT_RESETUP){
	    DISPLAY.deallocScreenController();
        DISPLAY.setupScreen(SCREEN_OPTIONS);
        DISPLAY.drawScreen();
    }
}

void EngineStateOptions::exit(Engine*)
{
    DISPLAY.deallocScreenController();
    std::cout << "Exiting Options State" << std::endl;
}

//-----------------------------------
//match state
//-----------------------------------
EngineStateMatch* EngineStateMatch::instance()
{
	static EngineStateMatch instance;
	return &instance;
}

void EngineStateMatch::enter(Engine* engine)
{
	std::cout << "Entering MatchState" << std::endl;
    WORLD.selectFirstPlayer();
	WORLD.updateMapVisibility();
	WORLD.clearConsole();
	WORLD.squadAI.setInitialAIState();
    screenType=SCREEN_MATCH;
	DISPLAY.setupScreen(screenType);
	DISPLAY.drawScreen();
}

void EngineStateMatch::execute(Engine* engine)
{
	event_t event = DISPLAY.getUIEvent();
	if (event==EVENT_MATCH_WIN){
		event=DISPLAY.promptForEvent("I think we've got them all Sir! Let's return.");
		engine->changeState(EngineStateResult::instance());
	};
	if (event==EVENT_MATCH_LOST){
		event=DISPLAY.promptForEvent("All your constables have been slain.");
		engine->changeState(EngineStateGameOver::instance());
	};
	if (event!=EVENT_NULL){
		WORLD.updateMapVisibility();
		DISPLAY.drawScreen();
	}
    if (event == EVENT_OPTIONS){
        std::cout << "Displaying options" << std::endl;
        //create options state and save previous
        engine->changeState(EngineStateOptions::instance(),true);
    }
}

void EngineStateMatch::exit(Engine*)
{
    
    std::cout << "Exiting MatchState" << std::endl;
}

//-----------------------------------
//title state
//-----------------------------------
EngineStateTitle* EngineStateTitle::instance()
{
	static EngineStateTitle instance;
	return &instance;
}

void EngineStateTitle::enter(Engine*)
{
	std::cout << "Entering TitleState" << std::endl;
    screenType=SCREEN_TITLE;
	DISPLAY.setupScreen(screenType);
	DISPLAY.drawScreen();
}

void EngineStateTitle::execute(Engine* engine)
{
	event_t event = DISPLAY.getUIEvent();
	if (event == EVENT_NEW_GAME){
		std::cout << "Starting new game" << std::endl;
		WORLD.initWorld();
        WORLD.generateItems();
	    WORLD.level+=1;
		engine->changeState(EngineStateSquad::instance());
		return;
	}
	if (event == EVENT_CONTINUE_GAME){
		std::cout << "Loading game" << std::endl;
		WORLD.initWorld();
        Saver saver;
        screenType_t type=saver.loadGame(engine);
        switch(type){
        case SCREEN_TITLE:
		    engine->changeState(EngineStateTitle::instance());
            break;
        case SCREEN_EMBARK:
		    engine->changeState(EngineStateSquad::instance());
            break;
        case SCREEN_INTRO:
		    engine->changeState(EngineStateIntro::instance());
            break;
        case SCREEN_MATCH:
		    engine->changeState(EngineStateMatch::instance());
            break;
        case SCREEN_RESULT:
		    engine->changeState(EngineStateResult::instance());
            break;
        }
        return;
        //hmm, we now need to somehow change state to the one in the load file.
	}
    if (event == EVENT_OPTIONS){
        std::cout << "Displaying options" << std::endl;
        //create options state and save previous
        engine->changeState(EngineStateOptions::instance(),true);
    }
	if (event == EVENT_REDRAW) DISPLAY.drawScreen();
}

void EngineStateTitle::exit(Engine*)
{
	std::cout << "Exiting TitleState" << std::endl;
	DISPLAY.deallocScreenController();
}

//-----------------------------------
//intro state - display map intro
//-----------------------------------
EngineStateIntro* EngineStateIntro::instance()
{
	static EngineStateIntro instance;
	return &instance;
}

void EngineStateIntro::enter(Engine* engine)
{
	std::cout << "Entering Map Intro State" << std::endl;
	//WORLD.generateMap();
    screenType=SCREEN_INTRO;
	DISPLAY.setupScreen(screenType);
	DISPLAY.drawScreen();
}

void EngineStateIntro::execute(Engine* engine)
{	
	event_t event = DISPLAY.getUIEvent();
	if (event == EVENT_CONTINUE){
		std::cout << "Starting new Embarkation" << std::endl;
		engine->changeState(EngineStateSquad::instance());
		return;
	}
    if (event == EVENT_OPTIONS){
        std::cout << "Displaying options" << std::endl;
        //create options state and save previous
        engine->changeState(EngineStateOptions::instance(),true);
    }
	if (event==EVENT_REDRAW){
        DISPLAY.drawScreen();
    }
	if (event==EVENT_RESETUP){
	    DISPLAY.deallocScreenController();
        DISPLAY.setupScreen(SCREEN_RESULT);
        DISPLAY.drawScreen();
    }
}

void EngineStateIntro::exit(Engine*)
{
    DISPLAY.deallocScreenController();
    std::cout << "Exiting Intro State" << std::endl;
}

//-----------------------------------
//help state
//-----------------------------------
EngineStateHelp* EngineStateHelp::instance()
{
	static EngineStateHelp instance;
	return &instance;
}

void EngineStateHelp::enter(Engine* engine)
{
	std::cout << "Entering HelpState" << std::endl;
}

void EngineStateHelp::execute(Engine* engine)
{	
	event_t event = DISPLAY.getUIEvent();
	if (event == KEY_ESCAPE || event == KEY_SPACE || event == MODE_HELP) {
		engine->changeState(EngineStateMatch::instance());
	}
	//if (event==EVENT_REDRAW) DISPLAY.drawHelpScreen();
}

void EngineStateHelp::exit(Engine*)
{
	std::cout << "Exiting HelpState" << std::endl;
}

//-----------------------------------
//choose squad state
//here we make sure the available members are populated and present a UI to add them to the squad.
//first we need to load the map to get the allowed squad size
//-----------------------------------
EngineStateSquad* EngineStateSquad::instance()
{
	static EngineStateSquad instance;
	return &instance;
}

void EngineStateSquad::enter(Engine* engine)
{
	std::cout << "Entering EmbarkState" << std::endl;
	WORLD.generateMap();
    WORLD.regeneratePlayers();
    screenType=SCREEN_EMBARK;
	DISPLAY.setupScreen(screenType);
	DISPLAY.drawScreen();
}

void EngineStateSquad::execute(Engine* engine)
{	
	event_t event = DISPLAY.getUIEvent();
	if (event == EVENT_CONTINUE){
		std::cout << "Starting new match" << std::endl;
		engine->changeState(EngineStateMatch::instance());
		return;
	}
	if (event == EVENT_CANCEL){
		std::cout << "Cancelling" << std::endl;
		engine->changeState(EngineStateTitle::instance());
		return;
	}
	if (event==EVENT_REDRAW){
        DISPLAY.drawScreen();
    }
	if (event==EVENT_RESETUP){
	    DISPLAY.deallocScreenController();
        DISPLAY.setupScreen(SCREEN_EMBARK);
        DISPLAY.drawScreen();
    }
    if (event == EVENT_OPTIONS){
        std::cout << "Displaying options" << std::endl;
        //create options state and save previous
        engine->changeState(EngineStateOptions::instance(),true);
    }
}

void EngineStateSquad::exit(Engine*)
{
	WORLD.positionPlayers();
    DISPLAY.deallocScreenController();
    std::cout << "Exiting SquadState" << std::endl;
}


//-----------------------------------
//result state - display match results
//-----------------------------------
EngineStateResult* EngineStateResult::instance()
{
	static EngineStateResult instance;
	return &instance;
}

void EngineStateResult::enter(Engine* engine)
{
	std::cout << "Entering Match Result State" << std::endl;
    WORLD.updatePlayerLevel();
    screenType=SCREEN_RESULT;
    DISPLAY.setupScreen(screenType);
	DISPLAY.drawScreen();
}

void EngineStateResult::execute(Engine* engine)
{	
	event_t event = DISPLAY.getUIEvent();
	if (event == EVENT_CONTINUE){
        if (WORLD.level==WORLD.finalLevel){
            std::cout << "final level reached. invoking game over" << std::endl;
            engine->changeState(EngineStateGameOver::instance());
        } else {
		    std::cout << "Starting new level" << std::endl;
		    engine->changeState(EngineStateSquad::instance());
        }
        return;
	}
	if (event==EVENT_REDRAW){
        DISPLAY.drawScreen();
    }
	if (event==EVENT_RESETUP){
	    DISPLAY.deallocScreenController();
        DISPLAY.setupScreen(SCREEN_RESULT);
        DISPLAY.drawScreen();
    }
}

void EngineStateResult::exit(Engine*)
{
    DISPLAY.deallocScreenController();
    std::cout << "Exiting Result State" << std::endl;
	WORLD.level+=1;
}

//-----------------------------------
//Game Over state
//-----------------------------------
EngineStateGameOver* EngineStateGameOver::instance()
{
	static EngineStateGameOver instance;
	return &instance;
}

void EngineStateGameOver::enter(Engine* engine)
{
	std::cout << "Entering Game Over State" << std::endl;
    screenType=SCREEN_GAMEOVER;
	DISPLAY.setupScreen(screenType);
	DISPLAY.drawScreen();
}

void EngineStateGameOver::execute(Engine* engine)
{	
	event_t event = DISPLAY.getUIEvent();
	if (event == EVENT_CONTINUE){
		std::cout << "Returning to TitleScreen" << std::endl;
		engine->changeState(EngineStateTitle::instance());
		return;
	}
	if (event==EVENT_REDRAW){
        DISPLAY.drawScreen();
    }
	if (event==EVENT_RESETUP){
	    DISPLAY.deallocScreenController();
        DISPLAY.setupScreen(SCREEN_GAMEOVER);
        DISPLAY.drawScreen();
    }
}

void EngineStateGameOver::exit(Engine*)
{
    DISPLAY.deallocScreenController();
    std::cout << "Exiting Game Over State" << std::endl;
}

