/*
 *  UITitleScreen.cpp
 *  tractionedge
 *
 *  Created by Steven Hamilton on 25/03/10.
 *  Copyright 2010 scorch.org. All rights reserved.
 *
 */

#include "Creature.h"
#include "DisplayManager.h"
#include "def_items.h"
#include "Logger.h"
#include "UIMatchScreen.h"
#include "Utility.h"

UIMatchScreen::UIMatchScreen()
{
	WORLD.initPlayers();
    stateChanged=true;
	matchState = STATE_PLAYER;

    //Map window
    DISPLAY.gfxEngine.createMapTerrain(WORLD.map.map);
    PROMPT("(tab)cycle, (a)ttack, (ent)end turn, (r)eserve AP, (numpad)move");
    MSG("Good luck Constables");
}

UIMatchScreen::~UIMatchScreen(){}

event_t UIMatchScreen::process(event_t event)
{
	switch (matchState) {
		case STATE_PLAYER:
            {
            WORLD.clearPrompt();    
            
            if (event == KEY_ESCAPE) {
                event=EVENT_OPTIONS;
                return event;
            }

            if (event == KEY_CTRL_Q){
                PROMPT("Quit? (y/n)");
                matchState = STATE_PROMPT_QUIT;
            }
            
            bool moved=false;
			if (event == KEY_RIGHT) moved=WORLD.selectedPlayer->move(MOVE_RIGHT);
			if (event == KEY_LEFT) moved=WORLD.selectedPlayer->move(MOVE_LEFT);
			if (event == KEY_UP) moved=WORLD.selectedPlayer->move(MOVE_UP);
			if (event == KEY_DOWN) moved=WORLD.selectedPlayer->move(MOVE_DOWN);
			if (event == KEY_UP_RIGHT) moved=WORLD.selectedPlayer->move(MOVE_UP_RIGHT);
			if (event == KEY_UP_LEFT) moved=WORLD.selectedPlayer->move(MOVE_UP_LEFT);
			if (event == KEY_DOWN_RIGHT) moved=WORLD.selectedPlayer->move(MOVE_DOWN_RIGHT);
			if (event == KEY_DOWN_LEFT) moved=WORLD.selectedPlayer->move(MOVE_DOWN_LEFT);
		    if (moved){
                LOG << "Checking for ENEMY reaction fire";
                WORLD.checkReaction(ENEMY);
            }

			// commands
			if (event == KEY_TAB) WORLD.selectNextPlayer();
			
            //attack
			if (event == KEY_A) {
                if (WORLD.selectedPlayer->isWielding(RANGED)){
                    matchState = STATE_PROMPT_FIRINGMODE;
			        PROMPT("(a)imed, (s)napshot, (esc)cancel");
                    }
                else if (WORLD.selectedPlayer->isWielding(MELEE)){
                    matchState = STATE_PROMPT_MELEEATTACK;
                    PROMPT("Direction?");
                } else {
                    MSG(WORLD.selectedPlayer->name+" isn't wielding anything");
                }
			}

            //set reserve
            if (event == KEY_R) {
                Item * item = WORLD.getItemForObjectId(WORLD.selectedPlayer->wielding);
                if(item->itemAttackType==RANGED){
                    matchState = STATE_PROMPT_RESERVE;
                    PROMPT("Reserve AP for (a)imed, (s)napshot, (n)one, (esc)cancel");
                } else  {
                    MSG("The constable is wielding a melee weapon");
                }
            }

            //end turn
			if (event == KEY_RETURN){
				MSG("Ending turn");
				
				//prep for AI's turn
				WORLD.squadAI.initAI();
				matchState= STATE_AI;
			}
			break;
            }
        case STATE_PROMPT_FIRINGMODE:
            if (event == KEY_ESCAPE) {
				WORLD.endSelectionCursor();
				matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                break;
			}
			if (event == KEY_A){
				WORLD.selectedPlayer->setFiringMode(AIMED);
                WORLD.initSelectionCursor();
                MSG(WORLD.selectedPlayer->name+" take's careful aim...");
				matchState = STATE_TARGET;
                PROMPT("Manually target or (r)cycle targets, (esc)cancel");
                break;
			}
			if (event == KEY_S){
				WORLD.selectedPlayer->setFiringMode(SNAPSHOT);
                WORLD.initSelectionCursor();
                MSG(WORLD.selectedPlayer->name+" has a potshot...");
				matchState = STATE_TARGET;
                PROMPT("Manually target or (r)cycle targets, (esc)cancel");
                break;
			}
			break;
        case STATE_PROMPT_MELEEATTACK:{
            bool attacked = true;
			if (event == KEY_RIGHT){ 
                attacked = WORLD.selectedPlayer->attack(MELEE, MOVE_RIGHT);
			    matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                }
			if (event == KEY_LEFT){ 
                bool attacked = WORLD.selectedPlayer->attack(MELEE, MOVE_LEFT);
			    matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                }
			if (event == KEY_UP){
                bool attacked = WORLD.selectedPlayer->attack(MELEE, MOVE_UP);
			    matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                }
			if (event == KEY_DOWN){
                bool attacked = WORLD.selectedPlayer->attack(MELEE, MOVE_DOWN);
			    matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                }
			if (event == KEY_UP_RIGHT){ 
                bool attacked = WORLD.selectedPlayer->attack(MELEE, MOVE_UP_RIGHT);
			    matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                }
			if (event == KEY_UP_LEFT){ 
                bool attacked = WORLD.selectedPlayer->attack(MELEE, MOVE_UP_LEFT);
			    matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                }
			if (event == KEY_DOWN_RIGHT){
                bool attacked = WORLD.selectedPlayer->attack(MELEE, MOVE_DOWN_RIGHT);
			    matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                }
			if (event == KEY_DOWN_LEFT){
                bool attacked = WORLD.selectedPlayer->attack(MELEE, MOVE_DOWN_LEFT);
			    matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                }
	        if (!attacked) MSG("Cannot attack. Not enough AP.");
            break;
            }
        case STATE_PROMPT_RESERVE:
            if (event == KEY_ESCAPE) {
                matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                break;
            }
            if (event == KEY_A){
                WORLD.selectedPlayer->setReserve(AIMED);
                matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                break;
            }
            if (event == KEY_S){
                WORLD.selectedPlayer->setReserve(SNAPSHOT);
                matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                break;
            }
            if (event == KEY_N){
                WORLD.selectedPlayer->setReserve(NONE);
                matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                break;
            }
        case STATE_PROMPT_QUIT:
            if (event == KEY_Y){
                DISPLAY.closeWindow();
                break;
            }
            if (event == KEY_N){
                matchState = STATE_PLAYER;
                WORLD.clearPrompt();
            }
            break;
        case STATE_TARGET:
			if (event == KEY_RIGHT) WORLD.moveSelectionCursor(MOVE_RIGHT);
			if (event == KEY_LEFT) WORLD.moveSelectionCursor(MOVE_LEFT);
			if (event == KEY_UP) WORLD.moveSelectionCursor(MOVE_UP);
			if (event == KEY_DOWN) WORLD.moveSelectionCursor(MOVE_DOWN);
			if (event == KEY_UP_RIGHT) WORLD.moveSelectionCursor(MOVE_UP_RIGHT);
			if (event == KEY_UP_LEFT) WORLD.moveSelectionCursor(MOVE_UP_LEFT);
			if (event == KEY_DOWN_RIGHT) WORLD.moveSelectionCursor(MOVE_DOWN_RIGHT);
			if (event == KEY_DOWN_LEFT) WORLD.moveSelectionCursor(MOVE_DOWN_LEFT);
			if (event == KEY_R) WORLD.targetNextEnemy(WORLD.selectedPlayer);
            if (event == KEY_RETURN){
				matchState = STATE_PLAYER;
                WORLD.clearPrompt();
				bool attacked=WORLD.selectedPlayer->attack(RANGED, NO_MOVE);
			    if (!attacked) MSG("Cannot attack. Not enough AP.");
                break;
            }
			if (event == KEY_ESCAPE) {
				WORLD.endSelectionCursor();
				matchState = STATE_PLAYER;
                WORLD.clearPrompt();
                break;
			}
			break;
		case STATE_AI:
			//this all needs to move outside this Switch and loop without interrupting the drawloop.
			if (WORLD.squadAI.processTurn()){
				WORLD.resetPlayers();
				WORLD.selectLivePlayer();
				matchState=STATE_PLAYER;
			}
			event=EVENT_REDRAW;
			break;
		default:
			break;
	}
	event=WORLD.checkMatchStatus(event);
    return event;
}

