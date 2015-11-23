//
//  Parser.hpp
//  ClashStats
//
//  Created by Atlas on 11/15/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>

class WarData;

class Parser
{
public:
	Parser(const char *fileName);
	virtual ~Parser();
	
	virtual void ProcessWar(WarData *warData) = 0;
	
protected:
	// returns a vector containing the tokenized line contents
	std::vector<std::string> GetCellsFromLine(const char token = ',');
	
	std::ifstream *m_dataFile;

};

#endif /* Parser_hpp */
