/*
 *  Position.h
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 24/09/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

// Position describes a coordinate on the map.

#ifndef __POSITION_H__
#define __POSITION_H__

#include <string>

class Position {
public:
	int x;
	int y;
	
	Position();
	Position(int,int);
	~Position();
	
	friend bool operator<(const Position &a, const Position &b);
	friend bool operator==(const Position &a, const Position &b);

	void setXY(int,int);
	void modXY(int,int);
	
	//AIpathing
	int pathWeight;
	void setPathWeight(int);
	int getPathWeight();
	std::string string();
	bool validatePosition();
};

#endif
