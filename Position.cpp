/*
 *  untitled.cpp
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 24/09/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#include "Position.h"
#include "Utility.h"

Position::Position()
{
	x=-1;
	y=-1;
	pathWeight=0;
}

Position::~Position()
{
}

Position::Position(int x1, int y1)
{
	x=x1;
	y=y1;
}

bool operator<(const Position &a, const Position &b)
{
	if (a.pathWeight < b.pathWeight) return true;
	return false;
}

bool operator==(const Position &a, const Position &b)
{
	if (a.x == b.x && a.y == b.y) return true;
	return false;
}

void Position::setXY(int x1, int y1)
{
	x=x1;
	y=y1;
}

void Position::modXY(int x1, int y1)
{
	x+=x1;
	y+=y1;
}

void Position::setPathWeight(int weight)
{
	pathWeight=weight;
}

int Position::getPathWeight()
{
	return pathWeight;
}

std::string Position::string()
{
	Utility tool;
	std::string str=tool.stringFromInt(x)+":"+tool.stringFromInt(y);
	return str;
}

bool Position::validatePosition()
{
	if (x>=0 && x<=MAPWIDTH && y>=0 && y<MAPHEIGHT) return true;
	else return false;
}
