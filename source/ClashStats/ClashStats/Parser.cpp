//
//  Parser.cpp
//  ClashStats
//
//  Created by Atlas on 11/15/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "Parser.hpp"


Parser::Parser(const char *fileName)
{
	m_dataFile = new std::ifstream(fileName);
}

Parser::~Parser()
{
	delete m_dataFile;
}