//
//  PlayerData.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "PlayerData.hpp"

PlayerData::PlayerData()
: m_SpecialFlag(kSpecialNone)
, m_PlayerName("")
, m_PlayerTag("")
, m_TownHallLevel(kTH0)
{
	
}

PlayerData::~PlayerData()
{
	
}

void PlayerData::SetPlayerName(const std::string name)
{
	m_PlayerName = name;
}

std::string PlayerData::GetPlayerName() const
{
	return m_PlayerName;
}

void PlayerData::SetTownHallLevel(const eTownHallLevel lvl)
{
	m_TownHallLevel = lvl;
}

PlayerData::eTownHallLevel PlayerData::GetTownHallLevel() const
{
	return m_TownHallLevel;
}

void PlayerData::SetPlayerTag(const std::string tag)
{
	m_PlayerTag = tag;
}

std::string PlayerData::GetPlayerTag() const
{
	return m_PlayerTag;
}

void PlayerData::SetSpecialFlag(const eSpecialFlags flags)
{
	m_SpecialFlag = flags;
}

bool PlayerData::IsSalt() const
{
	return (m_SpecialFlag & kSpecialNone);
}

void PlayerData::AddAttack(const AttackData *attack)
{
	m_Attacks.push_back(*attack);
}

void PlayerData::AddDefend(const AttackData *defend)
{
	m_Defends.push_back(*defend);
}

