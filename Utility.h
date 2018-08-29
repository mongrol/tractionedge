/*
 *  Utility.h
 *  sfmltest1
 *
 *  Created by Steven Hamilton on 10/07/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "GameWorld.h"


class Utility{
	private:

	public:
    std::string wrap(std::string str,int width);
	std::string stringFromInt(int);
	std::string stringFromBool(bool);
	std::string stringFromDouble(double);
	int stringToInt(std::string);
	
	float randFloat(float, float);
	double getDistance(Position, Position);
	std::vector<move_t> findPath(Position, Position);
	std::vector<std::vector<Position> > pathWeight(Position);

    //our dice function. num= number of rolls, die=sided dice. Returns the
    //total
    int rollDice(int num, int die);
    int randNum(int low, int high);
    Position getPositionFromDirection(Position position, move_t direction);
    int costForMove(Position start, Position dest);
    double getAngle(Position src, Position dst);
};

#endif
