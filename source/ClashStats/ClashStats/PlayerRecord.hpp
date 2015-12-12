//
//  PlayerRecord.hpp
//  ClashStats
//
//  Created by Atlas on 11/29/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef PlayerRecord_hpp
#define PlayerRecord_hpp

#include <string>
#include <vector>

struct DefendRecord;
struct AttackRecord;

struct PlayerRecord
{
	int			pk;
	std::string	playerTag;
	std::string playerName;
	
	std::vector<DefendRecord> defends;
	std::vector<AttackRecord> attacks;
};

#endif /* PlayerRecord_hpp */
