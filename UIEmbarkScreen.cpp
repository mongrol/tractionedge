/*
 *  UIEmbarkScreen.cpp
 *  tractionedge
 *
 *  Created by Steven Hamilton on 27/03/10.
 *  Copyright 2010 scorch.org. All rights reserved.
 *
 */

#include "defines.h"
#include "DisplayManager.h"
#include "GameWorld.h"
#include "Logger.h"
#include "UIEmbarkScreen.h"


UIEmbarkScreen::UIEmbarkScreen()
{
    state = SELECTMAN;
	defaultColor=COLORLIGHTGREY;
	defaultFont=BODYSMALL;
	defaultStyle=REGULAR;
	defaultSize=FONTSIZESMALL;

	squad=WORLD.getCreatures(FRIEND);
	creatureIt=squad.begin();
	selectedCreature=*creatureIt;
}

UIEmbarkScreen::~UIEmbarkScreen(){}

event_t UIEmbarkScreen::process(event_t event)
{
    if (event == KEY_ESCAPE) {
        event=EVENT_OPTIONS;
        return event;
    }
    if (state==SELECTMAN){
        switch(event){
            case KEY_DOWN:
				if (creatureIt==squad.end()-1)break;
				creatureIt++;
				event = EVENT_REDRAW;
                break;
            case KEY_UP:
				if (creatureIt==squad.begin())break;
                creatureIt--;
                event = EVENT_REDRAW;
                break;
            case KEY_RIGHT:
                if (selectedCreature->spareSkillPoints!=0){
                    state = SELECTMANSKILL;
                    selectedSkillIt=selectedCreature->skills.begin();
                    event = EVENT_REDRAW;
                }
                break;
			case KEY_E:
			    if(WORLD.getNumberOfCreatures(FRIEND,true)==WORLD.map.parameterMap["squadsize"]){
					event = EVENT_CONTINUE;
				}
                break;
            case KEY_RETURN:
                state = BASEINVENTORY;
				items = WORLD.getItems(WORLD.objectId);
				itemIt = items.begin();
				manItems = WORLD.getItems(selectedCreature->objectId);
				manItemIt = manItems.begin();
				makeItemStacks();
				selectedStackIt=itemStacks.begin();
				selectedItem=*itemIt;
                event = EVENT_REDRAW;
                break;
            case KEY_SPACE:
				if(WORLD.getNumberOfCreatures(FRIEND,true)<=WORLD.map.parameterMap["squadsize"]){
                    WORLD.toggleCreatureToMap(*creatureIt);
                    event = EVENT_REDRAW;
				}
                break;
            default:
                break;
            }
		selectedCreature=*creatureIt;
        }
    if (state==BASEINVENTORY){
        switch(event){
            case KEY_SPACE:
                state = SELECTMAN;
                event = EVENT_REDRAW;
                break;
			case KEY_DOWN:
				if (selectedStackIt==--itemStacks.end())break;
				selectedStackIt++;
				std::cout << "selectedStackIt = " << (*selectedStackIt).first << std::endl;
                event = EVENT_REDRAW;
                break;
            case KEY_UP:
				if (selectedStackIt==itemStacks.begin())break;
                selectedStackIt--;
				std::cout << "selectedStackIt = " << (*selectedStackIt).first << std::endl;
                event = EVENT_REDRAW;
                break;
			case KEY_RIGHT:{
                //change to maninventory only if he has something.
                std::vector<Item*> items; items = WORLD.getItems(selectedCreature->objectId);
                if (items.size()>0){
				    state = MANINVENTORY;
				    event = EVENT_REDRAW;
                    }
                break;
                }
			case KEY_RETURN:
				//we need to parse the item stack and add one item to man.
				for (itemIt=items.begin(); itemIt!=items.end(); ++itemIt) {
					std::cout << "itemIt = " << (*itemIt)->name << "item count= " << items.size() << std::endl;
					std::cout << "selectedStackIt = " << (*selectedStackIt).first << std::endl;
					if ((*selectedStackIt).first==(*itemIt)->name) {
						selectedItem=*itemIt;
						selectedCreature->addToInventory(selectedItem);
						//refresh our base items list
                        items = WORLD.getItems(WORLD.objectId);
						if (items.size()==0) state = MANINVENTORY;
						itemIt=items.begin();
						manItems = WORLD.getItems(selectedCreature->objectId);
                        manItemIt = manItems.begin();
						if (manItems.size()==1){
                            //if first item on man then set wield
							Item * weapon = *manItemIt;
					        selectedCreature->wielding=weapon->objectId;
                        }
						makeItemStacks();
						selectedStackIt=itemStacks.begin();
						event = EVENT_REDRAW;
						break;
					}
				}
				break;
            default:
                break;
            }
		selectedItem=*itemIt;
        }
	if (state==MANINVENTORY){
        switch(event){
            case KEY_SPACE:
                state = SELECTMAN;
                event = EVENT_REDRAW;
                break;
			case KEY_DOWN:
				if (manItemIt==manItems.end()-1)break;
                manItemIt++;
                event = EVENT_REDRAW;
                break;
            case KEY_UP:
				if (manItemIt==manItems.begin())break;
                manItemIt--;
                event = EVENT_REDRAW;
                break;
			case KEY_LEFT:
				state = BASEINVENTORY;
				event = EVENT_REDRAW;
				break;
			case KEY_W:{
				Item * weapon = *manItemIt;
				if (selectedCreature->wielding==weapon->objectId){
					selectedCreature->wielding=0;
				} else {
					selectedCreature->wielding=weapon->objectId;
				}
				event = EVENT_REDRAW;
				break;}
			case KEY_RETURN:
				selectedItem=*manItemIt;
				selectedItem->ownerObjectId=WORLD.objectId;
				manItems = WORLD.getItems(selectedCreature->objectId);
				if (manItems.size()==0) state = BASEINVENTORY;
				manItemIt=manItems.begin();
				items = WORLD.getItems(WORLD.objectId);
				itemIt=items.begin();
				makeItemStacks();
				selectedStackIt=itemStacks.begin();
				event = EVENT_REDRAW;
            default:
                break;
		}
		selectedItem=*itemIt;
	}
    if (state==SELECTMANSKILL){
        switch(event){
            case KEY_LEFT:
                state = SELECTMAN;
                event = EVENT_REDRAW;
                break;
            case KEY_DOWN:
				selectedSkillIt++;
				if (selectedSkillIt==selectedCreature->skills.end()) selectedSkillIt--;
				event = EVENT_REDRAW;
                break;
            case KEY_UP:
				if (selectedSkillIt==selectedCreature->skills.begin())break;
                selectedSkillIt--;
                event = EVENT_REDRAW;
                break;
            case KEY_PLUS:
                event = EVENT_REDRAW;
                if (selectedCreature->spareSkillPoints>0){
                    selectedCreature->spareSkillPoints--;
                    (*selectedSkillIt).second++;
                    if (selectedCreature->spareSkillPoints==0) state = SELECTMAN;
                }
                break;
        }
    }
    return event;
}

