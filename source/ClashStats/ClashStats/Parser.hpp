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

class Parser
{
public:
	Parser(const char *fileName);
	virtual ~Parser();
	
	virtual void ProcessWar() = 0;
	
protected:
	std::ifstream *m_dataFile;

};

#endif /* Parser_hpp */
