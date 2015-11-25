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
#include "WarData.hpp"

int main(int argc, const char * argv[])
{

	PARSER::Instantiate();
	Parser *parser = PARSER::GetInstance().getParserForFile(argv[1]);
	if (parser == NULL)
	{
		std::cout << "Unable to parse file.\n";
		return -1;
	}
	
	WarData *p_warData = new WarData();
	parser->ProcessWar(p_warData);
	
	p_warData->CalcCloserStars();
	p_warData->RunReports();
	
    return 0;
}
