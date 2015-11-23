//
//  WarData.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "WarData.hpp"

WarData::WarData(std::string clanName, std::string clanTag)
{
	m_OpponentClanName	= clanName;
	m_OpponentClanTag	= clanTag;
}

void WarData::AddUsPlayer(const PlayerData *us)
{
	m_UsList.push_back(*us);
}

void WarData::AddThemPlayer(const PlayerData *them)
{
	m_ThemList.push_back(*them);
}

void WarData::AddAttack(const AttackData *ad, const int attackerIndex)
{
	m_UsList[attackerIndex].AddAttack(ad);
}

void WarData::AddDefend(const AttackData *def, const int defenderIndex)
{
	m_UsList[defenderIndex].AddDefend(def);
}
