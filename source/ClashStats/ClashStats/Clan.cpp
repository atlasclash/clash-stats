//
//  Clan.cpp
//  ClashStats
//
//  Created by Atlas on 12/12/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "Clan.hpp"
#include "Player.hpp"
#include "WarRecord.hpp"
#include "AttackRecord.hpp"
#include "Database.hpp"
#include <iostream>
#include <fstream>

Clan::Clan()
{
	
}

Clan::~Clan()
{
	Reset();
}

void Clan::Reset()
{
	m_PlayerList.clear();
	m_WarRecordList.clear();
}

void Clan::CreateWarRecord()
{
	Reset();
	
	DATABASE::GetInstance().ReadAllWars(m_WarRecordList);
	
	struct tmpStruct
	{
		int matches;
		int wins;
	};
	
	std::map<int, tmpStruct> warRecord;
	
	for (int i = 0; i < m_WarRecordList.size(); ++i)
	{
		const WarRecord w = m_WarRecordList[i];
		
		tmpStruct warSize;
		if (warRecord.find(w.playerCount) == warRecord.end())
		{
			warSize.matches = 1;
			warSize.wins    = (w.usScore > w.themScore) ? 1 : 0;
			warRecord[w.playerCount] = warSize;
		}
		else
		{
			tmpStruct s = warRecord[w.playerCount];
			s.matches++;
			if (w.usScore > w.themScore)
			{
				s.wins++;
			}
			warRecord[w.playerCount] = s;
		}
	}
	
	std::map<int, tmpStruct>::iterator it = warRecord.begin();
	std::cout << "Sz # W" << std::endl;
	while (it != warRecord.end())
	{
		
		tmpStruct s = it->second;
		std::cout << it->first << " " << s.matches << " " << s.wins << " pct:" << (int)((float)s.wins / (float)s.matches*100) << std::endl;
		it++;
	}
}

void Clan::CreateBaseCloseRate()
{
	Reset();
	
	// Get all attacks
	std::vector<AttackRecord> attackList;
	DATABASE::GetInstance().ReadAllAttackData(attackList);
	
	struct tmpStruct
	{
		int attacks;
		int closes;
	};
	
	std::map<int, tmpStruct> closeRate;
	
	for (int i = 0; i < attackList.size(); ++i)
	{
		const AttackRecord ar = attackList[i];
		if (ar.opponentTH != ar.playerTH)
			continue;
		
		// do we have this base weight already?
		if (closeRate.find(ar.opponentWgt) == closeRate.end())
		{
			tmpStruct baseWgt;
			baseWgt.attacks = 1;
			baseWgt.closes  = (ar.starCount == 3) ? 1 : 0;
			closeRate[ar.opponentWgt] = baseWgt;
		}
		else
		{
			tmpStruct s = closeRate[ar.opponentWgt];
			s.attacks++;
			if (ar.starCount == 3)
			{
				s.closes++;
			}
			
			closeRate[ar.opponentWgt] = s;
		}
	}
	
	std::map<int, tmpStruct>::iterator it = closeRate.begin();
	std::cout << "Wgt # Close Pct" << std::endl;
	while (it != closeRate.end())
	{
		tmpStruct s = it->second;
		std::cout << it->first << " " << s.attacks << " " << s.closes << " " << (int)((float)s.closes / (float)s.attacks*100) << std::endl;
		it++;
	}
}

