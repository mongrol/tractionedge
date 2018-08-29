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

#include "fov.h"
#include "GameWorld.h"
#include <string.h> //I think that's where memset is declared... or was it stdlib.h? Change if I'm wrong
#include <algorithm>

/******** CLASS VARIABLE DECLARATIONS ********/

//since the Fov class isn't instantiated, the class variables are initialised here
int Fov::fovMap[MAPWIDTH][MAPHEIGHT] = {};
bool Fov::transparencyMap[MAPWIDTH][MAPHEIGHT] = {};

/******** METHODS' IMPLEMENTATIONS ********/

//copies the transparency status from the map's tiles to the array
//NOTE: This is how it works in UR. You'll need to rewrite this method to suit your map's architecture
void Fov::prepareTransparencyMap ()
{
    for (int x = 0; x < MAPWIDTH; x++){
		for (int y = 0; y < MAPHEIGHT; y++){
			if (WORLD.map.isTileTranslucent(x,y)){
				transparencyMap[x][y] = 1;
			} else {
				transparencyMap[x][y] = 0;
			}

		}
	}
}

//calculate the FOV
void Fov::calculate (int sourceX, int sourceY, int range) {
    //first, zero the FOV map
    memset(fovMap,0,sizeof(fovMap));
    //std::cout << "range:" << range << std::endl;
    //set source position as visible
    fovMap[sourceX][sourceY] = 1;
	
	//calculate the obstacles array size
	int arraySize = MAPWIDTH * MAPHEIGHT / 7;

    //calculate the four quadrants
    calculateQuadrant(sourceX,sourceY,1,1,range,arraySize);
	calculateQuadrant(sourceX,sourceY,1,-1,range,arraySize);
	calculateQuadrant(sourceX,sourceY,-1,1,range,arraySize);
	calculateQuadrant(sourceX,sourceY,-1,-1,range,arraySize);
}

