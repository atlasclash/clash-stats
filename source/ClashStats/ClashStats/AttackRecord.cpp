//
//  AttackRecord.cpp
//  ClashStats
//
//  Created by Atlas on 11/29/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "AttackRecord.hpp"

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
	
	return true;
}