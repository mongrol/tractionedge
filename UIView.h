/*
 *  UI_View.h
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 20/12/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#ifndef __UIVIEW_H__
#define __UIVIEW_H__

#include "def_events.h"
#include <vector>

class UIView {
private:

public:
	//draw coords are x,y in constructors.
	int y;
	int x;
	
	//attributes
	bool selectable;
	bool selected;
	
	//if true triggers a redraw
	bool stateChanged;
	
	//action to send on click
	event_t action;
	
	//functions
    virtual event_t process(event_t){};
	virtual void draw(){};
	virtual void setSelected(bool){};
};

#endif
