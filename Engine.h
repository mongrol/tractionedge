/*
 *  Engine.h
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 24/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "GameWorld.h"
#include "DisplayManager.h"

class EngineState;

class Engine {
private:
	EngineState* currentState;
    EngineState* previousState;

	void setup();	
	
public:
	void updateState();
	void changeState(EngineState* newState);
    void changeState(EngineState* newState, bool savePrevious);
    void restorePreviousState();
    screenType_t previousStateScreenType();
    int run();
	
	void initPlayerTurn();
	void endPlayerTurn();

};

#endif
