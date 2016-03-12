//
//  PlayerWarRecord.hpp
//  ClashStats
//
//  Created by Atlas on 12/8/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef PlayerWarRecord_hpp
#define PlayerWarRecord_hpp

#include <string>

struct PlayerWarRecord
{
	bool isEqual(const PlayerWarRecord rec) const;
	
	int			pk;
	std::string	playerTagKey;
	int			playerTH;
	int			warKey;
	int			closerStars;
	int			holds;
	int			bleeds;
	int			nuked;
	int			totalStars;
	int			threeStars;
};

#endif /* PlayerWarRecord_hpp */
