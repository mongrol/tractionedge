/*
 *  UIIntroScreen.h
 *  tractionedge
 *
 *  Created by Steven Hamilton on 15/05/10.
 *  Copyright 2010 scorch.org. All rights reserved.
 *
 */

#ifndef __UIINTROSCREEN_H__
#define __UIINTROSCREEN_H__

#include "UIView.h"

class UIIntroScreen : public UIView {
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
	UIIntroScreen();
	~UIIntroScreen();
	
	virtual void draw();
    virtual event_t process(event_t);
};



#endif