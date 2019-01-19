//
//  AttackRecord.cpp
//  ClashStats
//
//  Created by Atlas on 11/29/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "AttackRecord.hpp"
#include "Constants.h"

bool AttackRecord::isEqual(const AttackRecord a) const
{
	if (pk != a.pk)
		return false;
	
	if (playerTagPk.compare(a.playerTagPk) != 0)
		return false;
	
	if (playerTH != a.playerTH)
		return false;
	
	if (opponentTH != a.opponentTH)
		return false;
	
	if (starCount != a.starCount)
		return false;
	
	if (percentDmg != a.percentDmg)
		return false;
	
	if (isSalt != a.isSalt)
		return false;
	
	if (isClose != a.isClose)
		return false;
	
	if (attemptNum != a.attemptNum)
		return false;
	
	if (attackNum != a.attackNum)
		return false;
	
	if (warPk != a.warPk)
		return false;
	
	if (playerWgt != a.playerWgt)
		return false;
	
	if (opponentWgt != a.opponentWgt)
		return false;
	
	return true;
}

bool AttackRecord::isPeerAttack() const
{
	const int minWgt = GetMinTHWeight(playerTH);
	const int maxWgt = GetMaxTHWeight(playerTH);
	
	if ((opponentWgt <= maxWgt && opponentWgt >= minWgt) || opponentTH == playerTH)
	{
		return true;
	}
	
	return false;
}

bool AttackRecord::isSnipeAttack() const
{
	const int maxWgt = GetMaxTHWeight(playerTH);
	
	if (opponentWgt > maxWgt)
	{
		return true;
	}
	
	return false;
}

bool AttackRecord::isNukeAttack() const
{
	const int minWgt = GetMinTHWeight(playerTH);
	
	if (opponentWgt < minWgt)
	{
		return true;
	}
	
	return false;
}

float AttackRecord::CalculateQualityStars() const
{
	if (isPeerAttack())
	{
		return (float)(starCount - 0.1f * (playerWgt - opponentWgt));
	}
	else if (isNukeAttack())
	{
		return (float)(starCount - (playerTH - opponentTH));
	}
	else if (isSnipeAttack())
	{
		return (float)(starCount + (opponentTH - playerTH));
	}
	
	return 0.0f;
}

