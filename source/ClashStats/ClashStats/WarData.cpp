//
//  WarData.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "WarData.hpp"
#include <assert.h>

WarData::WarData(std::string clanName, std::string clanTag)
{
	m_OpponentClanName	= clanName;
	m_OpponentClanTag	= clanTag;
}

WarData::~WarData()
{
	m_UsList.clear();
	m_ThemList.clear();
}

void WarData::AddUsPlayer(const PlayerData *us)
{
	m_UsList.push_back(*us);
}

void WarData::AddThemPlayer(const PlayerData *them)
{
	m_ThemList.push_back(*them);
}

const int WarData::GetUsPlayerCount() const
{
	return (int)m_UsList.size();
}

const int WarData::GetThemPlayerCount() const
{
	return (int)m_ThemList.size();
}

void WarData::AddClanAttack(const AttackData *ad, const int attackerIndex)
{
	assert(attackerIndex-1 >= 0 && attackerIndex-1 < m_UsList.size());
	m_UsList[attackerIndex-1].AddAttack(ad);
}

void WarData::AddClanDefend(const AttackData *def, const int defenderIndex)
{
	assert(defenderIndex-1 >= 0 && defenderIndex-1 < m_ThemList.size());
	m_UsList[defenderIndex-1].AddDefend(def);
}

void WarData::AddThemAttack(const AttackData *ad, const int attackerIndex)
{
	assert(attackerIndex-1 >= 0 && attackerIndex-1 < m_ThemList.size());
	m_ThemList[attackerIndex-1].AddAttack(ad);
}

void WarData::AddThemDefend(const AttackData *def, const int defenderIndex)
{
	assert(defenderIndex-1 >= 0 && defenderIndex-1 < m_UsList.size());
	m_ThemList[defenderIndex-1].AddDefend(def);
}