//
//  WarData.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "WarData.hpp"
#include "Options.hpp"
#include "WarRecord.hpp"
#include "PlayerWarRecord.hpp"
#include "AttackRecord.hpp"
#include "DefendRecord.hpp"
#include "Database.hpp"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <algorithm>

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

PlayerData* WarData::GetUs(const unsigned int which)
{
	if (m_UsList.size() < which-1)
		return NULL;
	
	return &m_UsList[which-1];
}

PlayerData* WarData::GetThem(const unsigned int which)
{
	if (m_ThemList.size() < which-1)
		return NULL;
	
	return &m_ThemList[which-1];
}


void WarData::CalcCloserStars()
{
	int *closerStars = new int[m_UsList.size()];
	memset(closerStars, 0, sizeof(int) * m_UsList.size());
	
	for (int i = 0; i < m_ThemList.size(); ++i)
	{
		AttackData *ad = m_ThemList[i].GetCloserAttack();
		if (ad)
		{
			closerStars[ad->GetTargetId()-1] += ad->GetStars();
			
			// in some cases, the closer stars are awarded to non-3-starred bases.
			// we need to denote that this attack was the closer attack.
			{
				AttackData *usAttack = m_UsList[ad->GetTargetId()-1].GetAttackVs(i+1);
				usAttack->SetClosed(true);
				ad->SetClosed(true);
			}
		}
		else if (OPTIONS::GetInstance().parser_Check_Missing_Attacks)
		{
			std::cout << "Warning: No attack against enemy #" << i+1 << std::endl;
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

void WarData::CalcTotalScores()
{
	int usScore = 0;
	int themScore = 0;
	
	int usTHScore[kTH12];
	memset(usTHScore, 0, sizeof(int)*kTH12);
	
	int themTHScore[kTH12];
	memset(themTHScore, 0, sizeof(int)*kTH12);
	
	// assumption is that these lists are the same length
	for (int i = 0; i < m_UsList.size(); ++i)
	{
		int usStarsGiven	= m_UsList[i].GetMaxStarsGiven();
		int themStarsGiven	= m_ThemList[i].GetMaxStarsGiven();
		
		themScore	+= usStarsGiven;
		usScore		+= themStarsGiven;
		
		themTHScore[m_UsList[i].GetTownHallLevel()-1]		+= usStarsGiven;
		usTHScore[m_ThemList[i].GetTownHallLevel()-1]		+= themStarsGiven;
	}
	
	m_UsScore = usScore;
	m_ThemScore = themScore;

//	std::cout << "Stars earned per TH level" << std::endl;
//	for (int i = kTH6-1; i < kTH12; ++i)
//	{
//		std::cout << "TH(" << i+1 << ") Us: " << usTHScore[i] << " Them: " << themTHScore[i] << std::endl;
//	}
}

int WarData::GetTotalSecondsFromEpochOfWarDate() const
{
	return DATABASE::GetInstance().GetTotalSecondsFromEpoch(m_DateStr);
}

bool WarData::SaveWarToDB()
{
	// record any new players seen
	DATABASE::GetInstance().WritePlayerTags(m_UsList);
	
	// record the war summary
	WarRecord warRecord;
	warRecord.opponentName	= m_OpponentClanName;
	warRecord.opponentTag	= m_OpponentClanTag;
	warRecord.usName		= m_UsClanName;
	warRecord.usTag			= m_UsClanTag;
	warRecord.userMeta		= m_UserDefinedMeta;
	warRecord.playerCount	= m_WarSize;
	warRecord.usScore		= m_UsScore;
	warRecord.themScore		= m_ThemScore;
	warRecord.date			= GetTotalSecondsFromEpochOfWarDate();
	
	DATABASE::GetInstance().WriteWarRecord(warRecord);
	if (warRecord.pk == Database::INVALID_KEY)
	{
		std::cout << "Unable to write war!" << std::endl;
		return false;
	}
	
	for (int i = 0; i < m_UsList.size(); ++i)
	{
		PlayerData pd = m_UsList[i];
		
		PlayerWarRecord playerWarRecord;
		playerWarRecord.playerTagKey		= pd.GetPlayerTag();
		playerWarRecord.warKey				= warRecord.pk;
		playerWarRecord.closerStars			= pd.GetCloserStars();
		playerWarRecord.holds				= pd.GetHolds();
		playerWarRecord.bleeds				= pd.GetBleeds();
		playerWarRecord.nuked				= pd.GetNukes();
		playerWarRecord.totalStars			= pd.GetTotalStars();
		playerWarRecord.threeStars			= pd.GetThreeStars();
		playerWarRecord.playerTH			= (int)pd.GetTownHallLevel();
		
		DATABASE::GetInstance().WritePlayerWarRecord(playerWarRecord);
		
		for (int j = 0; j < (int)pd.GetAttackCount(); ++j)
		{
			AttackData ad = pd.GetAttacks()[j];
			AttackRecord ar;
			
			ar.playerTagPk					= pd.GetPlayerTag();
			ar.playerTH						= (int)pd.GetTownHallLevel();
			ar.playerWgt					= pd.GetPlayerWeight();
			ar.opponentTH					= ad.GetTownHall();
			ar.opponentWgt					= ad.GetOpponentWeight();
			ar.starCount					= ad.GetStars();
			ar.percentDmg					= ad.GetPctDamage();
			ar.isSalt						= ad.IsSalt();
			ar.isClose						= ad.IsClose();
			ar.attemptNum					= ad.GetAttemptNumber();
			ar.warPk						= warRecord.pk;
			ar.attackNum					= ad.GetAttackNumber();
			
			DATABASE::GetInstance().WritePlayerAttackRecord(ar);
		}
		
		for (int k = 0; k < (int)pd.GetDefendCount(); ++k)
		{
			AttackData ad = pd.GetDefends()[k];
			DefendRecord dd;
			
			dd.playerTagPk					= pd.GetPlayerTag();
			dd.playerTH						= (int)pd.GetTownHallLevel();
			dd.playerWgt					= pd.GetPlayerWeight();
			dd.opponentTH					= ad.GetTownHall();
			dd.opponentWgt					= ad.GetOpponentWeight();
			dd.starCount					= ad.GetStars();
			dd.percentDmg					= ad.GetPctDamage();
			dd.warPk						= warRecord.pk;
			
			DATABASE::GetInstance().WritePlayerDefendRecord(dd);
		}
	}
	
	return true;
}

void WarData::RunReports() const
{
	// Verification
	ReportFinalScore();
	ReportPlayerStats();
	
	// Warnings
	ReportWarningMissingInAction();
	ReportWarningNuke();
	ReportWarningSnipe();
}

void WarData::CalcWarStats()
{
	// us vs them total score
	CalcTotalScores();
	
	// closer stars
	CalcCloserStars();
}

void WarData::ReportFinalScore() const
{
	std::cout << "Final Score:" << std::endl;
	std::cout << "  Us: " << m_UsScore << std::endl;
	std::cout << "  Them: " << m_ThemScore << std::endl;
}

void WarData::ReportPlayerStats() const
{
	// player name
	// player tag
	// closer stars
	// holds
	// bleeds
	// nuked
	// total stars
	// three stars
	// <scouts>
	// mia ?
	
	for (int i = 0; i < m_UsList.size(); ++i)
	{
		std::cout << m_UsList[i].GetPlayerName() << " stars(" << m_UsList[i].GetTotalStars() << "|" << m_UsList[i].GetCloserStars() << ") bleeds("
				  << m_UsList[i].GetBleeds() << ") hold ("
				  << m_UsList[i].GetHolds()  << ") nukes ("
				  << m_UsList[i].GetNukes()  << ")" << std::endl;
	}
	
	std::ofstream outputFile;
	std::string dateStr = m_DateStr;
	std::replace(dateStr.begin(), dateStr.end(), '/', '-');
	outputFile.open(dateStr + "-" + m_OpponentClanName + "-results.csv");
	
	// header
	outputFile << "player,tag,closer stars,holds,bleeds,nuked,total stars,three stars\n";
	for (int i = 0; i < m_UsList.size(); ++i)
	{
		outputFile	<< m_UsList[i].GetPlayerName()		<< ","
					<< m_UsList[i].GetPlayerTag()		<< ","
					<< m_UsList[i].GetCloserStars()		<< ","
					<< m_UsList[i].GetHolds()			<< ","
					<< m_UsList[i].GetBleeds()			<< ","
					<< m_UsList[i].GetNukes()			<< ","
					<< m_UsList[i].GetTotalStars()		<< ","
					<< m_UsList[i].GetThreeStars()		<< ",\n";
	}
	
	outputFile.close();
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
				std::cout << "Nuke: " << i+1 << " vs " << m_UsList[attack.GetTargetId()-1].GetPlayerName() << " - Stars (" << attack.GetStars() << ")" << std::endl;
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
				std::cout << "Snipe: " << i+1 << " vs " << m_UsList[attack.GetTargetId()-1].GetPlayerName() << " - Stars (" << attack.GetStars() << ")" << std::endl;
				++total;
			}
		}
	}
	std::cout << "Total snipes (them): " << total << std::endl;
}

