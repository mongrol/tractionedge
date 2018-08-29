/*
 *  Save.h
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 24/03/11
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __SAVER_H__
#define __SAVER_H__

#include "GameWorld.h"
#include "tinyxml.h"
#include "defines.h"

class Engine;

class Saver {
private:
    TiXmlDocument file;

public:
    void saveGame(Engine* engine);
    screenType_t loadGame(Engine* engine);
    bool doesSaveFileExist();
};

#endif
