//
//  AttackData.cpp
//  ClashStats
//
//  Created by Atlas on 11/21/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "AttackData.hpp"

AttackData::AttackData(int opponent, StarType stars, int pctDmg, eTownHallLevel th)
: m_OpponentId(opponent)
, m_Stars(stars)
, m_PctDamage(pctDmg)
, m_TownHall(th)
{
	
}

