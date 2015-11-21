//
//  main.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include <iostream>
#include "ParserFactory.hpp"

int main(int argc, const char * argv[])
{
	std::cout << "Hello, World!\n";
	PARSER::Instantiate();
	PARSER::GetInstance().getParserForFile(argv[1]);
	
    return 0;
}
