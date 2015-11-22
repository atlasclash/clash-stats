//
//  ParserV1.hpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef ParserV1_hpp
#define ParserV1_hpp

#include <stdio.h>
#include "Parser.hpp"

class ParserV1 : public Parser
{
public:
	ParserV1(const char *fileName);
	virtual ~ParserV1();
	
	virtual void ProcessWar();
	
protected:
};

#endif /* ParserV1_hpp */
