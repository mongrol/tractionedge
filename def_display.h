/*
 *  def_display.h
 *  tractionedge
 *
 *  Created by Steven Hamilton on 24/03/10.
 *  Copyright 2010 scorch.org. All rights reserved.
 *
 */
#ifndef __DEF_DISPLAY_H__
#define __DEF_DISPLAY_H__

//display
float const FRAMERATE = 0.02f;
int const MAPWIDTH = 40;
int const MAPHEIGHT = 30;
int const SCREENWIDTH = 800;
int const SCREENHEIGHT = 600;

int const TILEWIDTH = 16;
int const TILEHEIGHT = 16;
int const TEXTPADDING = 4;

int const STORYTEXTWIDTH = 64;

int const CONSOLEPANELHEIGHT = 6;
int const MASTERTILEIMAGEWIDTH = 160;
int const MASTERTILEIMAGEHEIGHT = 160;

int const COLORBLACK = 0;
int const COLORDARKGREY = 1;
int const COLORMIDDLEGREY = 2;
int const COLORLIGHTGREY = 3;
int const COLORRED = 4;
int const COLORDARKRED = 5;
int const COLORGREEN = 6;
int const COLORDARKGREEN = 7;
int const COLORYELLOW = 8;
int const COLORORANGE = 9;
int const COLORBROWN = 10;
int const COLORBLUE = 11;
int const COLORCYAN = 12;
int const COLORMAGENTA = 13;
int const COLORPURPLE = 14;
int const COLORWHITE = 15;

//fonts
int const FONTSIZELARGE = 24;
int const FONTSIZEMEDIUM = 20;
int const FONTSIZESMALL = 16;
int const FONTSIZESMALLX = 10;
int const FONTSIZETINY = 12;

enum font_t{
	TITLE,
	BODYTINY,
	BODYSMALL,
	BODYMEDIUM,
	BODYLARGE
};

enum fontStyle_t{
	REGULAR,
	BOLD,
	ITALIC
};

//UI
enum uiViewType_t{
	BASECHAR,
	PREPCHAR,
	DETAIL
};

#endif
