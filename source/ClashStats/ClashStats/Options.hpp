//
//  Options.hpp
//  ClashStats
//
//  Created by Atlas on 11/26/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef Options_hpp
#define Options_hpp

#include "Singleton.h"

class Options
{
public:
	bool parser_Check_Data_Ranges;			// pct [0-100], stars [0-3]
	bool parser_Check_Player_Totals;		// player stars <= 6, player attacks <=2
	bool parser_Check_Missing_Attacks;		// if a player has not been attacked

protected:
	Options();
	virtual ~Options() {}
};

typedef atSingleton<Options> OPTIONS;

#endif /* Options_hpp */
