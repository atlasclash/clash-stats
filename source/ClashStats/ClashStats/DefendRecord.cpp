//
//  DefendRecord.cpp
//  ClashStats
//
//  Created by Atlas on 11/29/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "DefendRecord.hpp"

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
	
	return true;
}