//
//  Constants.cpp
//  ClashStats
//
//  Created by Jeremy Jessup on 9/5/16.
//  Copyright © 2016 JTJ. All rights reserved.
//

#include <stdio.h>
#include "Constants.h"

int MAX_STARS_PER_WAR   = 6;
int MAX_PLAYER_ATTACKS  = 2;

int GetMaxTHWeight(const int thLvl)
{
	switch (thLvl)
	{
        case kTH12:         return 120;
		case kTH11:			return 110;
		case kTH10:			return 90;
		case kTH9:			return 70;
		case kTH8:			return 60;
		
		case kTH7:
		default:			return 50;
	}
}

int GetMinTHWeight(const int thLvl)
{
	switch (thLvl)
	{
        case kTH12:         return 110;
		case kTH11:			return 100;
		case kTH10:			return 80;
		case kTH9:			return 60;
		case kTH8:			return 50;
		
		case kTH7:
		default:			return 0;
	}
}

void SetMaxPlayerAttacks(const int maxAttacks)
{
    MAX_PLAYER_ATTACKS = maxAttacks;
}

int GetMaxPlayerAttacks()
{
    return MAX_PLAYER_ATTACKS;
}

void SetMaxStarsPerWar(const int maxStarsPerWar)
{
    MAX_STARS_PER_WAR = maxStarsPerWar;
}

int GetMaxStarsPerWar()
{
    return MAX_STARS_PER_WAR;
}
