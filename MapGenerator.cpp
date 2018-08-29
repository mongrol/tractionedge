/*
 *  Map.cpp
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 20/11/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#include <string>
#include "defines.h"
#include "MapGenerator.h"
#include "Logger.h"
#include "tinyxml.h"
#include "Utility.h"

Building::Building(){}

Building::~Building(){}

Building::Building(std::vector<std::vector<Terrain> > tileMap, int size)
{
    tiles = tileMap;
}

int Building::getWidth()
{
    //std::cout << "tiles array size:" << tiles.size() << std::endl;
    int width=tiles[0].size()+1;
    return width;
}

int Building::getHeight()
{
    int height=tiles.size()+1;
    return height;
}

MapGenerator::MapGenerator(){}

MapGenerator::~MapGenerator(){}

std::vector<std::vector<Terrain> > MapGenerator::getMap(landscape_t type)
{
    LOG << "Creating random map";
    Utility tool;

    //create empty map
    std::vector<std::vector<Terrain> > map;
    map.clear();
    map.resize(MAPHEIGHT);
    for (int i=0; i < MAPHEIGHT; i++)
        map[i].resize(MAPWIDTH);

    //divide it into squares. Each square will hold its terrain tiles and a
    //flag on whether its occupied or not.
    int const xSquares = MAPWIDTH/MAP_SQUARE_SIZE;
    int const ySquares = MAPHEIGHT/MAP_SQUARE_SIZE;
    std::vector<std::vector<MapSquare> > squares;
    squares.resize(ySquares);
    for (int i=0; i < ySquares; i++) squares[i].resize(xSquares);
    std::map<int,int> buildingCountMap; 
    
    //load parameter file for landscape type
    switch(type){
        case LANDSCAPE_FARM:{
            std::string file="farm.xml";
            buildingCountMap = loadParameters(file);
            //start to create map
            //first fill it with grass
            for (int y=0;y < map.size(); y++){
                for (int x=0;x < MAPWIDTH; x++){
                    Terrain terrain(GRASS_S);
                    terrain.isVisible=false;
                    map[y][x] = terrain;
                }
            }
            //ask for a forest
            map = createForest(map);
            //ask for a road
            map = createRoad(map);
            
            break;
        }
        default:
        break;
    }

    //We now have map of building vectors and another map stating the number of
    //buildings we need of each size. Now to place them.
    //firstly iterate through the list of building counts;
    std::map<int,int>::iterator it;
    for (it=buildingCountMap.begin(); it!=buildingCountMap.end();it++){
        //initilise 
        int squareX = 0;
        int squareY = 0;
        
        //pick a vector of source buildings.
        int size = it->first;
        int count = it->second;
        std::vector<Building*> blds = buildingMap[size];
        //std::cout << "Starting placement of size: " << size << " Placing " << count << " buildings" <<std::endl;

        //iterate until we reach our building count.
        for (int i=0;i<count;i++){
            //pick one at random.
            int result = tool.randNum(0,blds.size());
            
            if (size==5){
                do {
                    //find a single square thats unoccupied.
                    squareX = tool.randNum(1,7);
                    squareY = tool.randNum(1,5);
                    //std::cout << "MapGen choosing square:" << squareX << "x" <<squareY << std::endl;
                } while (squares[squareY][squareX].isOccupied());
                //copy in the buildings terrain data.
                squares[squareY][squareX].setTiles(blds[result]->tiles);
            }
            
            if (size==10){
                
                do {
                    //find 4 squares in a cube near the middle thats unoccupied.
                    squareX = tool.randNum(3,7);
                    squareY = tool.randNum(1,4);
                    //std::cout << "MapGen choosing 10x start square:" << squareX << "x" <<squareY << std::endl;
                } while (squares[squareY][squareX].isOccupied() || squares[squareY][squareX+1].isOccupied() || squares[squareY+1][squareX].isOccupied() || squares[squareY+1][squareX+1].isOccupied());
                LOG << "Found a place for 10x building";
                
                //carve out the 5x5 chunks
                std::vector<std::vector<Terrain> > tiles;
                tiles.resize(5);
                for(int i=0; i<5; i++){
                    tiles[i].resize(5);
                }
                
                //carve top left quadrant;
                for (int y=0;y < 5; y++){
                    for (int x=0; x < 5; x++){
                        tiles[y][x] = blds[result]->tiles[y][x];
                    }
                }
                squares[squareY][squareX].setTiles(tiles);
                
                //carve top right quadrant;
                for (int y=0;y < 5; y++){
                    for (int x=0; x < 5; x++){
                        tiles[y][x] = blds[result]->tiles[y][x+5];
                    }
                }
                squares[squareY][squareX+1].setTiles(tiles);

                //carve bottom left quadrant;
                for (int y=0;y < 5; y++){
                    for (int x=0; x < 5; x++){
                        tiles[y][x] = blds[result]->tiles[y+5][x];
                    }
                }
                squares[squareY+1][squareX].setTiles(tiles);

                //carve bottom right quadrant;
                for (int y=0;y < 5; y++){
                    for (int x=0; x < 5; x++){
                        tiles[y][x] = blds[result]->tiles[y+5][x+5];
                    }
                }
                //stick them in a building
                squares[squareY+1][squareX+1].setTiles(tiles);
            }
        }
    }

    //iterate squares and draw the occupied ones
    for (int y=0;y<squares.size();y++){
        for (int x=0;x<squares[y].size();x++){
            if (squares[y][x].isOccupied()){
                //std::cout << "Square occupied at: " << x <<"x"<< y << std::endl;
                std::vector<std::vector<Terrain> > tiles=squares[y][x].getTiles();
                for (int tY=0; tY<MAP_SQUARE_SIZE;tY++){
                    for (int tX=0; tX<MAP_SQUARE_SIZE;tX++){
                        if (tiles[tY][tX].gfxId!=BLANK_S) map[y*MAP_SQUARE_SIZE+tY][x*MAP_SQUARE_SIZE+tX]=tiles[tY][tX];
                    }
                }
            }
        }
    }


    return map;
}

std::vector<std::vector<Terrain> > MapGenerator::createForest(std::vector<std::vector<Terrain> > map) 
{
    Utility tool;
    //We're going to use cellular automata to produce clumps of trees
    //first spray random trees around.
    for (int y=0;y < map.size(); y++){
        for (int x=0;x < MAPWIDTH; x++){
            int roll=tool.rollDice(1,100);
            if (roll<30){
                Terrain terrain(TREE_S);
                terrain.isVisible=false;
                map[y][x] = terrain;
            }
        }
    }
    //we then iterate each tile on the map using the 4/5 rule.
    //In every 3x3 block. If there's 4 tiles then the centre one is a tree
    //If there's less than 4 tiles then the centre isn't a tree.
    int iterations = 5;
    for (int i=0;i<=iterations;i++){
        int step = 2;
        for (int y=1;y < map.size()-1; y=y+step){
            for (int x=1;x < MAPWIDTH-1; x=x+step){
                int tilesSet=0;
                //iterate the 3x3 block and add 1 to tileSet for every tree found
                for (int yy=y-1;yy<y+2;yy++){
                    for (int xx=x-1;xx<x+2;xx++){
                        if (map[yy][xx].gfxId==TREE_S) tilesSet++;
                        //std::cout << "map["<<yy<<"]["<<xx<<"] is:" << map[yy][xx].gfxId << std::endl;
                    }
                }
                // if we have more than 4 trees. Make the centre tile a tree.
                if (tilesSet>=5){
                    Terrain terrain(TREE_S);
                    terrain.isVisible=false;
                    map[y][x]=terrain;
                } else {
                    Terrain terrain(GRASS_S);
                    terrain.isVisible=false;
                    map[y][x]=terrain;
                }
                //std::cout << "Checking CA at map["<<y<<"]["<<x<<"] Setting tile to " << map[y][x].gfxId << std::endl;
                int roll = tool.randNum(1,3);
                if (roll == 1) {
                    step == 1;
                } else {
                    step == 2;
                }                
            }
        }
    }
    return map;
}

std::vector<std::vector<Terrain> > MapGenerator::createRoad(std::vector<std::vector<Terrain> > map)
{
    Utility tool;
    //Firstly we'll try and divert a straight horizontal road
    int x = 0;
    int y = MAPHEIGHT/2;
    
    for (x=0; x<MAPWIDTH;++x){
        //lets try and vary the Y value of the road as we move along.
        int roll = tool.randNum(-1,2);
        y=y+roll;
        //std::cout << "roadY= " << y <<std::endl;
        //then we draw a wide road.
        if (y>=0 && y<MAPHEIGHT){
            Terrain terrain(GRASS_S);
            terrain.isVisible=false;
            map[y][x]=terrain;
        }
        if (y+1>=0 && y+1<MAPHEIGHT){
            Terrain terrain(DIRT_S);
            terrain.isVisible=false;
            map[y+1][x]=terrain;
        }
        if (y+2>=0 && y+2<MAPHEIGHT){
            Terrain terrain(DIRT_S);
            terrain.isVisible=false;
            map[y+2][x]=terrain;
        }
        if (y+3>=0 && y+3<MAPHEIGHT){
            Terrain terrain(DIRT_S);
            terrain.isVisible=false;
            map[y+3][x]=terrain;
        }
        if (y+4>=0 && y+4<MAPHEIGHT){
            Terrain terrain(GRASS_S);
            terrain.isVisible=false;
            map[y+4][x]=terrain;
        }
    }

    return map;
}

std::map<int,int>  MapGenerator::loadParameters(std::string file)
{
    //load landscape parameter file
    Utility tool;
    //std::string paramURL = "maps/map1.xml";
    std::string paramURL = "data/landscapes/"+file;
    TiXmlDocument paramFile(paramURL.c_str());
    bool loadOkay = paramFile.LoadFile();
    if (loadOkay)
    {
        //std::cout << "MapGen loaded " << paramURL << std::endl; 
    }
    else
    {
        //std::cout << "MapGen failed to load " << paramURL << std::endl; 
        exit(0);
    }

    //parse tile files. We create buildings from each file.
    TiXmlHandle fileHandle(&paramFile);
    for(TiXmlElement * itemNode=fileHandle.FirstChild("file").ToElement(); itemNode; itemNode=itemNode->NextSiblingElement("file") )
    {
        std::string fileName=itemNode->Attribute("name");
        int size,items;
        itemNode->QueryValueAttribute("size", &size);
        itemNode->QueryValueAttribute("items", &items);
        loadBuildingFile(fileName, size, items);        
    }

    //parse building count. This is the number of buildings of each size.
    //Put them in a map to return
    std::map<int,int> buildingCountMap;
    for(TiXmlElement * itemNode=fileHandle.FirstChild("building").ToElement(); itemNode; itemNode=itemNode->NextSiblingElement("building") )
    {
        int size,count;
        itemNode->QueryValueAttribute("size", &size);
        itemNode->QueryValueAttribute("count", &count);
        buildingCountMap[size]=count;
    }
    
    LOG << "Done loading params";
    return buildingCountMap;
}

void MapGenerator::loadBuildingFile(const std::string fileName, const int size, const int items)
{
    //loads building tmx files and returns a vector of building objects;
    Utility tool;

    std::vector<Building*> buildings;
    std::vector<std::vector<Terrain> > tiles;
    tiles.clear();
    tiles.resize(size);
    for (int i=0; i < size; i++)
        tiles[i].resize(size);

    //load building tmx file
    std::string fileURL = "data/landscapes/"+fileName;
    TiXmlDocument file(fileURL.c_str());
    bool loadOkay = file.LoadFile();
    if (loadOkay)
    {
        //std::cout << "loaded " << fileURL << std::endl;
        std::cout << "tileFile loading file: " << fileURL << " size: " << size << std::endl;
    }
    else
    {
        std::cout << "loadBuildingFile(): failed to load " << fileURL << std::endl;
        exit(0);
    }

    TiXmlHandle mapHandle(&file);

    TiXmlElement * tileElement=mapHandle.FirstChild("map").FirstChild("layer").FirstChild("data").FirstChild("tile").ToElement();

    for (int a=0;a < items;a++){
        for (int j=0;j < size; j++){
            for (int i=0;i < size; i++){
                if (tileElement){
                    //std::cout << "gid id=" << tileElement->Attribute("gid") << std::endl;
                    int terrainId=tool.stringToInt(tileElement->Attribute("gid"));
                    Terrain terrain(terrainId-1);
                    terrain.isVisible=false;
                    tiles[j][i] = terrain;
                    //std::cout << tiles[j][i].gfxId;
                    tileElement=tileElement->NextSiblingElement();
                }
            }
        }
        Building * building = new Building(tiles, size);
        buildings.push_back(building);
    }
    //std::cout << "Buildings: " << size << " contains " << buildings.size() << std::endl;
    buildingMap[size]=buildings;

    return;
}

std::vector<std::vector<Terrain> > MapGenerator::getBuilding()
{
    //start off by creating a square block house with no door.
    std::vector<std::vector<Terrain> > map;
    map.clear();
    map.resize(6);
    for (int i=0; i < 6; i++)
        map[i].resize(6);

    //do the walls
    for (int y=0;y < map.size(); y++){
        for (int x=0;x < map[y].size(); x++){
            Terrain terrain(STONE_WALL_S);
            terrain.isVisible=false;
            map[y][x] = terrain;
            //std::cout << map[y][x].gfxId;
        }
    }
    return map;
}

MapSquare::MapSquare()
{
    occupied = false;
    tiles.clear();
    tiles.resize(MAP_SQUARE_SIZE);
    for (int i=0; i < MAP_SQUARE_SIZE; i++)
        tiles[i].resize(MAP_SQUARE_SIZE);
}

MapSquare::~MapSquare(){}

void MapSquare::setTiles(std::vector<std::vector<Terrain> > building)
{
    //copies building terrain into mapSquare
    for(int y=0;y<MAP_SQUARE_SIZE;y++){
        for(int x=0;x<MAP_SQUARE_SIZE;x++){
            tiles[y][x] = building [y][x];
        }
    }
    occupied = true;
}

std::vector<std::vector<Terrain> > MapSquare::getTiles()
{
    return tiles;
}

bool MapSquare::isOccupied()
{
    return occupied;
}
