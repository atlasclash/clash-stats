//
//  DefendRecord.hpp
//  ClashStats
//
//  Created by Atlas on 11/29/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef DefendRecord_hpp
#define DefendRecord_hpp

#include <string>

struct DefendRecord
{
	bool isEqual(const DefendRecord d) const;
	
	int			pk;
	std::string	playerTagPk;
	int			playerTH;
	int			playerWgt;
	int			opponentTH;
	int			opponentWgt;
	int			starCount;
	int			percentDmg;
	int			warPk;
};

#endif /* DefendRecord_hpp */
