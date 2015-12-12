//
//  HistoricRecord.hpp
//  ClashStats
//
//  Created by Atlas on 11/29/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef HistoricRecord_hpp
#define HistoricRecord_hpp

#include <string>

struct HistoricRecord
{
	int			pk;
	std::string	playerTagPk;
	int			warTotal;
	int			closerStars;
	int			holds;
	int			bleeds;
	int			nuked;
	int			starsTotal;
	int			mia;
	int			scouts;
};

#endif /* HistoricRecord_hpp */
