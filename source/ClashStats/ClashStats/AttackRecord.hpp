//
//  AttackRecord.hpp
//  ClashStats
//
//  Created by Atlas on 11/29/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef AttackRecord_hpp
#define AttackRecord_hpp

#include <string>

struct AttackRecord
{
	int			pk;
	std::string	playerTagPk;
	int			playerTH;
	int			opponentTH;
	int			starCount;
	int			percentDmg;
	bool		isSalt;
	bool		isClose;
	int			attemptNum;
	int			attackNum;
	int			warPk;
};

#endif /* AttackRecord_hpp */