void UIEmbarkScreen::draw()
{
	clearArrays();
    if (state==BASEINVENTORY || state==MANINVENTORY){
        drawInventory();
        drawItemDetail(); 
    } else {
        drawBaseSquad();
		drawCreatureDetail();
    }
    drawTooltip();	
	//send it off to draw
    DISPLAY.gfxEngine.renderStrings(strings, colors, fonts, styles, sizes, positions);
}

void UIEmbarkScreen::drawBaseSquad()
{
	std::string str="Choose your best officers to partake in this adventure";
    Position pos(0,0);
	addToArrays(str, pos, defaultColor, defaultFont, defaultStyle, defaultSize);
	DISPLAY.gfxEngine.renderLine(0,FONTSIZESMALL+8,800,FONTSIZESMALL+8,COLORBROWN);
	DISPLAY.gfxEngine.renderLine(400,FONTSIZESMALL+8,400,600-FONTSIZESMALL*1.500,COLORBROWN);

    str="The Watchhouse\n--------------";
    pos.setXY(FONTSIZESMALLX*2,FONTSIZESMALL*3);
	addToArrays(str, pos, defaultColor, defaultFont, defaultStyle, defaultSize);
    
    squad=WORLD.getCreatures(FRIEND);
	
    pos.modXY(0, defaultSize*2);
    	
    for (int i=0;i<squad.size();i++){
        std::string str=squad[i]->name;
        fontStyle_t style;
		int color;
        if (*creatureIt==squad[i]){
            style=BOLD;
        } else{
            style=defaultStyle;
        }
		if (squad[i]->onMap){
            color=COLORGREEN;
        } else{
            color=defaultColor;
        }
	    addToArrays(str, pos, color, defaultFont, style, defaultSize);
        pos.modXY(0,defaultSize*2);
    }
	int spotsRemaining=	WORLD.map.parameterMap["squadsize"]-WORLD.getNumberOfCreatures(FRIEND,true);
    str="--------------\n "+tool.stringFromInt(spotsRemaining)+" remaining";
	addToArrays(str, pos, defaultColor, defaultFont, defaultStyle, defaultSize);
}

