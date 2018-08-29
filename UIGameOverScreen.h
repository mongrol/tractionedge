/*
 *  UIGameOverScreen.h
 *  tractionedge
 *
 *  Created by Steven Hamilton on 8/09/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#ifndef UIGAMEOVERSCREEN_H__
#define UIGAMEOVERSCREEN_H__

#include "UIView.h"

class UIGameOverScreen : public UIView {
private:
	std::vector<std::string> strings;
	std::vector<Position> positions;
	std::vector<int> colors;
	std::vector<int> sizes;
	std::vector<fontStyle_t> styles;
	std::vector<font_t> fonts;
	void clearArrays();
	void addToArrays(std::string, Position, int, font_t, fontStyle_t, int);
	
	int defaultColor;
	font_t defaultFont;
	fontStyle_t defaultStyle;
	int defaultSize;
	
public:
	UIGameOverScreen();
	~UIGameOverScreen();
	
	virtual void draw();
    virtual event_t process(event_t);
};



#endif