void Fov::calculateQuadrant (int sourceX, int sourceY, int dx, int dy, int range, int arraySize) {
    //calculate the vertical edged octant
    {
        //OK, first, we set up the key variables.
        int iteration = 1; //iteration of the algo for this octant
        bool done = false; //indicates whether the octant calculation can be ended
        int totalObstacles = 0; //the total number of found *VISIBLE* obstacles
        int obstaclesInLastLine = 0; //the total number of found visibe obstacles up to the end of the previous processed line
        double startAngle[arraySize], endAngle[arraySize]; //the arrays that store the blocked angles - the index indicates the order of encountering obstacles

        //do while there are unblocked angles left and the algo is within the map's boundaries:
        //scan progressive lines/columns from the PC outwards.
        int x, y = sourceY + (iteration*dy); //the outer slope's coordinates (first processed cell in the line)
        if (y < 0 || y >= MAPHEIGHT) done = true; //check whether the line's within the map's boundaries.
        while(!done) {
            done = true; //set the "done" var to true - it'll be set to false if there are any visible cells in the line
            //process cells in the line
            int processedCell = 0; //the cell number in the current line - always starts at 0 :)
			//Here's where the real boogie starts. First, calculate the slope ranges. These are the slope ranges for a single cell in the current line.
			//since line 1 has 2 cells, line 2 has 3 cells and so on, the line number (the variable "iteration" needs to be summed 1).
			//HalfSlopes is exactly half of this range. It will be used to calculate the centre key angle of a cell.
			double slopesPerCell = 1.0f/(double)(iteration+1);
            double halfSlopes = slopesPerCell * 0.5f;
			//These calculations used to be inside the following for loop conditions, but since the result is always the same, there's no point
			//in calculating them. Minx, maxx, miny and maxy are the cell number's coordinates up to which the current line will be calculated.
			//For instance, if the pc's coordinates are 50,10, minx in the first line will be 49, in the next one it'll be 48, etc. But if the
			//pc' coordinates are 1,10, minx will be 0 from the first line onward. The line calculation will have to stop at this cell, else it
			//produces a segmentation fault. Hope that's clear.
            int minx = std::max(0,sourceX-iteration), maxx = std::min(MAPWIDTH-1,sourceX+iteration);
            //OK, this calculates the entire line. As you can see, it goes horizontally here, starting from i and ending at either minx or maxx.
			for (x = sourceX; x >= minx && x <= maxx; x+=dx ) {
                if (((x-sourceX)*(x-sourceX))+((iteration)*(iteration))>(range*range+range)) continue;
                //std::cout << "Radius Calc X " << ((x-sourceX)*(x-sourceX))<<" + Y " <<((y-sourceY)*(y-sourceY)) << " < R " << (range*range) << std::endl;
				bool visible = true; //the current cell is initially visible
                //calculate the current cell's angles:
                double startSlope = (double)processedCell*slopesPerCell; //the starting angle: the current cell number multiplied by a single cell's angles range.
                double centreSlope = startSlope + halfSlopes; //the centre slope is the starting slope plus half the angle range
                double endSlope = startSlope + slopesPerCell; //end slope is starting slope plus full angle range per cell. Also could be (double)(currentCell+1)*slopesPerCell, but I think it's slower
                //the following conditions says: process the cell if and only if:
				//1: some obstacles have been found already. If not, what's the point? It's visible anyway.
				//2: the cell hasn't been previously marked as visible. A cell at the octant's border might have been processed already.
				//It it's visible, we can be sure it has already been processed, so we can skip it.
				if (obstaclesInLastLine > 0 && fovMap[x][y] == 0) {
                    int idx = 0; //the index number for the obstacles arrays
                    while(visible && idx < obstaclesInLastLine) { //while the cell isn't marked unlit and there are obstacles left to process...
                        if (transparencyMap[x][y] == true) { //if the cell is transparent...
                            if (centreSlope > startAngle[idx] && centreSlope < endAngle[idx]) //if the cell's centre slope is within the obstacle's blocked range
                                visible = false; //then the cell is marked as unlit. (no else statement because the cell starts out lit)
                            }
                        else { //else if the cell is opaque...
                            if (startSlope >= startAngle[idx] && endSlope <= endAngle[idx]) //if the opaque cell is ENTIRELY within the blocked range of some other obstacle...
                                visible = false; //mark it as unlit
                        }
						//Now, this line introduces some magic that makes the algo less symmetric, but eliminates disconnected visible cells.
						//It's sort of complicated. Basically, it means "if the two previous cells (from the previous line - one in diagonal,
						//one in orthogonal direction towards the PC) are either opaque or unlit, the current visible cell cannot be lit". The
						//check is complicated, so let's explain it in detail:
						//visible - the cell has to be marked as visible first since this REMOVES disconnected visible cells
						//(fovMap[x][y-dy] == 0 || !transparencyMap[x][y-dy]) - the orthogonally adjacent cell in the previous line is
						//either unlit or opaque
						//x-dx >= 0 && x-dx <= MAPWIDTH - this checks whether the diagonally adjacent cell is within the map's bounds...
						//...and if it is...
						//(fovMap[x-dx][y-dy] == 0 || !transparencyMap[x-dx][y-dy]) - if the diagonally adjacent cell in the previous line is
						//either opaque or unlit...
						//...well, if all these are true, the cell is marked unlit.
                        if (visible && (fovMap[x][y-dy] == 0 || !transparencyMap[x][y-dy]) && (x-dx >= 0 && x-dx < MAPWIDTH && (fovMap[x-dx][y-dy] == 0 || !transparencyMap[x-dx][y-dy]))) visible = false;
                        idx++; //increase the index to check the next obstacle
                    }
                }
                //Alright, so the cell has been processed. If it is invisible, ignore it, but if it's visible...
				if (visible) {
                    fovMap[x][y] = 1; //mark it as lit in the lit cells array
                    done = false; //tell the algo that it's not finished yet
                    //if the cell is opaque, block the adjacent slopes
                    if (!transparencyMap[x][y]) {
                        startAngle[totalObstacles] = startSlope; //the cell's starting slope becomes blocked...
                        endAngle[totalObstacles++] = endSlope; //so does the end slope. Total number of obstacles is increased by 1.
                    }
                }
                processedCell++; //the number of the currently processed cell is increased by 1
            }
			//the entire line has been processed.
            iteration == range ? done = true : iteration++; //if the currently processed line number (iteration) is equal to the specified range, end. Else, increase the iteration variable by 1 and continue.
            obstaclesInLastLine = totalObstacles; //mark the number of obstacles in the last line as the current total count.
            y += dy; //update the y variable
            if (y < 0 || y >= MAPHEIGHT) done = true; //check whether the y variable is within the map's bounds and if it isn't, end the algo
        }
    }
    //calculate the horizontal edged octant
    {
        int iteration = 1;
        bool done = false;
        int totalObstacles = 0;
        int obstaclesInLastLine = 0;
        double startAngle[arraySize], endAngle[arraySize];

        int x = sourceX + (iteration*dx), y; //this line differs from the other octant
        if (x < 0 || x >= MAPWIDTH) done = true; //this line differs from the other octant
        while(!done) {
            done = true;
            int processedCell = 0;
            double slopesPerCell = 1.0f/(double)(iteration+1);
            double halfSlopes = slopesPerCell * 0.5f;
            int miny = std::max(0,sourceY-iteration), maxy = std::min(MAPHEIGHT-1,sourceY+iteration); //this line differs from the other octant
            for (y = sourceY; y >= miny && y <= maxy; y+=dy) { //this line differs from the other octant
                if (((y-sourceY)*(y-sourceY))+((iteration)*(iteration))>(range*range+range)) continue;
                bool visible = true;
                double startSlope = (double)processedCell*slopesPerCell;
                double centreSlope = startSlope + halfSlopes;
                double endSlope = startSlope + slopesPerCell;
                if (obstaclesInLastLine > 0 && fovMap[x][y] == 0) {
                    int idx = 0;
                    while(visible && idx < obstaclesInLastLine) {
                        if (transparencyMap[x][y] == true) {
                            if (centreSlope > startAngle[idx] && centreSlope < endAngle[idx])
                                visible = false;
                            }
                        else {
                            if (startSlope >= startAngle[idx] && endSlope <= endAngle[idx])
                                visible = false;
                        }
                        if (visible && (fovMap[x-dx][y] == 0 || !transparencyMap[x-dx][y]) && (y-dy >= 0 && y-dy < MAPHEIGHT && (fovMap[x-dx][y-dy] == 0 || !transparencyMap[x-dx][y-dy]))) visible = false; //this line differs from the other octant
                        idx++;
                    }
                }
                if (visible) {
                    fovMap[x][y] = 1;
                    done = false;
                    if (!transparencyMap[x][y]) {
                        startAngle[totalObstacles] = startSlope;
                        endAngle[totalObstacles++] = endSlope;
                    }
                }
                processedCell++;
            }
			iteration == range ? done = true : iteration++;
            obstaclesInLastLine = totalObstacles;
            x += dx; //this line differs from the other octant
            if (x < 0 || x >= MAPWIDTH) done = true; //this line differs from the other octant
        }
    }
}

