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
#include "Constants.h"

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


bool Parser::CheckDataRanges(const int opponentId, const int usId, const int stars, const int pct, const int warSize) const
{
	if (opponentId < MIN_ATTACKER_ID || opponentId > warSize)
		return false;
	
	if (usId < MIN_ATTACKER_ID || usId > warSize)
		return false;
	
	if (stars < MIN_STARS_PER_ATTACK || stars > MAX_STARS_PER_ATTACK)
		return false;
	
	if (pct < MIN_PCT_DAMAGE || pct > MAX_PCT_DAMAGE)
		return false;
    
    if (stars == 2 && pct < 50)
        return false;
	
    // special checks for Clan War Leagues
    if (GetWarMode() == kCWL)
    {
        
    }
    
	return true;
}
