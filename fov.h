/*
* Mingos' Restrictive Precise Angle Shadowcasting (MRPAS)
* Copyright (c) 2009 Dominik Marczuk
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Dominik Marczuk may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY DOMINIK MARCZUK ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL DOMINIK MARCZUK BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __FOV_H__
#define __FOV_H__

//#include "defines.h"
#include "def_display.h"

class GameWorld;

class Fov {
    private:
	inline static void calculateQuadrant (int sourceX, int sourceY, int dx, int dy, int range, int arraySize); //calculates one quadrant of the map - dx and dy are either 1 or -1.

	public:
	static bool transparencyMap[MAPWIDTH][MAPHEIGHT]; //map of transparent/opaque tiles
    static int fovMap[MAPWIDTH][MAPHEIGHT]; //the visibility map - this will be referenced when checking whether a tile is in FOV
    static void prepareTransparencyMap (); //fills transparencyMap with adequate content - needs editing in each game :)
	static void calculate (int sourceX, int sourceY, int range = 0); //calculate the FOV
};

#endif