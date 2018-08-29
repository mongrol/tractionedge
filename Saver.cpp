/*
 *  Saver.cpp
 *
 *  Created by Steven Hamilton on 24/06/09.
 *  Copyright 2009. All rights reserved.
 *
 */

#include <iostream>
#include <vector>
#include "tinyxml.h"
#include "EngineState.h"
#include "Saver.h"
#include "Creature.h"
#include "Utility.h"
#include "defines.h"

void Saver::saveGame(Engine * engine)
{
    //save xml header
    TiXmlDeclaration * decl = new TiXmlDeclaration("1.0","","");
    file.LinkEndChild(decl);

    //save current world attribs and enginestate
    TiXmlElement * state = new TiXmlElement("engineState");
    state->SetAttribute("screenType_t", engine->previousStateScreenType());
    file.LinkEndChild(state);

    TiXmlElement * world = new TiXmlElement("world");
    world->SetAttribute("objectId", WORLD.objectId);
    world->SetAttribute("objectIdIndex", WORLD.objectIdIndex);
    world->SetAttribute("level", WORLD.level);
    world->SetAttribute("finalLevel", WORLD.finalLevel);
    world->SetAttribute("mapObjectId", WORLD.map.objectId);
    file.LinkEndChild(world);
    

    //save the worlds creature list
    std::vector<Creature*> creatures = WORLD.getCreatures();
    std::vector<Creature*>::iterator creatureIt;
    for (creatureIt=creatures.begin();creatureIt!=creatures.end();++creatureIt){
        TiXmlElement * element = new TiXmlElement("Creature");
        element->SetAttribute("objectId", (*creatureIt)->objectId);
        element->SetAttribute("gfxId", (*creatureIt)->gfxId);
        element->SetAttribute("name", (*creatureIt)->name);
        element->SetAttribute("type", (*creatureIt)->type);
        element->SetAttribute("faction", (*creatureIt)->faction);
        element->SetAttribute("level", (*creatureIt)->level);
        element->SetAttribute("xp", (*creatureIt)->xp);
        element->SetAttribute("xpValue", (*creatureIt)->xpValue);
        element->SetAttribute("oldXP", (*creatureIt)->oldXP);
        element->SetAttribute("spareSkillPoints", (*creatureIt)->spareSkillPoints);
        element->SetAttribute("spareAttributePoints", (*creatureIt)->spareAttributePoints);
        element->SetAttribute("strength", (*creatureIt)->strength);
        element->SetAttribute("dexterity", (*creatureIt)->dexterity);
        element->SetAttribute("intelligence", (*creatureIt)->intelligence);
        element->SetAttribute("totalHitPoints", (*creatureIt)->totalHitPoints);
        element->SetAttribute("totalActionPoints", (*creatureIt)->totalActionPoints);
        element->SetAttribute("currentHitPoints", (*creatureIt)->currentHitPoints);
        element->SetAttribute("currentActionPoints", (*creatureIt)->currentActionPoints);
        element->SetAttribute("wielding", (*creatureIt)->wielding);
        element->SetAttribute("onMap", (*creatureIt)->onMap);
        element->SetAttribute("isAlive", (*creatureIt)->isAlive);

        //position
        TiXmlElement * position = new TiXmlElement("Position");
        position->SetAttribute("x", (*creatureIt)->position.x);
        position->SetAttribute("y", (*creatureIt)->position.y);
        element->LinkEndChild(position);

        //skills
	    std::map<std::string, int>::iterator skillIt;
        for (skillIt=(*creatureIt)->skills.begin();skillIt!=(*creatureIt)->skills.end();skillIt++){
            TiXmlElement * skill = new TiXmlElement("skill");
            skill->SetAttribute(skillIt->first, skillIt->second);
            element->LinkEndChild(skill);
        }

        //add creature to file
        file.LinkEndChild(element);
    }

    //save the worlds item list
    std::vector<Item*> items = WORLD.getItems();
    std::vector<Item*>::iterator itemIt;
    for (itemIt=items.begin();itemIt!=items.end();++itemIt){
        TiXmlElement * element = new TiXmlElement("Item");
        element->SetAttribute("objectId", (*itemIt)->objectId);
        element->SetAttribute("gfxId", (*itemIt)->gfxId);
        element->SetAttribute("ownerObjectId", (*itemIt)->ownerObjectId);
        element->SetAttribute("onMap", (*itemIt)->onMap);
        element->SetAttribute("type", (*itemIt)->type);
        element->SetAttribute("itemAttackType", (*itemIt)->itemAttackType);
        element->SetAttribute("name", (*itemIt)->name);
        element->SetAttribute("skill", (*itemIt)->skill);
        element->SetAttribute("weight", (*itemIt)->weight);

        //position
        TiXmlElement * position = new TiXmlElement("Position");
        position->SetAttribute("x", (*itemIt)->position.x);
        position->SetAttribute("y", (*itemIt)->position.y);
        element->LinkEndChild(position);
        
        //properties
	    std::map<std::string, int>::iterator propertyIt;
        for (propertyIt=(*itemIt)->properties.begin();propertyIt!=(*itemIt)->properties.end();propertyIt++){
            TiXmlElement * property = new TiXmlElement("property");
            property->SetAttribute(propertyIt->first, propertyIt->second);
            element->LinkEndChild(property);
        }
        
        //add item to file
        file.LinkEndChild(element);
    }

    //save map. We only serialise the terrain type then use that to build it
    //create it later.
    TiXmlElement * map = new TiXmlElement("Map");
	for (int y=0;y < MAPHEIGHT; y++){
		for (int x=0;x < MAPWIDTH; x++){
            TiXmlElement * terrain = new TiXmlElement("terrain");
            terrain->SetAttribute("gfxId_t", WORLD.map.map[y][x].gfxId);
            map->LinkEndChild(terrain);
        }
    }
    file.LinkEndChild(map);

    file.SaveFile("save.xml");
}

