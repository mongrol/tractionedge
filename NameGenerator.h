/*
 *  NameGenerator.h
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 20/12/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#ifndef __NAMEGENERATOR_H__
#define __NAMEGENERATOR_H__

#include <string>
#include <vector>

class NameGenerator {
private:
    std::vector<std::string> firstNames;
    std::vector<std::string> surnames;
public:
    NameGenerator();
    ~NameGenerator();
    std::string name();
};

#endif
