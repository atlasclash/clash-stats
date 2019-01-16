//
//  ParserV1.cpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "ParserV1.hpp"
#include "StringHelpers.hpp"
#include "WarData.hpp"
#include "AttackData.hpp"
#include "PlayerData.hpp"
#include "Options.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#define PARSER_VERSION						("1")

#define FIELD_WARDATA_ENEMY_CLAN_NAME		(0)
#define FIELD_WARDATA_ENEMY_CLAN_TAG		(1)
#define FIELD_WARDATA_US_CLAN_NAME			(3)
#define FIELD_WARDATA_US_CLAN_TAG			(4)
#define FIELD_WARDATA_USER_META				(6)

#define FIELD_WARDATA_SIZE					(0)

#define FIELD_WARDATA_DATE					(0)

#define FIELD_PLAYER_NAME					(1)
#define FIELD_PLAYER_TH						(2)
#define FIELD_PLAYER_TAG					(3)
#define FIELD_PLAYER_SPECIAL				(4)
#define FIELD_PLAYER_OPPONENT_TH			(5)

#define FIELD_ATTACK_NUM					(0)
#define FIELD_ATTACK_US_STARS				(1)
#define FIELD_ATTACK_US_PCT					(2)
#define FIELD_ATTACK_US_INDEX				(3)
#define FIELD_ATTACK_THEM_INDEX				(4)
#define FIELD_ATTACK_THEM_STARS				(5)
#define FIELD_ATTACK_THEM_PCT				(6)

ParserV1::ParserV1(const char *fileName) : Parser(fileName)
{
    m_version = atoi(PARSER_VERSION);
}

ParserV1::~ParserV1()
{
	
}

