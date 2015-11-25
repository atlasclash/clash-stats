//
//  WarData.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "WarData.hpp"
#include <assert.h>
#include <iostream>

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

void WarData::RunReports() const
{
	// Verification
	ReportFinalScore();
	
	
	// Warnings
	ReportWarningMissingInAction();
}

void WarData::ReportFinalScore() const
{
	int usScore = 0;
	int themScore = 0;
	
	int usTHScore[PlayerData::kTH11];
	memset(usTHScore, 0, sizeof(int)*PlayerData::kTH11);
	
	int themTHScore[PlayerData::kTH11];
	memset(themTHScore, 0, sizeof(int)*PlayerData::kTH11);
	
	// assumption is that these lists are the same length
	for (int i = 0; i < m_UsList.size(); ++i)
	{
		int usStarsGiven = m_UsList[i].GetMaxStarsGiven();
		int themStarsGiven = m_ThemList[i].GetMaxStarsGiven();
		
		themScore	+= usStarsGiven;
		usScore		+= themStarsGiven;
		
		themTHScore[m_UsList[i].GetTownHallLevel()-1]		+= usStarsGiven;
		usTHScore[m_ThemList[i].GetTownHallLevel()-1]		+= themStarsGiven;
	}
	
	std::cout << "Final Score:" << std::endl;
	std::cout << "  Us: " << usScore << std::endl;
	std::cout << "  Them: " << themScore << std::endl;
	
	std::cout << "Stars earned per TH level" << std::endl;
	for (int i = PlayerData::kTH6-1; i < PlayerData::kTH11; ++i)
	{
		std::cout << "TH(" << i+1 << ") Us: " << usTHScore[i] << " Them: " << themTHScore[i] << std::endl;
	}
}

void WarData::ReportWarningMissingInAction() const
{
	for (int i = 0; i < m_UsList.size(); ++i)
	{
		if (m_UsList[i].GetAttackCount() < MAX_PLAYER_ATTACKS)
		{
			std::cout << "MIA: " << m_UsList[i].GetPlayerName() << " attacks " << m_UsList[i].GetAttackCount() << std::endl;
		}
	}
}