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
	std::cout	<< opponentName
				<< " (" << opponentTag << ") "
				<< playerCount
				<< " Us:" << usScore
				<< " Them:" << themScore
				<< " Date: " << DATABASE::GetInstance().StringFromDate(date)
				<< std::endl;
}