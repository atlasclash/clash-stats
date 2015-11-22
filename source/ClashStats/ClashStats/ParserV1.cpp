//
//  ParserV1.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "ParserV1.hpp"
#include <sstream>
#include <string>
#include <vector>
#include "StringHelpers.hpp"

ParserV1::ParserV1(const char *fileName) : Parser(fileName)
{
	
}

ParserV1::~ParserV1()
{
	
}

void ParserV1::ProcessWar()
{
	if (m_dataFile == NULL)
	{
		return;
	}
	
	std::string line;
	
	// retrieve a single line from the file
	std::getline(*m_dataFile, line);
	std::stringstream lineStream(line);
	std::string cell;
	
	std::vector<std::string> lineData;
	
	// break the line into each cell
	while (std::getline(lineStream, cell, ','))
	{
		lineData.push_back(reduce(cell));
	}
	
	printf("linedata = %lu", lineData.size());
}