void UIEmbarkScreen::drawCreatureDetail()
{
	//draws detail of selected creature
	std::string str="Officer Particulars\n-------------------";
    Position pos(400+FONTSIZESMALLX*2,FONTSIZESMALL*3);
	int color=COLORLIGHTGREY;
	addToArrays(str, pos, color, defaultFont, defaultStyle, defaultSize);
	
	str=" Name:\nLevel:\n   XP:\n  Str:\n  Dex:\n  Int:\n   HP:\n   AP:\n  Enc:\n";
    pos.setXY(400+FONTSIZESMALLX*3,FONTSIZESMALL*5);
	color=COLORYELLOW;
	addToArrays(str, pos, color, defaultFont, defaultStyle, defaultSize);
	
	str="      "+selectedCreature->name;
	str+="\n      "+tool.stringFromInt(selectedCreature->level);
	str+="\n      "+tool.stringFromInt(selectedCreature->xp);
	str+="\n      "+tool.stringFromInt(selectedCreature->strength);
	str+="\n      "+tool.stringFromInt(selectedCreature->dexterity);
	str+="\n      "+tool.stringFromInt(selectedCreature->intelligence);
	str+="\n      "+tool.stringFromInt(selectedCreature->totalHitPoints);
	str+="\n      "+tool.stringFromInt(selectedCreature->totalActionPoints);
    str+="\n      "+selectedCreature->encumberanceString();
    

	color=COLORLIGHTGREY;
	addToArrays(str, pos, color, defaultFont, defaultStyle, defaultSize);
	
	str="Skills\n------";
	color=COLORLIGHTGREY;
	pos.modXY(0,FONTSIZESMALL*10);
	addToArrays(str, pos, color, defaultFont, defaultStyle, defaultSize);

    if (selectedCreature->spareSkillPoints!=0){
        str="+" + tool.stringFromInt(selectedCreature->spareSkillPoints) + " Skillup!";
        color=COLORGREEN;
        pos.modXY(8*TILEWIDTH,0);
	    addToArrays(str, pos, color, defaultFont, defaultStyle, defaultSize);
    }

	std::map<std::string, int>::iterator skillIt;
	skillIt=selectedCreature->skills.begin();
	
	for (int i=0;i<selectedCreature->skills.size();i++){
	
        fontStyle_t style = defaultStyle;
        if (skillIt==selectedSkillIt && state==SELECTMANSKILL) style = BOLD;

		str=(*skillIt).first+":";
		color=COLORYELLOW;
		pos.setXY(400+FONTSIZESMALLX*2,FONTSIZESMALL*(17+i));
		addToArrays(str, pos, color, defaultFont, style, defaultSize);
		str="            "+tool.stringFromInt((*skillIt).second);
		color=COLORLIGHTGREY;
		addToArrays(str, pos, color, defaultFont, style, defaultSize);
		skillIt++;
	}

    str="Equipment\n---------";
    color=COLORLIGHTGREY;
    pos.modXY(0,FONTSIZESMALL*2);
	addToArrays(str, pos, color, defaultFont, defaultStyle, defaultSize);
	
	manItems = WORLD.getItems(selectedCreature->objectId);
    pos.modXY(0,FONTSIZESMALL);
	
    for (int i=0;i<manItems.size();i++){
        std::string str=manItems[i]->name+" ("+tool.stringFromInt(manItems[i]->weight)+")";
        pos.modXY(0,defaultSize);
        fontStyle_t style;
        if (state==MANINVENTORY && *manItemIt==manItems[i]){
            style=BOLD;
        } else{
			style=defaultStyle;
        }
		if (manItems[i]->objectId==selectedCreature->wielding){
            str="(w)"+str;
		}
	    addToArrays(str, pos, defaultColor, defaultFont, style, defaultSize);
    }
	
}

