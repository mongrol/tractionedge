/*
 *  GfxSFML.cpp
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 6/07/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#include <vector>
#include <string>
#include "GfxSFML.h"
#include "Terrain.h"
#include "DisplayManager.h"
#include "GameWorld.h"
#include "defines.h"

GfxEngine::GfxEngine()
{
	//create window
	m_mainWindow.Create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT, 32), "Traction Edge");
	m_mainWindow.UseVerticalSync(true); 
	m_mainWindow.EnableKeyRepeat(true);
	
	//create lookup table for colors
	colorLookup[0] = sf::Color(0,0,0,255);
	colorLookup[1] = sf::Color(100,100,100,255);
	colorLookup[2] = sf::Color(150,150,150,255);
	colorLookup[3] = sf::Color(200,200,200,255);
	colorLookup[4] = sf::Color(255,0,0,255);
	colorLookup[5] = sf::Color(100,0,0,255);
	colorLookup[6] = sf::Color(0,255,0,255);
	colorLookup[7] = sf::Color(0,100,0,255);
	colorLookup[8] = sf::Color(255,255,0,255);
	colorLookup[9] = sf::Color(220,170,30,255);
	colorLookup[10] = sf::Color(180,120,0,255);
	colorLookup[11] = sf::Color(0,0,255,255);
	colorLookup[12] = sf::Color(0,255,255,255);
	colorLookup[13] = sf::Color(255,0,255,255);
	colorLookup[14] = sf::Color(150,0,150,255);
	colorLookup[15] = sf::Color(255,255,255,255);
		
	//load fonts
	sf::Font titleFont;
	sf::Font tinyFont;
	sf::Font smallFont;
	sf::Font mediumFont;
	sf::Font largeFont;
	
	titleFont.LoadFromFile("fonts/Vtks Revolt.ttf", 60);
	tinyFont.LoadFromFile("fonts/LiberationMono-Regular.ttf",FONTSIZETINY);
	smallFont.LoadFromFile("fonts/LiberationMono-Regular.ttf",FONTSIZESMALL);
	mediumFont.LoadFromFile("fonts/LiberationMono-Regular.ttf",FONTSIZEMEDIUM);
	largeFont.LoadFromFile("fonts/LiberationMono-Regular.ttf",FONTSIZELARGE);


	//create lookup table of fonts
	fontLookupMap[TITLE]=titleFont;
	fontLookupMap[BODYTINY]=tinyFont;
	fontLookupMap[BODYSMALL]=smallFont;
	fontLookupMap[BODYMEDIUM]=mediumFont;
	fontLookupMap[BODYLARGE]=largeFont;
	
	//create lookup table of font styles
	styleLookupMap[REGULAR]=sf::String::Regular;
	styleLookupMap[BOLD]=sf::String::Bold;
	styleLookupMap[ITALIC]=sf::String::Italic;
	
	//create lookup table of color shapes
	for (int i=0;i < 15;i++){
		sf::Shape rect = sf::Shape::Rectangle((float)0, (float)0, (float)TILEWIDTH, (float)TILEHEIGHT, colorLookup[i]);
		rect.SetBlendMode(sf::Blend::None);
		colorShapeMap[i]=rect;
	}
	
	//load title image
	if (!titleImage.LoadFromFile("images/tetitle.png")) {
		std::cout << "ERROR loading title image";
	}
	
	//load carat image
	if (!caratImage.LoadFromFile("images/carat.png")) {
		std::cout << "ERROR loading carat image";
	}
	carat.SetImage(caratImage);
	carat.SetColor(colorLookup[COLORORANGE]);	
	
	//load tile png
	sf::Image masterImage;
	if (!masterImage.LoadFromFile("images/tetiles.png")) {
		std::cout << "ERROR loading master image";
	}
	titleSprite.SetImage(titleImage);
	
	//iterate image and generate sf::Images
	tileImages.resize(0);
	for (int y=0; y<MASTERTILEIMAGEHEIGHT/TILEHEIGHT;y++){
		for (int x=0; x<MASTERTILEIMAGEWIDTH/TILEWIDTH; x++) {
			sf::Image image(TILEWIDTH,TILEHEIGHT,colorLookup[0]);
			sf::IntRect rect(x*TILEWIDTH,y*TILEHEIGHT,x*TILEWIDTH+TILEWIDTH,y*TILEHEIGHT+TILEHEIGHT);
			image.Copy(masterImage,0,0,rect,false);
			switch (tileImages.size()) {
				case TARGET_S:
					image.SetSmooth(false);
					break;
				case SELECTED_S:
					image.SetSmooth(false);
					break;
				default:
					image.SetSmooth(true);
					break;
			}
			tileImages.push_back(image);

		}
	}
	std::cout << tileImages.size() << " images loaded" << std::cout;
	
	//create the terrain map. This is the background image of the current data.
    mapSprites.resize(MAPHEIGHT);
    for (int i=0; i<MAPHEIGHT; i++){
        mapSprites[i].resize(MAPWIDTH);
    };
	for (int n=0; n < MAPHEIGHT; n++){
		for (int m=0; m < MAPWIDTH; m++){
		    sf::Sprite tileSprite;
			tileSprite.SetPosition(m*TILEWIDTH,n*TILEHEIGHT);
			tileSprite.SetColor(colorLookup[COLORWHITE]);
			tileSprite.SetBlendMode(sf::Blend::None);
		    mapSprites[n][m]=tileSprite;
        }
    }
}

GfxEngine::~GfxEngine()
{
}

void GfxEngine::clearScreen()
{
	m_mainWindow.Clear(sf::Color(0, 0, 0));
}

void GfxEngine::createMapTerrain(std::vector<std::vector<Terrain> >& mapArray)
{
	//update the terrain map. The sprites were positioned in the constructor
    // so this should only need to set the correct image.
	for (int n=0; n < MAPHEIGHT; n++){
		for (int m=0; m < MAPWIDTH; m++){
	//		std::cout << mapArray[n][m].isVisible; 
            if (mapArray[n][m].isVisible){
		        mapSprites[n][m].SetImage(tileImages[mapArray[n][m].gfxId]);
		        mapSprites[n][m].SetColor(colorLookup[COLORWHITE]);
            } else if (mapArray[n][m].wasVisible){
		        mapSprites[n][m].SetImage(tileImages[mapArray[n][m].gfxId]);
		        mapSprites[n][m].SetColor(colorLookup[COLORDARKGREY]);
            } else {
                mapSprites[n][m].SetImage(tileImages[BLANK_S]);
            }
		}
      //  std::cout << std::endl;
    }
}

void GfxEngine::createMapOverlay(std::vector<gfxId_t*>& gfxIds, std::vector<Position*>& positions)
{
	overlaySprites.resize(0);
	//this creates overlay sprites, players, monsters, everything that's not terrain. Called every frame update.
	for (int g=0; g < gfxIds.size();g++){
		sf::Sprite tileSprite;
		tileSprite.SetImage(tileImages[*gfxIds[g]]);
		tileSprite.SetPosition(positions[g]->x*TILEWIDTH,positions[g]->y*TILEHEIGHT);
		tileSprite.SetBlendMode(sf::Blend::Alpha);
		overlaySprites.push_back(tileSprite);
	}
}

void GfxEngine::animateProjectile()
{
	Position * start = &WORLD.anim.coords[0];
	Position * end = &WORLD.anim.coords[WORLD.anim.coords.size()-1];
	std::cout << "Animating from " << start->string() << " to " << end->string() << std::endl;
	
	sf::Sprite tileSprite;
	tileSprite.SetImage(tileImages[BULLET_S]);
	
	for (int i=0; i<WORLD.anim.coords.size();i++) {
		renderMap();
		renderOverlay();
		tileSprite.SetPosition(WORLD.anim.coords[i].x*TILEWIDTH,WORLD.anim.coords[i].y*TILEHEIGHT);
		if (WORLD.map.map[WORLD.anim.coords[i].y][WORLD.anim.coords[i].x].isVisible){
		    m_mainWindow.Draw(tileSprite);
        }
        m_mainWindow.Display();
 		sf::Sleep(0.15f);
	}
	PROMPT("");
}

void GfxEngine::renderMap()
{
	for (int n=0; n < MAPHEIGHT; n++){
		for (int m=0; m < MAPWIDTH; m++){
			m_mainWindow.Draw(mapSprites[n][m]);
		}
	}
}

void GfxEngine::renderOverlay()
{
	for (int i=0; i<overlaySprites.size(); i++){
		m_mainWindow.Draw(overlaySprites[i]);
	}
}

void GfxEngine::renderStrings(std::vector<std::string>& strings, std::vector<int>& colors, std::vector<font_t>& fonts, std::vector<fontStyle_t>& styles, std::vector<int>& sizes, std::vector<Position>& positions)
{
	for (int i=0;i < strings.size();i++){
		sf::String text;
		text.SetText(strings[i]);
		text.SetPosition(positions[i].x,positions[i].y);
		text.SetColor(colorLookup[colors[i]]);
		text.SetFont(fontLookupMap[fonts[i]]);
        text.SetSize(sizes[i]);
	    text.SetStyle(styleLookupMap[styles[i]]);
		m_mainWindow.Draw(text);
	}

}

void GfxEngine::renderFrame(int x1, int y1, int x2, int y2)
{
	sf::Shape rect = sf::Shape::Rectangle(x1, y1, x2, y2, colorLookup[COLORBROWN],true,colorLookup[COLORBROWN]);
	rect.SetOutlineWidth(1);
	rect.EnableFill(false);
	m_mainWindow.Draw(rect);
}

void GfxEngine::renderLine(int x1, int y1, int x2, int y2, int color)
{
	sf::Shape line = sf::Shape::Line(x1, y1, x2, y2, 1, colorLookup[color]);
	m_mainWindow.Draw(line);
}


void GfxEngine::renderScreen()
{
	//called once all UI objects have finished drawing for that frame.
	m_mainWindow.Display();

    //std::cout << "fps: " << m_mainWindow.GetFrameTime() << std::endl;
}

void GfxEngine::createWindow()
{
	//create window
	m_mainWindow.Create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT, 32), "Traction Edge");
	m_mainWindow.UseVerticalSync(true); 
	m_mainWindow.EnableKeyRepeat(true);
}	

void GfxEngine::closeWindow()
{
    m_mainWindow.Close();
    std::cout << "Closing Window";
}

bool GfxEngine::isOpen()
{
	if (m_mainWindow.IsOpened()) return true;
	else return false;
}

event_t GfxEngine::event()
{
	//fetch events and send only keypresses back
	sf::Event Event;
	while (m_mainWindow.GetEvent(Event))
	 {
		//close and exit
		if (Event.Type == sf::Event::Closed)
			m_mainWindow.Close();
		
		if (Event.Type == sf::Event::KeyPressed)
		 {
            if (Event.Key.Code == sf::Key::Equal){
                return (KEY_PLUS);}
            if (Event.Key.Code == sf::Key::Dash){
                return (KEY_MINUS);}
            if (Event.Key.Control && Event.Key.Code == sf::Key::Q){
                return (KEY_CTRL_Q);}
			if (Event.Key.Code == sf::Key::Space)
				return (KEY_SPACE);
			if (Event.Key.Code == sf::Key::Escape)
				return (KEY_ESCAPE);
			if (Event.Key.Code == sf::Key::Return)
				return (KEY_RETURN);
			if (Event.Key.Code == sf::Key::Right || Event.Key.Code == sf::Key::Numpad6)
				return (KEY_RIGHT);
			if (Event.Key.Code == sf::Key::Left || Event.Key.Code == sf::Key::Numpad4)
				return (KEY_LEFT);
			if (Event.Key.Code == sf::Key::Up || Event.Key.Code == sf::Key::Numpad8)
				return (KEY_UP);
			if (Event.Key.Code == sf::Key::Down || Event.Key.Code == sf::Key::Numpad2)
				return (KEY_DOWN);
			if (Event.Key.Code == sf::Key::Numpad9)
				return (KEY_UP_RIGHT);
			if (Event.Key.Code == sf::Key::Numpad7)
				return (KEY_UP_LEFT);
			if (Event.Key.Code == sf::Key::Numpad3)
				return (KEY_DOWN_RIGHT);
			if (Event.Key.Code == sf::Key::Numpad1)
				return (KEY_DOWN_LEFT);
			
			if (Event.Key.Code == sf::Key::Tab)
				return (KEY_TAB);
			if (Event.Key.Code == sf::Key::T)
				return (ACTION_TARGET);
			if (Event.Key.Code == sf::Key::R)
				return (KEY_R);
			if (Event.Key.Code == sf::Key::A)
				return (KEY_A);
			if (Event.Key.Code == sf::Key::A)
				return (KEY_A);
			if (Event.Key.Code == sf::Key::B)
				return (KEY_B);
			if (Event.Key.Code == sf::Key::C)
				return (KEY_C);
			if (Event.Key.Code == sf::Key::D)
				return (KEY_D);
			if (Event.Key.Code == sf::Key::E)
				return (KEY_E);
			if (Event.Key.Code == sf::Key::F)
				return (KEY_F);
			if (Event.Key.Code == sf::Key::G)
				return (KEY_G);
			if (Event.Key.Code == sf::Key::I)
				return (KEY_I);
			if (Event.Key.Code == sf::Key::H)
				return (KEY_H);
			if (Event.Key.Code == sf::Key::N)
				return (KEY_N);
			if (Event.Key.Code == sf::Key::P)
				return (KEY_P);
			if (Event.Key.Code == sf::Key::S)
				return (KEY_S);
			if (Event.Key.Code == sf::Key::W)
				return (KEY_W);
			if (Event.Key.Code == sf::Key::Y)
				return (KEY_Y);

		 }
	 }
	return EVENT_NULL;
}

void GfxEngine::drawTitleGfx()
{
	int x = (SCREENWIDTH-titleImage.GetWidth())/2;
	std::cout << "x=" << x << std::endl;
	titleSprite.SetPosition(x,60);
	m_mainWindow.Draw(titleSprite);
}

void GfxEngine::renderCarat(int x, int y, int size)
{
	carat.SetPosition(x,y+size/4);
	carat.Resize(size,size);
	m_mainWindow.Draw(carat);
}
