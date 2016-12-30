//
//  WarRecord.cpp
//  ClashStats
//
//  Created by Atlas on 11/29/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "WarRecord.hpp"
#include "Database.hpp"
#include <iostream>

void WarRecord::Description()
{
	std::cout	<< "[" << pk << "] "
				<< opponentName
				<< " (" << opponentTag << ") "
				<< " vs "
				<< usName
				<< " (" << usTag << ") "
				<< playerCount
				<< " Us:" << usScore
				<< " Them:" << themScore
				<< " Date: " << DATABASE::GetInstance().StringFromDate(date)
				<< std::endl;
}

WarRecord& WarRecord::operator=(const WarRecord &rhs)
{
	pk				= rhs.pk;
	userMeta		= rhs.userMeta;
	opponentName	= rhs.opponentName;
	opponentTag		= rhs.opponentTag;
	usName			= rhs.usName;
	usTag			= rhs.usTag;
	playerCount		= rhs.playerCount;
	usScore			= rhs.usScore;
	themScore		= rhs.themScore;
	date			= rhs.date;			// seconds from "epoch" (2010-Jan-01)
	
	return *this;
}