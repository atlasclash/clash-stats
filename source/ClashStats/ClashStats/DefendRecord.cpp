//
//  DefendRecord.cpp
//  ClashStats
//
//  Created by Atlas on 11/29/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "DefendRecord.hpp"
#include "Constants.h"

bool DefendRecord::isEqual(const DefendRecord d) const
{
	if (pk != d.pk)
		return false;
	
	if (playerTagPk.compare(d.playerTagPk) != 0)
		return false;
	
	if (playerTH != d.playerTH)
		return false;
	
	if (opponentTH != d.opponentTH)
		return false;
	
	if (starCount != d.starCount)
		return false;
	
	if (percentDmg != d.percentDmg)
		return false;
	
	if (warPk != d.warPk)
		return false;
	
	if (playerWgt != d.playerWgt)
		return false;
	
	if (opponentWgt != d.opponentWgt)
		return false;
	
	return true;
}

bool DefendRecord::isPeerDefend() const
{
	const int minWgt = GetMinTHWeight(playerTH);
	const int maxWgt = GetMaxTHWeight(playerTH);
	
	if (opponentWgt <= maxWgt && opponentWgt >= minWgt)
	{
		return true;
	}
	
	return false;
}

bool DefendRecord::isNukeDefend() const
{
	const int maxWgt = GetMaxTHWeight(playerTH);
	
	if (opponentWgt > maxWgt)
	{
		return true;
	}
	
	return false;
}

bool DefendRecord::isSnipeDefend() const
{
	const int minWgt = GetMinTHWeight(playerTH);
	
	if (opponentWgt < minWgt)
	{
		return true;
	}
	
	return false;
}
