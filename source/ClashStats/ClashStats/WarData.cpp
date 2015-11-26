//
//  WarData.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "WarData.hpp"
#include "Options.hpp"
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

void WarData::AddUsAttack(const AttackData *ad, const int attackerIndex)
{
	assert(attackerIndex-1 >= 0 && attackerIndex-1 < m_UsList.size());
	m_UsList[attackerIndex-1].AddAttack(ad);
}

void WarData::AddUsDefend(const AttackData *def, const int defenderIndex)
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

eTownHallLevel WarData::GetUsTHLevel(const int usId) const
{
	return m_UsList[usId-1].GetTownHallLevel();
}

eTownHallLevel WarData::GetThemTHLevel(const int themId) const
{
	return m_ThemList[themId-1].GetTownHallLevel();
}

void WarData::CalcCloserStars()
{
	int *closerStars = new int[m_UsList.size()];
	memset(closerStars, 0, sizeof(int) * m_UsList.size());
	
	for (int i = 0; i < m_ThemList.size(); ++i)
	{
		const AttackData *ad = m_ThemList[i].GetCloserAttack();
		if (ad)
		{
			closerStars[ad->GetTargetId()-1] += ad->GetStars();
		}
		else if (OPTIONS::GetInstance().parser_Check_Missing_Attacks)
		{
			std::cout << "Warning: No attack against enemy #" << i << std::endl;
		}
	}
	
	for (int i = 0; i < m_UsList.size(); ++i)
	{
		if (OPTIONS::GetInstance().parser_Check_Player_Totals)
		{
			if (closerStars[i] > MAX_STARS_PER_WAR)
			{
				std::cout << "Warning: closer stars exceeded for " << m_UsList[i].GetPlayerName() << std::endl;
			}
			
			if (m_UsList[i].GetTotalStars() > MAX_STARS_PER_WAR)
			{
				std::cout << "Warning: max stars exceeded for " << m_UsList[i].GetPlayerName() << std::endl;
			}
			
			if (m_UsList[i].GetAttackCount() > MAX_PLAYER_ATTACKS)
			{
				std::cout << "Warning: max attacks per player exceeded for " << m_UsList[i].GetPlayerName() << std::endl;
			}
		}
		
		if (OPTIONS::GetInstance().parser_Check_Missing_Attacks)
		{
			if (m_UsList[i].GetDefends().size() == 0)
			{
				std::cout << "Warning: No attack against player: " << m_UsList[i].GetPlayerName() << std::endl;
			}
		}
		
		m_UsList[i].SetCloserStars(closerStars[i]);
	}
	
	delete [] closerStars;
}

void WarData::RunReports() const
{
	// Verification
	ReportFinalScore();
	ReportPlayerStats();
	
	// Warnings
//	ReportWarningMissingInAction();
//	ReportWarningNuke();
//	ReportWarningSnipe();
}

void WarData::ReportFinalScore() const
{
	int usScore = 0;
	int themScore = 0;
	
	int usTHScore[kTH11];
	memset(usTHScore, 0, sizeof(int)*kTH11);
	
	int themTHScore[kTH11];
	memset(themTHScore, 0, sizeof(int)*kTH11);
	
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
	for (int i = kTH6-1; i < kTH11; ++i)
	{
		std::cout << "TH(" << i+1 << ") Us: " << usTHScore[i] << " Them: " << themTHScore[i] << std::endl;
	}
}

void WarData::ReportPlayerStats() const
{
	for (int i = 0; i < m_UsList.size(); ++i)
	{
		std::cout << m_UsList[i].GetPlayerName() << " stars(" << m_UsList[i].GetTotalStars() << "|" << m_UsList[i].GetCloserStars() << ") bleeds("
				  << m_UsList[i].GetDefends().size() - 1 << ") hold ("
				  << 3 - m_UsList[i].GetMaxStarsGiven() << ")" << std::endl;
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

void WarData::ReportWarningNuke() const
{
	// us nukes
	int total = 0;
	std::cout << "Nukes (Us)" << std::endl;
	for (int i = 0; i < m_UsList.size(); ++i)
	{
		for (int j = 0; j < m_UsList[i].GetAttacks().size(); ++j)
		{
			const AttackData attack = m_UsList[i].GetAttacks()[j];
			if (m_ThemList[attack.GetTargetId()-1].GetTownHallLevel() < m_UsList[i].GetTownHallLevel())
			{
				std::cout << "Nuke: " << m_UsList[i].GetPlayerName() << " vs " << attack.GetTargetId() << " - Stars (" << attack.GetStars() << ")" << std::endl;
				++total;
			}
		}
	}
	std::cout << "Total nukes (us): " << total << std::endl;

	// them nukes
	total = 0;
	std::cout << "Nukes (Them)" << std::endl;
	for (int i = 0; i < m_ThemList.size(); ++i)
	{
		for (int j = 0; j < m_ThemList[i].GetAttacks().size(); ++j)
		{
			const AttackData attack = m_ThemList[i].GetAttacks()[j];
			if (m_UsList[attack.GetTargetId()-1].GetTownHallLevel() < m_ThemList[i].GetTownHallLevel())
			{
				std::cout << "Nuke: " << i << " vs " << m_UsList[attack.GetTargetId()-1].GetPlayerName() << " - Stars (" << attack.GetStars() << ")" << std::endl;
				++total;
			}
		}
	}
	std::cout << "Total nukes (them): " << total << std::endl;
}

void WarData::ReportWarningSnipe() const
{
	// us snipes
	int total = 0;
	std::cout << "Snipes (Us)" << std::endl;
	for (int i = 0; i < m_UsList.size(); ++i)
	{
		for (int j = 0; j < m_UsList[i].GetAttacks().size(); ++j)
		{
			const AttackData attack = m_UsList[i].GetAttacks()[j];
			if (m_ThemList[attack.GetTargetId()-1].GetTownHallLevel() > m_UsList[i].GetTownHallLevel())
			{
				std::cout << "Snipe: " << m_UsList[i].GetPlayerName() << " vs " << attack.GetTargetId() << " - Stars (" << attack.GetStars() << ")" << std::endl;
				++total;
			}
		}
	}
	std::cout << "Total snipes (us): " << total << std::endl;
	
	// them nukes
	total = 0;
	std::cout << "Snipes (Them)" << std::endl;
	for (int i = 0; i < m_ThemList.size(); ++i)
	{
		for (int j = 0; j < m_ThemList[i].GetAttacks().size(); ++j)
		{
			const AttackData attack = m_ThemList[i].GetAttacks()[j];
			if (m_UsList[attack.GetTargetId()-1].GetTownHallLevel() > m_ThemList[i].GetTownHallLevel())
			{
				std::cout << "Snipe: " << i << " vs " << m_UsList[attack.GetTargetId()-1].GetPlayerName() << " - Stars (" << attack.GetStars() << ")" << std::endl;
				++total;
			}
		}
	}
	std::cout << "Total snipes (them): " << total << std::endl;
}