void UIEmbarkScreen::drawInventory()
{
    std::string str="Equip your esteemed patrolman for the task ahead";
    Position pos(FONTSIZESMALLX*2,0);
	addToArrays(str, pos, defaultColor, defaultFont, defaultStyle, defaultSize);
    DISPLAY.gfxEngine.renderLine(0,FONTSIZESMALL+8,800,FONTSIZESMALL+8,COLORBROWN);
	DISPLAY.gfxEngine.renderLine(400,FONTSIZESMALL+8,400,600-FONTSIZESMALL*1.500,COLORBROWN);
	
	//Print Base Inventory
	str="The Watchhouse\n--------------";
    pos.setXY(FONTSIZESMALLX*2, defaultSize*3);
	addToArrays(str, pos, defaultColor, defaultFont, defaultStyle, defaultSize);
	
    items = WORLD.getItems(WORLD.objectId);
    pos.modXY(0,FONTSIZESMALL);
	
	//iterate our stackmap and print it
	fontStyle_t style;
	std::map<std::string,int>::iterator stackIt;
    for (stackIt=itemStacks.begin();stackIt!=itemStacks.end();++stackIt){
        pos.modXY(0,defaultSize);
        str=(*stackIt).first+" x"+tool.stringFromInt((*stackIt).second);
		if (state==BASEINVENTORY && stackIt==selectedStackIt){
            style=BOLD;
        } else{
			style=defaultStyle;
        }
	    addToArrays(str, pos, defaultColor, defaultFont, style, defaultSize);
    }
	
	//Print Creature Inventory
	str=selectedCreature->name+"\n--------";
    Position pos2(400+FONTSIZESMALLX*2, defaultSize*3);
	addToArrays(str, pos2, defaultColor, defaultFont, defaultStyle, defaultSize);
	
    manItems = WORLD.getItems(selectedCreature->objectId);
    pos2.modXY(0,FONTSIZESMALL);
	
    for (int i=0;i<manItems.size();i++){
        std::string str=manItems[i]->name+" ("+tool.stringFromInt(manItems[i]->weight)+")";
        pos2.modXY(0,defaultSize);
        fontStyle_t style;
        if (state==MANINVENTORY && *manItemIt==manItems[i]){
            style=BOLD;
        } else{
			style=defaultStyle;
        }
		if (manItems[i]->objectId==selectedCreature->wielding){
            str="(w)"+str;
		}
	    addToArrays(str, pos2, defaultColor, defaultFont, style, defaultSize);
    }
    //write encumberance
    str="Enc: "+selectedCreature->encumberanceString();
    pos2.modXY(0,FONTSIZESMALLX*3);
    int color=COLORYELLOW;
	addToArrays(str, pos2, color, defaultFont, defaultStyle, defaultSize);
}

