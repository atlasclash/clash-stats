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
, m_IsSalt(false)
, m_IsClose(false)
, m_AttemptNumber(0)
{
}

AttackData::AttackData(int opponent, StarType stars, int pctDmg, eTownHallLevel th, bool isSalt, bool isClose, int attempt)
: m_OpponentId(opponent)
, m_Stars(stars)
, m_PctDamage(pctDmg)
, m_TownHall(th)
, m_IsSalt(isSalt)
, m_IsClose(isClose)
, m_AttemptNumber(attempt)
{
	
}

