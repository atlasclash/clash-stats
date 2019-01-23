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
	
	std::ofstream outputFile;
	outputFile.open("Clan-War-History.csv");
	const std::string delimiter = ",";
	
	outputFile	<< "Meta" << delimiter
				<< "Opponent" << delimiter
				<< "Opp Tag" << delimiter
				<< "Size" << delimiter
				<< "Us Score" << delimiter
				<< "Them Score" << delimiter
				<< "Date" << delimiter << "\n";
	
	for (int i = 0; i < m_WarRecordList.size(); ++i)
	{
		tmpStruct warSize;
		const WarRecord wr = m_WarRecordList[i];
		
		if (warRecord.find(wr.playerCount) == warRecord.end())
		{
			warSize.matches = 1;
			warSize.wins	= (wr.usScore > wr.themScore) ? 1 : 0;
			warRecord[wr.playerCount] = warSize;
		}
		else
		{
			tmpStruct s = warRecord[wr.playerCount];
			s.matches++;
			if (wr.usScore > wr.themScore)
			{
				s.wins++;
			}
			warRecord[wr.playerCount] = s;
		}
		
		outputFile	<< wr.userMeta << delimiter
					<< wr.opponentName << delimiter
					<< wr.opponentTag << delimiter
					<< wr.playerCount << delimiter
					<< wr.usScore << delimiter
					<< wr.themScore << delimiter
					<< DATABASE::GetInstance().StringFromDate(wr.date) << delimiter << "\n";
	}
	
	std::map<int, tmpStruct>::iterator it = warRecord.begin();
	
	outputFile	<< "Size" << delimiter
				<< "Matches" << delimiter
				<< "Wins" << delimiter
				<< "Percent" << delimiter << "\n";
	
	while (it != warRecord.end())
	{
		tmpStruct s = it->second;
		outputFile	<< it->first << delimiter
					<< s.matches << delimiter
					<< s.wins << delimiter
					<< (int)((float)s.wins / (float)s.matches*100) << delimiter << "\n";
		it++;
	}
	
	outputFile.close();
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

void Clan::CreateCloseRateByAttackNumber()
{
	Reset();
	
	// Get all attacks
	std::vector<AttackRecord> attackList;
	DATABASE::GetInstance().ReadAllAttackData(attackList);
	
	struct tmpStruct
	{
		int stars;
		int destruction;
		int count;
	};
	
	std::map<int, tmpStruct> starsPerAttempt;
	
	for (int i = 0; i < attackList.size(); ++i)
	{
		const AttackRecord ar = attackList[i];
		if (ar.opponentTH != ar.playerTH)
			continue;
		
		if (starsPerAttempt.find(ar.attemptNum) == starsPerAttempt.end())
		{
			tmpStruct attk;
			attk.stars = ar.starCount;
			attk.destruction = ar.percentDmg;
			attk.count++;
			starsPerAttempt[ar.attemptNum] = attk;
		}
		else
		{
			tmpStruct totalAttk = starsPerAttempt[ar.attemptNum];
			totalAttk.stars += ar.starCount;
			totalAttk.destruction += ar.percentDmg;
			totalAttk.count++;
			starsPerAttempt[ar.attemptNum] = totalAttk;
		}
	}
	
	std::map<int, tmpStruct>::iterator it = starsPerAttempt.begin();
	std::cout << "Close Rate Per Attempt" << std::endl;
	std::cout << "Attempt - Avg Stars - Avg Destruction" << std::endl;
	while (it != starsPerAttempt.end())
	{
		tmpStruct s = it->second;
		std::cout << it->first << " " << (float)((float)s.stars / (float)s.count) << " " << s.destruction / s.count << " " << s.count << std::endl;
		it++;
	}
}

// SELECT * FROM AttackTable WHERE playerTH = 11 AND opponentTH = 9 AND attemptNum = 1 AND warKey IN (SELECT pk FROM WarTable WHERE userMeta='LEAGUE');
void Clan::CreateLeagueAttackMatrix()
{
	Reset();

	struct peerStruct
	{
		int attacks;
		int closes;
	};
	
	std::map<int, peerStruct> freshPeerAttacks;
	
	std::vector<AttackRecord> attackList;
	DATABASE::GetInstance().ReadAttackDataWithUserMeta(attackList, "LEAGUE");

	for (int i = 0; i < attackList.size(); ++i)
	{
		const AttackRecord ar = attackList[i];
		
		// Fresh attempt
		if (ar.attemptNum == 1)
		{
			if (ar.isPeerAttack())
			{
				if (freshPeerAttacks.find(ar.playerTH) == freshPeerAttacks.end())
				{
					peerStruct tmp;
					tmp.attacks = 1;
					tmp.closes = (ar.starCount == 3) ? 1 : 0;
					freshPeerAttacks[ar.playerTH] = tmp;
				}
				else
				{
					peerStruct totPeerAttacks = freshPeerAttacks[ar.playerTH];
					totPeerAttacks.attacks++;
					if (ar.starCount == 3)
					{
						totPeerAttacks.closes++;
					}
					freshPeerAttacks[ar.playerTH] = totPeerAttacks;
				}
			}
		}
		// "Scouted"
		else
		{
			
		}
	}
	
	printf("");
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