void Clan::CreateClanWithUserMeta(std::string userMeta)
{
	Reset();
	
	// get a list of all the players in this particular meta
	std::vector<int> playerIDs;
	
	// initialize all fo the players
	DATABASE::GetInstance().ReadPlayerIDsWithWarUserMeta(userMeta, playerIDs);

	std::ofstream outputFile;
	outputFile.open("Clan-Report-" + userMeta + ".csv");

	const std::string delimiter = ",";
	
	// header block
	outputFile	<< "Player"					<< delimiter
				<< "TH"						<< delimiter
				<< "Total Wars"				<< delimiter
				<< "Total Closer Stars"		<< delimiter
				<< "Total Avg Closer Stars"	<< delimiter
				<< "Total Stars"			<< delimiter
				<< "Avg Total Stars"		<< delimiter
				<< "Total 3-stars"			<< delimiter
				<< "Total Holds"			<< delimiter
				<< "Total Bleeds"			<< delimiter
				<< "Total Nuked"			<< delimiter

				<< "Attacks"				<< delimiter
				<< "Closer Stars"			<< delimiter
				<< "Quality Stars"			<< delimiter
				<< "Avg Closer Stars"		<< delimiter
				<< "Stars"					<< delimiter
				<< "Avg Stars"				<< delimiter
				<< "Avg Dmg %"				<< delimiter
				<< "3-stars"				<< delimiter
				<< "3-star %"				<< delimiter
				<< "Near-miss (95%)"		<< delimiter
	
				<< "Salt Attacks"			<< delimiter
				<< "Salt Closer Stars"		<< delimiter
				<< "Salt Stars"				<< delimiter
				<< "Salt Avg Stars"			<< delimiter
				<< "Salt 3-stars"			<< delimiter
				<< "Salt 3-star %"			<< delimiter
				<< "Salt 1st Attempt"		<< delimiter
				<< "Salt % 1st Close"		<< delimiter
				<< "Salt Avg Dmg"			<< delimiter
	
				<< "Boot Attacks"			<< delimiter
				<< "Boot Closer Stars"		<< delimiter
				<< "Boot Stars"				<< delimiter
				<< "Boot Avg Stars"			<< delimiter
				<< "Boot 3-stars"			<< delimiter
				<< "Boot 3-star %"			<< delimiter
				<< "Boot 1st Attempt"		<< delimiter
				<< "Boot % 1st Close"		<< delimiter
				<< "Boot Avg Dmg"			<< delimiter
	
				<< "Nuke Attacks"			<< delimiter
				<< "Nuke Closer Stars"		<< delimiter
				<< "Nuke Stars"				<< delimiter
				<< "Nuke Avg Stars"			<< delimiter
				<< "Nuke 3-stars"			<< delimiter
				<< "Nuke 3-star %"			<< delimiter
				<< "Nuke 1st Attempt"		<< delimiter
				<< "Nuke % 1st Close"		<< delimiter
				<< "Nuke Avg Dmg"			<< delimiter
	
				<< "Snipe Attacks"			<< delimiter
				<< "Snipe Closer Stars"		<< delimiter
				<< "Snipe Stars"			<< delimiter
				<< "Snipe Avg Stars"		<< delimiter
				<< "Snipe 3-stars"			<< delimiter
				<< "Snipe 3-star %"			<< delimiter
				<< "Snipe 1st Attempt"		<< delimiter
				<< "Snipe % 1st Close"		<< delimiter
				<< "Snipe Avg Dmg"			<< delimiter
	
				<< "Total Defends"			<< delimiter
				<< "Stars Yielded"			<< delimiter
				<< "Avg Stars Yielded"		<< delimiter
				<< "Avg Dmg"				<< delimiter
				<< "3-stars Yielded"		<< delimiter
				<< "Misses"					<< delimiter
				<< "Defend Ratio"			<< delimiter
	
				<< "Peer Attacks"			<< delimiter
				<< "Peer Stars"				<< delimiter
				<< "Peer 3-stars"			<< delimiter
				<< "Peer Avg Stars"			<< delimiter
				<< "Peer Avg Dmg"			<< delimiter
				<< "Peer Misses"			<< delimiter
	
				<< "Nuke Attacks"			<< delimiter
				<< "Nuke Stars"				<< delimiter
				<< "Nuke 3-stars"			<< delimiter
				<< "Nuke Avg Stars"			<< delimiter
				<< "Nuke Avg Dmg"			<< delimiter
				<< "Nuke Misses"			<< delimiter
	
				<< "Snipe Attacks"			<< delimiter
				<< "Snipe Stars"			<< delimiter
				<< "Snipe 3-stars"			<< delimiter
				<< "Snipe Avg Stars"		<< delimiter
				<< "Snipe Avg Dmg"			<< delimiter
				<< "Snipe Misses"			<< delimiter << "\n";
	
	
	// write each player's record
	for (int i = 0; i < playerIDs.size(); ++i)
	{
		Player *p1 = new Player(playerIDs[i]);
		p1->GenerateHistoryWithMeta(userMeta);
		p1->WritePlayerStatsData(outputFile);
		
		delete p1;
	}
	
	// close
	outputFile.close();
}