void ParserV1::ProcessWar(WarData *warData)
{
	std::cout << "Reading war data... [v1]" << std::endl;
	
	if (m_dataFile == NULL)
	{
		std::cout << "Unable to open file" << std::endl;
		return;
	}
	
	// retrieve a single line from the file
	std::vector<std::string> cellResults = GetCellsFromLine();
	
	// line 1
	if (cellResults[1] != PARSER_VERSION)
	{
		std::cout << "Incorrect file format" << std::endl;
		return;
	}
	
	// line 2: clan name / clan tag
	cellResults = GetCellsFromLine();
	warData->SetThemClanName(cellResults[FIELD_WARDATA_ENEMY_CLAN_NAME]);
	warData->SetThemClanTag(cellResults[FIELD_WARDATA_ENEMY_CLAN_TAG]);
	warData->SetUsClanName(cellResults[FIELD_WARDATA_US_CLAN_NAME]);
	warData->SetUsClanTag(cellResults[FIELD_WARDATA_US_CLAN_TAG]);
	warData->SetUserMeda(cellResults[FIELD_WARDATA_USER_META]);
	
	std::cout << "Clan: " << cellResults[FIELD_WARDATA_ENEMY_CLAN_NAME] << " Tag: " << cellResults[FIELD_WARDATA_ENEMY_CLAN_TAG] << std::endl;
	
	// line 3: war size
	cellResults = GetCellsFromLine();
	warData->SetWarSize(atoi(cellResults[FIELD_WARDATA_SIZE].c_str()));
	
	std::cout << "War size: " << cellResults[FIELD_WARDATA_SIZE] << std::endl;
	
	// line 4: war date
	cellResults = GetCellsFromLine();
	warData->SetWarDate(cellResults[FIELD_WARDATA_DATE]);
	
	// line 5 - WarSize: process participants
	for (int i = 0; i < warData->GetWarSize(); ++i)
	{
		cellResults = GetCellsFromLine();
		
		PlayerData *usPlayer = new PlayerData();
		usPlayer->SetPlayerName(cellResults[FIELD_PLAYER_NAME]);
		usPlayer->SetTownHallLevel((eTownHallLevel)atoi(cellResults[FIELD_PLAYER_TH].c_str()));
		usPlayer->SetPlayerTag(cellResults[FIELD_PLAYER_TAG]);
		
		PlayerData *themPlayer = new PlayerData();
		themPlayer->SetSpecialFlag((PlayerData::eSpecialFlags)atoi(cellResults[FIELD_PLAYER_SPECIAL].c_str()));
		themPlayer->SetTownHallLevel((eTownHallLevel)atoi(cellResults[FIELD_PLAYER_OPPONENT_TH].c_str()));
		
		warData->AddUsPlayer(usPlayer);
		warData->AddThemPlayer(themPlayer);
	}
	
	const int warSize = warData->GetWarSize();
	if (warData->GetUsPlayerCount() != warSize)
	{
		std::cout << "Us player count mis-match.  Aborting" << std::endl;
		return;
	}
	
	if (warData->GetThemPlayerCount() != warSize)
	{
		std::cout << "Them player count mis-match.  Aborting" << std::endl;
		return;
	}
	
	cellResults = GetCellsFromLine();
	int attackTotal = 0;
	while (cellResults.size())
	{
		++attackTotal;
		
		// we are attacking them
		if (cellResults[FIELD_ATTACK_US_STARS].length() && cellResults[FIELD_ATTACK_US_PCT].length())
		{
			int attackNumber		= atoi(cellResults[FIELD_ATTACK_NUM].c_str());
			int opponentId			= atoi(cellResults[FIELD_ATTACK_THEM_INDEX].c_str());
			int usId				= atoi(cellResults[FIELD_ATTACK_US_INDEX].c_str());
			int stars				= atoi(cellResults[FIELD_ATTACK_US_STARS].c_str());
			int pct					= atoi(cellResults[FIELD_ATTACK_US_PCT].c_str());
			
			if (OPTIONS::GetInstance().parser_Check_Data_Ranges && !CheckDataRanges(opponentId, usId, stars, pct, warSize))
			{
				std::cout << "Check data range failure: attack: " << attackTotal << std::endl;
				return;
			}
			
			const PlayerData *opponent = warData->GetThem(opponentId);
			assert(opponent != NULL);
			
			eTownHallLevel usTH		= warData->GetUsTHLevel(usId);
			eTownHallLevel oppTH 	= warData->GetThemTHLevel(opponentId);
			bool isSalt				= opponent->IsSalt();
			bool isCloseAttk		= (!opponent->IsClosed() && stars == AttackData::kThreeStar);
			int attemptNum			= (int)opponent->GetDefendCount() + 1;	// we're the next try
			
			const AttackData *usAttack = new AttackData(opponentId, (AttackData::StarType)stars, pct, oppTH, isSalt, isCloseAttk, attemptNum, attackNumber);
			const AttackData *themDefend = new AttackData(usId, (AttackData::StarType)stars, pct, usTH);
			warData->AddUsAttack(usAttack, usId);
			warData->AddThemDefend(themDefend, opponentId);
		}
		// they are attacking us
		else if (cellResults[FIELD_ATTACK_THEM_STARS].length() && cellResults[FIELD_ATTACK_THEM_PCT].length())
		{
			int opponentId			= atoi(cellResults[FIELD_ATTACK_THEM_INDEX].c_str());
			int stars				= atoi(cellResults[FIELD_ATTACK_THEM_STARS].c_str());
			int pct					= atoi(cellResults[FIELD_ATTACK_THEM_PCT].c_str());
			int usId				= atoi(cellResults[FIELD_ATTACK_US_INDEX].c_str());

			if (OPTIONS::GetInstance().parser_Check_Data_Ranges && !CheckDataRanges(opponentId, usId, stars, pct, warSize))
			{
				std::cout << "Check data range failure: attack: " << attackTotal << std::endl;
				return;
			}

			eTownHallLevel usTH		= warData->GetUsTHLevel(usId);
			eTownHallLevel oppTH 	= warData->GetThemTHLevel(opponentId);

			const AttackData *usDefend = new AttackData(opponentId, (AttackData::StarType)stars, pct, oppTH);
			const AttackData *themAttack = new AttackData(usId, (AttackData::StarType)stars, pct, usTH);
			warData->AddUsDefend(usDefend, usId);
			warData->AddThemAttack(themAttack, opponentId);
		}
		
		// get next attack/defense
		cellResults = GetCellsFromLine();
	}
	
	std::cout << "Read " << attackTotal << " attacks." << std::endl;
	std::cout << "War data complete." << std::endl;
}

