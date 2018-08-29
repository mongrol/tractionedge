/*
 *  GfxSFML.h
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 6/07/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#ifndef __GFXSFML_H__
#define __GFXSFML_H__

#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "def_events.h"
#include "def_display.h"
#include "Position.h"
#include "Terrain.h"

class GfxEngine{
private:
	sf::RenderWindow m_mainWindow;
	//sf::Font m_displayFont;
	//sf::Font m_titleFont;
	sf::Color colorLookup[16];
	
    std::map<char, sf::String> tileLookupMap;
	std::map<int, sf::Shape> colorShapeMap;
	std::map<font_t, sf::Font> fontLookupMap;
	std::map<fontStyle_t, sf::String::Style> styleLookupMap;
	
	std::vector<sf::Image> tileImages;
    std::vector<std::vector<sf::Sprite> > mapSprites;
	std::vector<sf::Sprite> overlaySprites;
	
	sf::Image titleImage;
	sf::Sprite titleSprite;
	sf::Image caratImage;
	sf::Sprite carat;
	
public:
	GfxEngine();
	~GfxEngine();
	
	//map rendering
	void createMapTerrain(std::vector<std::vector<Terrain> >&);
	void createMapOverlay(std::vector<gfxId_t*> &, std::vector<Position*> &);
	void animateProjectile();
	void renderMap();
	void renderOverlay();

	//UI rendering.
	void renderStrings(std::vector<std::string>&,  std::vector<int>&, std::vector<font_t>&, std::vector<fontStyle_t>&, std::vector<int>&, std::vector<Position>&);
	void renderFrame(int,int,int,int); //not used
	void renderLine(int,int,int,int,int); //not used
	void renderCarat(int,int,int);
	
	//general screen functions
	void createWindow();
    void closeWindow();
	void clearScreen();
	void renderScreen();
	
	bool isOpen();
	event_t event();
	
	//to be removed
	void drawTitleGfx();
};
#endif
