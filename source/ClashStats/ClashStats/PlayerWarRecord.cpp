//
//  PlayerWarRecord.cpp
//  ClashStats
//
//  Created by Atlas on 12/8/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "PlayerWarRecord.hpp"

bool PlayerWarRecord::isEqual(const PlayerWarRecord rec) const
{
	if (pk != rec.pk)
		return false;
	
	if (playerTagKey.compare(rec.playerTagKey) != 0)
		return false;
	
	if (playerTH != rec.playerTH)
		return false;
	
	if (warKey != rec.warKey)
		return false;
	
	if (closerStars != rec.closerStars)
		return false;
	
	if (holds != rec.holds)
		return false;
	
	if (bleeds != rec.bleeds)
		return false;
	
	if (nuked != rec.nuked)
		return false;
	
	if (totalStars != rec.totalStars)
		return false;
	
	if (threeStars != rec.threeStars)
		return false;
	
	return true;
}