void UIMatchScreen::draw()
{
	clearArrays();
    drawMap();
    drawCharPanels();
    drawConsole();
    drawPrompt();
    DISPLAY.gfxEngine.renderStrings(strings, colors, fonts, styles, sizes, positions);
    DISPLAY.gfxEngine.renderLine(0,600-FONTSIZESMALL-4,800,600-FONTSIZESMALL-4,COLORBROWN);
}


void UIMatchScreen::drawMap()
{
	//create a new map array of sprites.
	if (stateChanged){
		DISPLAY.gfxEngine.createMapTerrain(WORLD.map.map);
		stateChanged=true;
	}

    //std::cout << "matchState=" << matchState << std::endl;
	//draw items
	std::vector<Item*> itemList = WORLD.getItems(true);
	//std::cout << "Drawing Items " << itemList.size() << std::endl;
	for (int i=0; i<itemList.size(); i++) {
		Position &pos=itemList[i]->position;
		if (WORLD.map.map[pos.y][pos.x].isVisible){
            gfxIds.push_back(&itemList[i]->gfxId);
			positions_p.push_back(&itemList[i]->position);
		}
	}
	
    //draw creatures
    std::vector<Creature*> creatureList = WORLD.getCreatures(true);
	for (int i=0; i<creatureList.size(); i++) {
		Position &pos=creatureList[i]->position;
		if (WORLD.map.map[pos.y][pos.x].isVisible){
		gfxIds.push_back(&creatureList[i]->gfxId);
		positions_p.push_back(&creatureList[i]->position);
		}
	}

	//add selected player reticle
	if (matchState==STATE_PLAYER && WORLD.selectedPlayer->isAlive){
        Position pos;
	    gfxId_t gfxId=SELECTED_S;
	    pos.setXY(WORLD.selectedPlayer->position.x,WORLD.selectedPlayer->position.y);
	    positions_p.push_back(&pos);
	    gfxIds.push_back(&gfxId);
    }
		
	//add targeting reticle
	if (matchState==STATE_TARGET) {
	//	std::cout << "Adding targetting reticle" << std::endl;
		gfxIds.push_back(&WORLD.selectionGfxId);
		positions_p.push_back(&WORLD.target);
	}
	
	//add animatables
	if (matchState==STATE_ANIMATE) {
		DISPLAY.gfxEngine.animateProjectile();
	}
	
	DISPLAY.gfxEngine.createMapOverlay(gfxIds, positions_p);
	
	//draw map
	DISPLAY.gfxEngine.renderMap();
	DISPLAY.gfxEngine.renderOverlay();
}

