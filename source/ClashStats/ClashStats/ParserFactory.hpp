//
//  ParserFactory.hpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef ParserFactory_hpp
#define ParserFactory_hpp

#include <stdio.h>
#include "Singleton.h"
#include "Parser.hpp"

class ParserFactory
{
public:
	Parser* getParserForFile(const char *fileName);
	
protected:
};

typedef atSingleton<ParserFactory> PARSER;

#endif /* ParserFactory_hpp */
