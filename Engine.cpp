/*
 *  Engine.cpp
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 24/06/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include <SFML/Graphics.hpp>
#include <assert.h>
#include <iostream>
#include "Engine.h"
#include "EngineState.h"
#include "DisplayManager.h"
#include "Utility.h"

#include "Map.h"

void Engine::updateState()
{
	if (currentState) {
		currentState->execute(this);
	}
}

void Engine::changeState(EngineState* newState)
{
	//make sure both states are valid before attempting to call their methods
	assert (currentState && newState);
	//call the exit method of the existing state
	currentState->exit(this);
	//change state to the new state
	currentState = newState;
	//call the entry method of the new state
	currentState->enter(this);
}

void Engine::changeState(EngineState* newState, bool savePrevious)
{
    assert (currentState && newState);
    previousState = currentState;
    currentState = newState;
    //we have to save the Display's previous screen.
    if (savePrevious) DISPLAY.saveScreen();
    currentState->enter(this);
}

void Engine::restorePreviousState()
{
    currentState->exit(this);
    currentState = previousState;
    DISPLAY.restoreScreen();
}

screenType_t Engine::previousStateScreenType()
{
    return previousState->screenType;
}

int Engine::run()
{
	setup();
	
	//loop
	while (DISPLAY.isOpen())
	 {
		currentState->execute(this);
		sf::Sleep(FRAMERATE);
	 }
	return 0;
}	

void Engine::setup()
{	
	//init randomiser
	srand (time(NULL));
	
	//set initial game state
	currentState = new EngineStateTitle();
	currentState->enter(this);
}