void UIMatchScreen::drawCharPanels()
{
	Utility tool;
    int x=MAPWIDTH*TILEWIDTH;
    int y=0;

    std::vector<Creature*> squad = WORLD.getCreatures(FRIEND, true);
    for (int i=0; i<squad.size(); i++) {

	    Position pos(x,y);
	    
        //add non variable text
	    addToArrays(squad[i]->name+"\nHP:    AP:\nXP:\nWld:\nRsv:\n\n", pos, COLORLIGHTGREY, BODYSMALL, REGULAR, FONTSIZESMALL);
	
	    //add HP
	    pos.setXY(x+32,y+FONTSIZESMALL);
	    addToArrays(tool.stringFromInt(squad[i]->currentHitPoints), pos, COLORGREEN, BODYSMALL, REGULAR, FONTSIZESMALL);
	    
	    //add AP
	    pos.modXY(72,0);
	    addToArrays(tool.stringFromInt(squad[i]->currentActionPoints), pos, COLORCYAN, BODYSMALL, REGULAR, FONTSIZESMALL);
	    
		//add XP
	    pos.setXY(x+32,y+FONTSIZESMALL*2);
	    addToArrays(tool.stringFromInt(squad[i]->xp), pos, COLORLIGHTGREY, BODYSMALL, REGULAR, FONTSIZESMALL);
		
        //add wielding
        pos.setXY(x+48,y+FONTSIZESMALL*3);
        if (!squad[i]->isWielding()){
            addToArrays("empty!", pos, COLORYELLOW, BODYSMALL, REGULAR, FONTSIZESMALL);
        } else {
			Item * weapon=WORLD.getItemForObjectId(squad[i]->wielding);
		    addToArrays(weapon->name, pos, COLORYELLOW, BODYSMALL, REGULAR, FONTSIZESMALL);
    	}   

        //add reserve
        pos.setXY(x+48,y+FONTSIZESMALL*4);
        
        switch (squad[i]->getReserveMode()){
            case AIMED:
                addToArrays("Aimed", pos, COLORMAGENTA, BODYSMALL, REGULAR, FONTSIZESMALL);
                break;
            case SNAPSHOT:
                addToArrays("Snap", pos, COLORMAGENTA, BODYSMALL, REGULAR, FONTSIZESMALL);
                break;            
            case NONE:
                addToArrays("None", pos, COLORDARKGREY, BODYSMALL, REGULAR, FONTSIZESMALL);
                break;
            default:
                break;
        }
	    //draw frame if selected player
		
		int color= COLORORANGE;
		
	    if (squad[i]==WORLD.selectedPlayer) {
		    DISPLAY.gfxEngine.renderLine(x-1,y,x-1+16,y,color);
			DISPLAY.gfxEngine.renderLine(x-1,y,x-1,y+16,color);

			//DISPLAY.gfxEngine.renderLine(SCREENWIDTH-16,y,SCREENWIDTH,y,color);
		    //DISPLAY.gfxEngine.renderLine(SCREENWIDTH,y,SCREENWIDTH,y+16,color);

			DISPLAY.gfxEngine.renderLine(x-1,y+FONTSIZESMALL*5-16+2,x-1,y+FONTSIZESMALL*5+2,color);
		    DISPLAY.gfxEngine.renderLine(x-1,y+FONTSIZESMALL*5+2,x+16-1,y+FONTSIZESMALL*5+2,color);

			//DISPLAY.gfxEngine.renderLine(SCREENWIDTH-16,y+FONTSIZESMALL*4+2,SCREENWIDTH,y+FONTSIZESMALL*4+2,color);
		    //DISPLAY.gfxEngine.renderLine(SCREENWIDTH,y+FONTSIZESMALL*4-16+2,SCREENWIDTH,y+FONTSIZESMALL*4+2,color);

	    }
        y+=TILEHEIGHT*7;
    }
}

