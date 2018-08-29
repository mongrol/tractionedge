/*
 *  AnimManager.cpp
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 29/08/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#include "AnimManager.h"
#include "defines.h"

void AnimManager::initialiseProjectile()
{
	//set initial values
	index=0;
	gfxIds.resize(0);
	coords.resize(0);
}
