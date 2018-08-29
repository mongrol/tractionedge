/*
 *  Utility.cpp
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 10/07/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#include "GameWorld.h"
#include "Logger.h"
#include "Position.h"
#include "Utility.h"

std::string Utility::wrap(std::string str, int width)
{

    //then wrap it
    int index;
    std::string wrapped="";
    while(str.length()>=width)
    {
        index=str.substr(0,width).find_last_of(" ");
        wrapped.append(str.substr(0,index));
        wrapped.append("\n");
        str=str.substr(index+1);
    }
    wrapped.append(str);
    
    //first replace \'s with newlines
    std::string::size_type loc = wrapped.find("\\",0);
    while(loc!=std::string::npos)
    {
        std::cout << "found \\ at " << loc << std::endl;
        wrapped.replace(loc,1,"\n");
        loc = wrapped.find("\\",0);
    }
    return wrapped;
}

std::string Utility::stringFromInt(int integer)
{
	std::string newString;
	std::stringstream out;
	out << integer;
	newString=out.str();
	return (newString);
}

std::string Utility::stringFromBool(bool b)
{
    std::stringstream out;
    out << b;
    return out.str();
}

std::string Utility::stringFromDouble(double d)
{
	std::stringstream out;
	out << d;
	return out.str();
}

int Utility::stringToInt(std::string s)
{
	std::istringstream i(s);
	int x;
	i >> x;
	return x;
}

double Utility::getDistance(Position start, Position end)
{
	int x1 = start.x;
	int y1 = start.y;
	int x2 = end.x;
	int y2 = end.y;
	int dX = abs(x2-x1);
	int dY = abs(y2-y1);
	double dH = (dX*dX)+(dY*dY);
	double distance = sqrt(dH);
	
	//std::cout << "dx is " << stringFromInt(dX) << " dY is " << stringFromInt(dY) << " range is " << stringFromDouble(distance) << std::endl;
	return distance;
}
	
std::vector<move_t> Utility::findPath(Position origin, Position dest)
{
	std::cout << "----Finding Path---------" << std::endl;
	std::cout << stringFromInt(origin.x) << ":" << stringFromInt(origin.y) << " " << stringFromInt(dest.x) << ":" << stringFromInt(dest.y) << std::endl;
	
	//reset our return vector
	std::vector<move_t> moves;
	moves.resize(0);
	
	//build a vector of positions to represent map.
	std::vector<std::vector<Position> > positions;
	positions.resize(MAPHEIGHT);
	for (int i=0; i < MAPHEIGHT; ++i){
		positions[i].resize(MAPWIDTH);
	}
	
	//reset weights to 0 and set coords in our position objects
	for (int y=0;y < MAPHEIGHT; y++){
		for (int x=0;x < MAPWIDTH; x++){
			int weight=0;
			positions[y][x].setPathWeight(0);
			positions[y][x].setXY(x,y);
			//std::cout << stringFromInt(positions[y][x].getPathWeight());
		}
		//std::cout << std::endl;
	}
	
	//create an unfinished tile queue.
	std::queue<Position*> openList;
		
	//origin is the current square
	Position* currentTile = &positions[origin.y][origin.x];
	currentTile->setPathWeight(1);
	Position* selectedTile;
	
	//push the current tile onto our queue
	openList.push(currentTile);
	
	//iterate towards target
	do {
		if (!openList.empty()){
			currentTile = openList.front();
			//std::cout << "curTile: " << stringFromInt(currentTile->x) << ":" << stringFromInt(currentTile->y) << " weight " << stringFromInt(currentTile->pathWeight) << std::endl;
			//select an adjacent square who's still set to 0
			for (int i = currentTile->x-1; i < currentTile->x+2; i++){
				//check for out of bounds
				if (i<0) i=0;
				if (i==MAPWIDTH-2) break;
				
				for (int j = currentTile->y-1; j < currentTile->y+2; j++){
					//check for out of bounds;
					if (j<0) j=0;
					if (j==MAPHEIGHT-2) break;
					selectedTile = &positions[j][i];
					
					if (WORLD.map.isTerrainPassable(*selectedTile) && selectedTile->getPathWeight()==0){
						selectedTile->setPathWeight(currentTile->getPathWeight()+1);
                        openList.push(selectedTile);
					}
					//std::cout << "selTile: " << stringFromInt(selectedTile->x) << ":" << stringFromInt(selectedTile->y) << " weight " << stringFromInt(selectedTile->pathWeight) << std::endl;
				}
			}
			//std::cout << "openList size: " << stringFromInt(openList.size()) << std::endl;
			openList.pop();
			//std::cout << stringFromInt(currentTile->getPathWeight()) << std::endl;
			//std::cout << stringFromInt(currentTile->x) << ":" << stringFromInt(currentTile->y) << " " << stringFromInt(dest.x) << ":" << stringFromInt(dest.y) << std::endl;
		} else {
			std::cout << "Utility findPath() No path found. Aborting" << std::endl;
			return moves;
		}
	}
	while (!(currentTile->x==dest.x && currentTile->y==dest.y));
	
	//print debug map
	//for (int y=0;y < MAPHEIGHT; y++){
	//	for (int x=0;x < MAPWIDTH; x++){
    //       int value = positions[y][x].getPathWeight();
    //       std::string str=stringFromInt(value)+" ";
    //      if (value<10) str="0"+str;
    //        std::cout << str;
	//	}
	//	std::cout << std::endl;
	//}
	
	//iterate back to origin and build moves list
	//start at destination;
	currentTile = &positions[dest.y][dest.x];
	start:
	while (!(currentTile->x==origin.x && currentTile->y==origin.y))
	{
		//std::cout << "Tracing back from " << currentTile->x <<":"<< currentTile->y << " with weight " << currentTile->getPathWeight()
		//	<< " to origin " << origin.x <<":"<< origin.y << std::endl;
		//iterate around adjacent tiles;
		for (int i = currentTile->x-1; i < currentTile->x+2; i++){
			if (i<0) i=0;
			if (i==MAPWIDTH-1) break;
			for (int j = currentTile->y-1; j < currentTile->y+2; j++){
				if (j<0) j=0;
				if (j==MAPHEIGHT-1) break;
				selectedTile = &positions[j][i];
				//test if its the next step in the path
				//std::cout << "Checking " << selectedTile->x <<":"<< selectedTile->y << " with weight " << selectedTile->getPathWeight() << std::endl;
				if(selectedTile->getPathWeight()==currentTile->getPathWeight()-1)
				 {
					//calc the direction and store the move.
					int x1=selectedTile->x;
					int y1=selectedTile->y;
					int x2=currentTile->x;
					int y2=currentTile->y;
                    move_t move;
					if (x1==x2 && y1<y2) move=MOVE_DOWN;
					if (x1==x2 && y1>y2) move=MOVE_UP;
					if (x1<x2 && y1==y2) move=MOVE_RIGHT;
					if (x1>x2 && y1==y2) move=MOVE_LEFT;
					if (x1<x2 && y1<y2) move=MOVE_DOWN_RIGHT;
					if (x1>x2 && y1<y2) move=MOVE_DOWN_LEFT;
                    if (x1<x2 && y1>y2) move=MOVE_UP_RIGHT;
                    if (x1>x2 && y1>y2) move=MOVE_UP_LEFT;
					moves.push_back(move);
                    currentTile=selectedTile;
					// std::cout << "adding " << move << std::endl;
					goto start;
				 } 
			}
		}
	}
	
	//std::cout << "Moves size: " << stringFromInt(moves.size()) << std::endl;
	//print debug
	for (int i=0;i < moves.size();i++){
		std::cout << stringFromInt(moves[i]);
	}
	std::cout << std::endl;
	return moves;
}

std::vector<std::vector<Position> > Utility::pathWeight(Position origin)
{
	std::cout << "----Building pathWeight Map from " << stringFromInt(origin.x) << ":" << stringFromInt(origin.y) << std::endl;
	
	//build a vector of positions to represent map.
	std::vector<std::vector<Position> > positions;
	positions.resize(MAPHEIGHT);
	for (int i=0; i < MAPHEIGHT; ++i){
		positions[i].resize(MAPWIDTH);
	}
	
	//reset weights to 0 and set coords in our position objects
	for (int y=0;y < MAPHEIGHT; ++y){
		for (int x=0;x < MAPWIDTH; ++x){
			int weight=0;
			positions[y][x].setPathWeight(0);
			positions[y][x].setXY(x,y);
			//std::cout << stringFromInt(positions[y][x].getPathWeight());
		}
		//std::cout << std::endl;
	}
	
	//create an unfinished tile queue.
	std::queue<Position*> openList;
	
	//origin is the current square
	Position* currentTile = &positions[origin.y][origin.x];
	currentTile->setPathWeight(1);
	Position* selectedTile;
	
	//push it current tile onto our queue
	openList.push(currentTile);
	
	//iterate map and set weights
	do {
		currentTile = openList.front();
		//std::cout << "curTile: " << stringFromInt(currentTile->x) << ":" << stringFromInt(currentTile->y) << " weight " << stringFromInt(currentTile->pathWeight) << std::endl;
		//select an adjacent square who's still set to 0
		for (int i = std::max(0,currentTile->x-1); i < std::min(MAPWIDTH,currentTile->x+2); ++i){			
			for (int j = std::max(0,currentTile->y-1); j < std::min(MAPHEIGHT,currentTile->y+2); ++j){
				selectedTile = &positions[j][i];
				if (WORLD.map.isTerrainPassable(*selectedTile) && selectedTile->getPathWeight()==0){
					selectedTile->setPathWeight(currentTile->getPathWeight()+1);
					openList.push(selectedTile);
				}
				//std::cout << "selTile: " << stringFromInt(selectedTile->x) << ":" << stringFromInt(selectedTile->y) << " weight " << stringFromInt(selectedTile->pathWeight) << std::endl;
			}
		}
		//std::cout << "openList size: " << stringFromInt(openList.size()) << std::endl;
		openList.pop();
		//std::cout << stringFromInt(currentTile->getPathWeight()) << std::endl;
		//std::cout << stringFromInt(currentTile->x) << ":" << stringFromInt(currentTile->y) << " " << stringFromInt(dest.x) << ":" << stringFromInt(dest.y) << std::endl;
	}
	while (!openList.empty());
	
	//print debug map
	//for (int y=0;y < MAPHEIGHT; y++){
	//		for (int x=0;x < MAPWIDTH; x++){
	//			//std::cout << stringFromInt(positions[y][x].getPathWeight()) <<" ";
      //          std::cout << (char)positions[y][x].getPathWeight();
		//	}
	//		std::cout << std::endl;
	//}
	return positions;
}

float Utility::randFloat(float a, float b)
{
	return ((b-a)*((float)rand()/RAND_MAX))+a;
}

int Utility::rollDice(int num, int die)
{
    int result=0;
    //std::cout << "roll: rolling " << num << " * " << die << std::endl;
    for (int i=0;i<num;i++){
        const float scale = rand()/float(RAND_MAX);
        int roll=scale*die+1;
        result+=roll;
        //std::cout << "roll: rolled " << roll << std::endl;
    }
    //std::cout << "roll: returning " << result << std::endl; 
    return result;
}

int Utility::randNum(int low, int high)
{
    int diff = high-low;
    int result=rand() % diff + low;
    std::cout << "randNum returning: " << result << " diff " << diff << " low " << low << std::endl;
    return result;
}

Position Utility::getPositionFromDirection(Position position, move_t direction)
{
	//returns the Position
    Position newPosition;
	switch (direction){
		case MOVE_RIGHT:
			newPosition.setXY(position.x+1, position.y);
			break;
		case MOVE_LEFT:
			newPosition.setXY(position.x-1, position.y);
			break;
		case MOVE_UP:
			newPosition.setXY(position.x, position.y-1);
			break;
		case MOVE_DOWN:
			newPosition.setXY(position.x, position.y+1);
			break;
		case MOVE_UP_RIGHT:
			newPosition.setXY(position.x+1, position.y-1);
			break;
		case MOVE_UP_LEFT:
			newPosition.setXY(position.x-1, position.y-1);
			break;
		case MOVE_DOWN_RIGHT:
			newPosition.setXY(position.x+1, position.y+1);
			break;
		case MOVE_DOWN_LEFT:
			newPosition.setXY(position.x-1, position.y+1);
			break;
		default:
			break;
	}

    return newPosition;
}

int Utility::costForMove(Position start, Position dest)
{
    //does a findPath and returns the total cost to move from one position to
    //another.
    int totalCost=0;
    std::vector<move_t> moves=findPath(start,dest);    
    std::vector<move_t>::iterator movesIt;
    Position pos=start;
    for (movesIt=moves.begin();movesIt!=moves.end();movesIt++){
        pos=getPositionFromDirection(pos, (*movesIt));
	    int moveCost=WORLD.map.getMoveCost(pos);
        totalCost=totalCost+moveCost;
        LOG << "totalCost="+stringFromInt(totalCost);
    }
    return totalCost;
}

double Utility::getAngle(Position src, Position dst)
{
	Utility tool;
	//short variables to make things readable;
	int x1 = src.x;
	int y1 = src.y;
	int x2 = dst.x;
	int y2 = dst.y;
	double dx = x2-x1;
	double dy = y2-y1;
	
	//convert to an angle
	double angle = atan2(dy, dx) * 180/PI;
//	std::cout << "targetAngle=" << tool.stringFromDouble(angle) << std::endl;
	return angle;
}