screenType_t Saver::loadGame(Engine * engine)
{
    Utility tool;
    //remove worlds items and creatures if they exist - DONE
    //open file first - DONE
    //read world stats, find out level - DONE
    //load map for that level using starting stats, - DONE
    //load in creatures and items - DONE
    //replace map terrain with file data - DONE
    //setup engine state - DONE
    //bootstrap! - DONE

    TiXmlDocument file("save.xml");
	if (file.LoadFile()){
        //read world stats
	    TiXmlHandle fileHandle(&file);
        
        TiXmlElement * worldNode=fileHandle.FirstChild("world").Element();
        WORLD.objectId = tool.stringToInt(worldNode->Attribute("objectId"));
        WORLD.objectIdIndex = tool.stringToInt(worldNode->Attribute("objectIdIndex"));
        WORLD.level = tool.stringToInt(worldNode->Attribute("level"));
        WORLD.finalLevel = tool.stringToInt(worldNode->Attribute("finalLevel"));
        WORLD.map.objectId = tool.stringToInt(worldNode->Attribute("mapObjectId"));
        std::cout << "Loading world finalLevel=" << WORLD.finalLevel << std::endl;

        //load the map data for current level. No creatures. We need params.
        WORLD.map.loadMapTerrain(WORLD.level);
        WORLD.map.loadMapParameters(WORLD.level);
        
        //replace map data with load file data
        TiXmlElement * terrainNode=fileHandle.FirstChild("Map").Element()->FirstChildElement("terrain");
        std::vector<int> gids;
        gids.resize(0);
        for (terrainNode; terrainNode; terrainNode=terrainNode->NextSiblingElement("terrain")){
            TiXmlAttribute * attribute=terrainNode->FirstAttribute();
            std::string str=attribute->ValueStr();
            int value=tool.stringToInt(str);
            gids.push_back(value);
        //    std::cout << "gfxId_t:" << value << std::endl;
        }

        WORLD.map.replaceMapTerrain(gids);

        //load creatures
		TiXmlElement * creatureNode=fileHandle.FirstChild("Creature").Element();
		for(creatureNode; creatureNode; creatureNode=creatureNode->NextSiblingElement("Creature") ){
            //get attributes
            std::map<std::string, std::string> attribs;
            TiXmlAttribute * attribute=creatureNode->FirstAttribute();
            for(attribute; attribute; attribute=attribute->Next() ){
                std::string str;
                str=attribute->Name();
                attribs[str] = attribute->ValueStr();
                //std::cout << "Attrib: " << attribs[str] << std::endl;
            }
            //get position
            TiXmlElement * positionNode=creatureNode->FirstChildElement("Position");
            TiXmlAttribute * xattribute=positionNode->FirstAttribute();
            TiXmlAttribute * yattribute=xattribute->Next();
            Position position(tool.stringToInt(xattribute->ValueStr()),tool.stringToInt(yattribute->ValueStr()));
            std::cout << "Creatures Position is: " << position.string() << std::endl;
           
            //get skills
	        std::map<std::string, int> skills;
            TiXmlElement * skillNode=creatureNode->FirstChildElement("skill");
            for(skillNode; skillNode; skillNode=skillNode->NextSiblingElement("skill")){
                TiXmlAttribute * attribute=skillNode->FirstAttribute();
                std::string str;
                str=attribute->Name();
                skills[str] = tool.stringToInt(attribute->ValueStr());
               // std::cout << "Skill: " << skills[str] << std::endl;
            }
            //create creature and add to world
            Creature creature(attribs, position, skills);
            WORLD.addCreature(creature);
        }

        // load items
        TiXmlElement * itemNode=fileHandle.FirstChild("Item").Element();
        for(itemNode; itemNode; itemNode=itemNode->NextSiblingElement("Item")){
            std::map<std::string, std::string> attribs;
            TiXmlAttribute * attribute=itemNode->FirstAttribute();
            for(attribute; attribute; attribute=attribute->Next() ){
                std::string str;
                str = attribute->Name();
                attribs[str] = attribute->ValueStr();
             //   std::cout << "Attrib: " << attribs[str] << std::endl;
            }
            //get position
            TiXmlElement * positionNode=itemNode->FirstChildElement("Position");
            TiXmlAttribute * xattribute=positionNode->FirstAttribute();
            TiXmlAttribute * yattribute=xattribute->Next();
            Position position(tool.stringToInt(xattribute->ValueStr()),tool.stringToInt(yattribute->ValueStr()));
            std::cout << "Item Position is: " << position.string() << std::endl;
            //get properties
	        std::map<std::string, int> props;
            TiXmlElement * propNode=itemNode->FirstChildElement("property");
            for(propNode; propNode; propNode=propNode->NextSiblingElement("property")){
                TiXmlAttribute * attribute=propNode->FirstAttribute();
                std::string str;
                str=attribute->Name();
                props[str] = tool.stringToInt(attribute->ValueStr());
               // std::cout << "Skill: " << skills[str] << std::endl;
            }
            Item item(attribs, position, props);
            WORLD.addItem(item);
        }

    //bootstrap game
    TiXmlElement * engineNode=fileHandle.FirstChild("engineState").Element();
    TiXmlAttribute * attribute=engineNode->FirstAttribute();
    std::string str = attribute->ValueStr();
    screenType_t screenType = (screenType_t)tool.stringToInt(str);
    return screenType;
    } else {
        std::cout << "Save file missing or corrupt" << std::endl;
        exit(0);
    }
}

bool Saver::doesSaveFileExist()
{
    std::cout << "Looking for save file" << std::endl;
    TiXmlDocument file("save.xml");
    bool result = file.LoadFile();
    std::cout << "save File Exists = " << result << std::endl;
    if (result) return true;
    std::cout << "file error: " << file.ErrorDesc() << std::endl;
    return false;
}
