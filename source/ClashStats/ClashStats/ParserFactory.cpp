//
//  ParserFactory.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "ParserFactory.hpp"
#include "StringHelpers.hpp"
#include "ParserV1.hpp"
#include "ParserV2.hpp"
#include "Constants.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#define VERSION_ONE		    (1)
#define VERSION_TWO		    (2)
#define CWL_MODE            ("CWL")
#define REGULAR_WAR_MODE    ("REG")
#define RESERVED            ("reserved")

Parser* ParserFactory::getParserForFile(const char *fileName)
{
	std::ifstream dataFile(fileName);
	if (dataFile.fail())
	{
		return NULL;
	}
	
	std::string line;
	std::getline(dataFile, line);
	std::stringstream lineStream(line);
	std::string cell;
	
	std::vector<std::string> lineData;
	while (std::getline(lineStream, cell, ','))
	{
		lineData.push_back(reduce(cell));
	}
	
	if (atoi(lineData[1].c_str()) == VERSION_ONE)
	{
		return new ParserV1(fileName);
	}
	else if (atoi(lineData[1].c_str()) == VERSION_TWO)
	{
		return new ParserV2(fileName);
	}
    
    if (lineData[2].c_str() == CWL_MODE)
    {
        SetMaxStarsPerWar(3);
        SetMaxPlayerAttacks(1);
    }
    else if (lineData[2].c_str() == REGULAR_WAR_MODE ||
             lineData[2].c_str() == RESERVED)
    {
        SetMaxStarsPerWar(6);
        SetMaxPlayerAttacks(2);
    }

	return NULL;
}


