/*
 *  MapGenerator.h
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 18/11/11.
 *  Copyright 2011 scorch.org. All rights reserved.
 *
 */

#ifndef __MAPGENERATOR_H__
#define __MAPGENERATOR_H__

#include <map>
#include <vector>
#include <string>
#include "Map.h"
#include "Position.h"
#include "Terrain.h"
#include "defines.h"

int const MAP_SQUARE_SIZE=5;

enum landscape_t {
    LANDSCAPE_FARM,
    LANDSCAPE_TOWN,
    LANDSCAPE_CITY,
    LANDSCAPE_SEWER
};

enum building_t {
    FARM_HUT,
    FARM_HOUSE,
    FARM_BARN
};

class Building
{
    private:
        std::vector<std::vector<Terrain> > loadBuildingTerrain();

    public:
        std::vector<std::vector<Terrain> > tiles;
        Building();
        Building(std::vector<std::vector<Terrain> >, int);
        ~Building();

        int getHeight();
        int getWidth();
};

class MapGenerator
{
private:
    std::vector<std::vector<Terrain> > getBuilding();
    std::map<int,int> loadParameters(std::string);
    void loadBuildingFile(const std::string fileName, const int size, const int items);
    std::vector<Building> buildings;
    std::map<int, std::vector<Building*> > buildingMap;
    std::vector<std::vector<Terrain> > createForest(std::vector<std::vector<Terrain> >);
    std::vector<std::vector<Terrain> > createRoad(std::vector<std::vector<Terrain> >);
public:
	MapGenerator();
	~MapGenerator();

    std::vector<std::vector<Terrain> > getMap(landscape_t);
};

class MapSquare
{
    private:
        std::vector<std::vector<Terrain> > tiles;
        int xSize;
        int ySize;
        bool occupied;
    public:
        MapSquare();
        ~MapSquare();
        std::vector<std::vector<Terrain> > getTiles();
        void setTiles(std::vector<std::vector<Terrain> >);
        bool isOccupied();
};
#endif
