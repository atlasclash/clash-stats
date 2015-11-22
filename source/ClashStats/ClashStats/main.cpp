//
//  main.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include <iostream>
#include "ParserFactory.hpp"
#include "Parser.hpp"

int main(int argc, const char * argv[])
{

	PARSER::Instantiate();
	Parser *parser = PARSER::GetInstance().getParserForFile(argv[1]);
	if (parser == NULL)
	{
		std::cout << "Unable to parse file.\n";
		return -1;
	}
	
	parser->ProcessWar();
	
    return 0;
}