void UIEmbarkScreen::drawItemDetail()
{
    switch (state){
        case BASEINVENTORY:{
	        for (itemIt=items.begin(); itemIt!=items.end(); ++itemIt) {
		        if ((*selectedStackIt).first==(*itemIt)->name) {
			        selectedItem=*itemIt;
                }
            }
            break;
        }
        case MANINVENTORY:{
            selectedItem=*manItemIt;
            break;
        }
        default:
            break;
    }

    //draws detail of selected item
	std::string str="Equipment Details\n----------------";
    Position pos(400+FONTSIZESMALLX*2,250+FONTSIZESMALL*3);
	int color=COLORLIGHTGREY;
	addToArrays(str, pos, color, defaultFont, defaultStyle, defaultSize);
	
	str="Name:\nWeight:\nSkill:\n";
    pos.setXY(400+FONTSIZESMALLX*2,250+FONTSIZESMALL*5);
	color=COLORYELLOW;
	addToArrays(str, pos, color, defaultFont, defaultStyle, defaultSize);
	
	str="        "+selectedItem->name;
    str+="\n        "+tool.stringFromInt(selectedItem->weight);
	str+="\n        "+selectedItem->skill;

    std::cout << str << std::endl;
	color=COLORLIGHTGREY;
	addToArrays(str, pos, color, defaultFont, defaultStyle, defaultSize);
	
	std::map<std::string, int>::iterator propertyIt;
	propertyIt=selectedItem->properties.begin();
	
	pos.modXY(0,FONTSIZESMALL*3);
	for (int i=0;i<selectedItem->properties.size();i++){
		
		str=(*propertyIt).first+":";
		color=COLORYELLOW;
		pos.modXY(0,FONTSIZESMALL*+i);
		addToArrays(str, pos, color, defaultFont, defaultStyle, defaultSize);
	
		str="        "+tool.stringFromInt((*propertyIt).second);
		color=COLORLIGHTGREY;
		addToArrays(str, pos, color, defaultFont, defaultStyle, defaultSize);
		propertyIt++;
	}
}

void UIEmbarkScreen::drawTooltip()
{
	DISPLAY.gfxEngine.renderLine(0,600-FONTSIZESMALL*1.5,800,600-FONTSIZESMALL*1.5,COLORBROWN);
	Position pos(0,600-FONTSIZESMALL*1.5);
    std::string str;
    switch(state){
        case SELECTMAN:{
            Creature * creature=*creatureIt;
            str="[Up/Dn] Select [Ent] Inv";
			if(WORLD.getNumberOfCreatures(FRIEND,true)<WORLD.map.parameterMap["squadsize"] && !creature->onMap){
                str+=" [Spc] Add to Patrol";
            } else if (creature->onMap) {
                str+=" [Spc] Remove from Patrol";
            }
            if(creature->spareSkillPoints>0){
                str+=" [Lft] Skills";
            }
			if(WORLD.getNumberOfCreatures(FRIEND,true)==WORLD.map.parameterMap["squadsize"]){
                str+=" [e] Embark";
            }
	        addToArrays(str, pos, defaultColor, defaultFont, defaultStyle, defaultSize);
            break;}
        case BASEINVENTORY:{
            str="[Up/Dn] Select Item [Ent] Equip [Spc] Exit";
	        addToArrays(str, pos, defaultColor, defaultFont, defaultStyle, defaultSize);
           break;}
		case MANINVENTORY:{
            str="[Up/Dn] Select Item [Ent] Unequip [w] Toggle Wield [Spc] Exit";
	        addToArrays(str, pos, defaultColor, defaultFont, defaultStyle, defaultSize);
			break;}
		case SELECTMANSKILL:{
            str="[Up/Dn] Select Skill [+] Inc Skill";
	        addToArrays(str, pos, defaultColor, defaultFont, defaultStyle, defaultSize);
			break;}
        default:
            break;
    }
}

void UIEmbarkScreen::clearArrays()
{
	strings.resize(0);
	colors.resize(0);
	positions.resize(0);
	styles.resize(0);
	sizes.resize(0);
	fonts.resize(0);
}

void UIEmbarkScreen::addToArrays(std::string str, Position pos, int col, font_t fnt, fontStyle_t style, int size)
{
	strings.push_back(str);
	positions.push_back(pos);
    colors.push_back(col);
    fonts.push_back(fnt);
    styles.push_back(style);
	sizes.push_back(size);
}

void UIEmbarkScreen::makeItemStacks()
{
	std::map<std::string,int>::iterator stackIt;
    std::string name;
	itemStacks.clear();
    for (stackIt=itemStacks.begin();stackIt!=itemStacks.end();++stackIt){
        stackIt->second=0;
	}
    for (int i=0;i<items.size();i++){
        name=items[i]->name;
        itemStacks[name]+=1;
		//std::cout << "stack count: adding: " << name << std::endl;
    }
	//std::cout << "itemStacks address is " << &itemStacks << std::endl;
}


