/*
 *  DisplayManager.h
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 5/07/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */
// The Display Manager interfaces with the chosen gfx engine.
// Processes events
// Composites the various screen components (map, stats etc)
// Kicks off the render.

#ifndef __DISPLAYMANAGER_H__
#define __DISPLAYMANAGER_H__

#include "defines.h"
#include "def_display.h"
#include "GfxSFML.h"
#include <vector>
#include <string>

class GfxEngine;
class GameWorld;
class Creature;
class Position;
class UIView;


class DisplayManager{
private:
	DisplayManager(const DisplayManager&);
	DisplayManager& operator=(const DisplayManager&);
	DisplayManager();
public:
	~DisplayManager();
	
	static DisplayManager& instance(){
		static DisplayManager display;
		return display;
	}
	
	//draw window and UI functions
	GfxEngine gfxEngine;

	UIView * screenController;				
    UIView * previousScreen;
	void deallocScreenController();				//deallocates the current screens gadgets.
	
	void setupScreen(screenType_t);
    void saveScreen();
    void restoreScreen();
	void drawScreen();
	void drawHelpScreen();
	void animate();

	//misc window functions
	event_t getUIEvent();
    event_t promptForEvent(std::string);
    void closeWindow();
	bool isOpen();
};

#endif
