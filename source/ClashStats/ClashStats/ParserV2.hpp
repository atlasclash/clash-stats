//
//  ParserV2.hpp
//  ClashStats
//
//  Created by Jeremy Jessup on 7/17/16.
//  Copyright © 2016 JTJ. All rights reserved.
//

#ifndef ParserV2_hpp
#define ParserV2_hpp

#include "Parser.hpp"

class WarData;

class ParserV2 : public Parser
{
public:
	ParserV2(const char *fileName);
	virtual ~ParserV2();
	
	virtual void ProcessWar(WarData *warData);
	
protected:
};

#endif /* ParserV2_hpp */
