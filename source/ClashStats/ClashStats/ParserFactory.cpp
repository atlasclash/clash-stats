//
//  ParserFactory.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "ParserFactory.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <limits>
#include "StringHelpers.hpp"

Parser* ParserFactory::getParserForFile(const char *fileName)
{
	std::ifstream dataFile(fileName);
	
	std::string line;
	std::getline(dataFile, line);
	std::stringstream lineStream(line);
	std::string cell;
	
	std::vector<std::string> lineData;
	while (std::getline(lineStream, cell, ','))
	{
		lineData.push_back(reduce(cell));
	}
	
	
	printf("hello world!\n");
	return NULL;
}


