/*
 *  AnimManager.h
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 29/08/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#ifndef _ANIMMANAGER_H__
#define _ANIMMANAGER_H__

#include "Item.h"
#include "Position.h"
#include <vector>
#include "DisplayManager.h"

class AnimManager: public Item{
public:
		
	std::deque<gfxId_t> gfxIds;
	std::deque<Position> coords;
	
	int index;
		
	void initialiseProjectile();
};


#endif