void WarData::ReportScoreGraph() const
{
	ReportScoreGraphWithList(m_UsList);
	ReportScoreGraphWithList(m_ThemList);
}

void WarData::ReportScoreGraphWithList(const std::vector<PlayerData> list) const
{
	int i = (int)list.size();
	do
	{
		std::cout << "Player,Rank,TH,OppRank,OppWgt,Stars,Pct,OppTH,Close,Attempt,AttackNumber" << std::endl;
		
		for (int j = 0; j < list[i].GetAttacks().size(); ++j)
		{
			const AttackData attack = list[i].GetAttacks()[j];
			// output the attack data
			// player name, player id, player th, attack record ...
			
			PlayerData player = list[i];
			
			std::cout	<< player.GetPlayerName()			<< ","
						<< i+1								<< ","
						<< player.GetTownHallLevel()		<< ","
						<< attack.GetTargetId()				<< ","
						<< attack.GetOpponentWeight()		<< ","
						<< attack.GetStars()				<< ","
						<< attack.GetPctDamage()			<< ","
						<< attack.GetTownHall()				<< ","
						<< attack.IsClose()					<< ","
						<< attack.GetAttemptNumber()		<< ","
						<< attack.GetAttackNumber()			<< std::endl;
		}
		
	} while (--i >= 0);
}
