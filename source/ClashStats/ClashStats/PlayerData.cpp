//
//  PlayerData.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "PlayerData.hpp"
#include "AttackData.hpp"
#include <assert.h>

PlayerData::PlayerData()
: m_SpecialFlag(kSpecialNone)
, m_PlayerName("")
, m_PlayerTag("")
, m_TownHallLevel(kTH0)
, m_CloserStars(0)
{
	
}

PlayerData::~PlayerData()
{
	m_Attacks.clear();
	m_Defends.clear();
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

eTownHallLevel PlayerData::GetTownHallLevel() const
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

void PlayerData::SetCloserStars(const int stars)
{
	assert(stars <= GetTotalStars() && "closer stars exceeds stars earned in attacks");
	m_CloserStars = stars;
}

const int PlayerData::GetCloserStars() const
{
	return m_CloserStars;
}

const int PlayerData::GetThreeStars() const
{
	int count = 0;
	for (int i = 0; i < m_Attacks.size(); ++i)
	{
		if (m_Attacks[i].GetStars() == MAX_STARS_PER_ATTACK)
		{
			++count;
		}
	}
	
	return count;
}

bool PlayerData::IsSalt() const
{
	return (m_SpecialFlag & kSpecialSalt);
}

bool PlayerData::IsClosed() const
{
	for (int i = 0; i < m_Defends.size(); ++i)
	{
		if ((int)m_Defends[i].GetStars() == AttackData::kThreeStar)
		{
			return true;
		}
	}
	
	return false;
}

void PlayerData::AddAttack(const AttackData *attack)
{
	m_Attacks.push_back(*attack);
}

void PlayerData::AddDefend(const AttackData *defend)
{
	m_Defends.push_back(*defend);
}

const std::vector<AttackData> PlayerData::GetAttacks() const
{
	return m_Attacks;
}

const std::vector<AttackData> PlayerData::GetDefends() const
{
	return m_Defends;
}

const unsigned long PlayerData::GetAttackCount() const
{
	return (unsigned long)m_Attacks.size();
}

const unsigned long PlayerData::GetDefendCount() const
{
	return (unsigned long)m_Defends.size();
}

const int PlayerData::GetBleeds() const
{
	return GetDefendCount() ? (int)GetDefendCount() - 1 : 0;
}

const int PlayerData::GetHolds() const
{
	return (MAX_STARS_PER_ATTACK - GetMaxStarsGiven());
}

const int PlayerData::GetNukes() const
{
	int count = 0;
	for (int i = 0; i < m_Defends.size(); ++i)
	{
		if (m_Defends[i].GetTownHall() > m_TownHallLevel)
		{
			++count;
		}
	}
	
	return count;
}

const int PlayerData::GetMaxStarsGiven() const
{
	int max = 0;
	for (int i = 0; i < m_Defends.size(); ++i)
	{
		if ((int)m_Defends[i].GetStars() > max)
		{
			max = (int)m_Defends[i].GetStars();
		}
	}
	
	return max;
}

const AttackData* PlayerData::GetCloserAttack() const
{
	const AttackData *ad = NULL;
	for (int i = 0; i < m_Defends.size(); ++i)
	{
		if (ad == NULL)
		{
			ad = &m_Defends[i];
		}
		else if (ad->GetStars() < m_Defends[i].GetStars())
		{
			ad = &m_Defends[i];
		}
	}
	
	return ad;
}

const int PlayerData::GetTotalStars() const
{
	int stars = 0;
	for (int i = 0; i < m_Attacks.size(); ++i)
	{
		stars += m_Attacks[i].GetStars();
	}
	
	return stars;
}