void UIMatchScreen::drawConsole()
{
    int x=0;
    int y=MAPHEIGHT*TILEHEIGHT;

	for (int i=0;i<WORLD.consoleBuffer.size();i++){
		std::string str=WORLD.consoleBuffer[i];
        //std::cout << "console: " << str << std::endl;
		int col = COLORDARKGREY;
		if (i == WORLD.consoleBuffer.size()-1) col = COLORLIGHTGREY;
		Position pos(x,y+i*FONTSIZESMALL);
	    font_t font=BODYSMALL;
	    fontStyle_t style=REGULAR;
	    int size=FONTSIZESMALL;
	    
        addToArrays(str, pos, col, font, style, size);
	}
}

void UIMatchScreen::drawPrompt()
{
    int x=0;
    int y=MAPHEIGHT*TILEHEIGHT;
	int col = COLORYELLOW;
	Position pos(x,y+6*FONTSIZESMALL+2);
    font_t font=BODYSMALL;
    fontStyle_t style=REGULAR;
    int size=FONTSIZESMALL;
    addToArrays(WORLD.promptString, pos, col, font, style, size);
}

void UIMatchScreen::clearArrays()
{
	strings.resize(0);
	colors.resize(0);
	positions.resize(0);
	positions_p.resize(0);
    gfxIds.resize(0);
    styles.resize(0);
	sizes.resize(0);
	fonts.resize(0);
}

void UIMatchScreen::addToArrays(std::string str, Position pos, int col, font_t fnt, fontStyle_t style, int size)
{
	strings.push_back(str);
	positions.push_back(pos);
    colors.push_back(col);
    fonts.push_back(fnt);
    styles.push_back(style);
	sizes.push_back(size);
}
