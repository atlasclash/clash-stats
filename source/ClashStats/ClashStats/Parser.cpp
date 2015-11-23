//
//  Parser.cpp
//  ClashStats
//
//  Created by Atlas on 11/15/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "Parser.hpp"
#include <sstream>
#include "StringHelpers.hpp"

Parser::Parser(const char *fileName)
{
	m_dataFile = new std::ifstream(fileName);
}

Parser::~Parser()
{
	delete m_dataFile;
}

std::vector<std::string> Parser::GetCellsFromLine(const char token)
{
	std::string line;
	std::vector<std::string> cellResults;

	if (std::getline(*m_dataFile, line))
	{
		std::stringstream lineStream(line);
		std::string cell;
		
		while (std::getline(lineStream, cell, token))
		{
			cellResults.push_back(reduce(cell));
		}
	}
	
	return cellResults;
}
