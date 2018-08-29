/*
 *  UITitleScreen.h
 *  tractionedge
 *
 *  Created by Steven Hamilton on 25/03/10.
 *  Copyright 2010 scorch.org. All rights reserved.
 *
 */

#ifndef __UITITLESCREEN_H__
#define	__UITITLESCREEN_H__

#include "UIView.h"
#include "Saver.h"

class UITitleScreen : public UIView {
private:
	std::vector<std::string> strings;
	std::vector<Position> positions;
	std::vector<int> colors;
	std::vector<int> sizes;
	std::vector<fontStyle_t> styles;
	std::vector<font_t> fonts;
	void clearArrays();
	void addToArrays(std::string, Position, int, font_t, fontStyle_t, int);
    
    Saver saver;
public:
	UITitleScreen();
	~UITitleScreen();
	
	virtual void draw();
    virtual event_t process(event_t);
};


#endif
