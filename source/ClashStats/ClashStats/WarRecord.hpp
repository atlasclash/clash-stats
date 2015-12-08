//
//  WarRecord.hpp
//  ClashStats
//
//  Created by Atlas on 11/29/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef WarRecord_hpp
#define WarRecord_hpp

#include <string>

struct WarRecord
{
	int			pk;
	std::string	opponentName;
	std::string	opponentTag;
	int			playerCount;
	int			usScore;
	int			themScore;
	int			date;			// seconds from "epoch" (2010-Jan-01)
};
#endif /* WarRecord_hpp */
