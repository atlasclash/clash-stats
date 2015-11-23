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
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#define PARSER_VERSION						("1")

#define FIELD_WARDATA_ENEMY_CLAN_NAME		(0)
#define FIELD_WARDATA_ENEMY_CLAN_TAG		(1)

#define FIELD_WARDATA_SIZE					(0)

#define FIELD_WARDATA_DATE					(0)

#define FIELD_PLAYER_NAME					(1)
#define FIELD_PLAYER_TH						(2)
#define FIELD_PLAYER_TAG					(3)
#define FIELD_PLAYER_SPECIAL				(4)
#define FIELD_PLAYER_OPPONENT_TH			(5)

#define FIELD_ATTACK_US_STARS				(1)
#define FIELD_ATTACK_US_PCT					(2)
#define FIELD_ATTACK_US_INDEX				(3)
#define FIELD_ATTACK_THEM_INDEX				(4)
#define FIELD_ATTACK_THEM_STARS				(5)
#define FIELD_ATTACK_THEM_PCT				(6)

ParserV1::ParserV1(const char *fileName) : Parser(fileName)
{
	
}

ParserV1::~ParserV1()
{
	
}

void ParserV1::ProcessWar(WarData *warData)
{
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
	warData->SetClanName(cellResults[FIELD_WARDATA_ENEMY_CLAN_NAME]);
	warData->SetClanTag(cellResults[FIELD_WARDATA_ENEMY_CLAN_TAG]);
	
	// line 3: war size
	cellResults = GetCellsFromLine();
	warData->SetWarSize(atoi(cellResults[FIELD_WARDATA_SIZE].c_str()));
	
	// line 4: war date
	cellResults = GetCellsFromLine();
	warData->SetWarDate(cellResults[FIELD_WARDATA_DATE]);
	
	// line 5 - WarSize: process participants
	for (int i = 0; i < warData->GetWarSize(); ++i)
	{
		cellResults = GetCellsFromLine();
		
		PlayerData *usPlayer = new PlayerData();
		usPlayer->SetPlayerName(cellResults[FIELD_PLAYER_NAME]);
		usPlayer->SetTownHallLevel((PlayerData::eTownHallLevel)atoi(cellResults[FIELD_PLAYER_TH].c_str()));
		usPlayer->SetPlayerTag(cellResults[FIELD_PLAYER_TAG]);
		
		PlayerData *themPlayer = new PlayerData();
		themPlayer->SetSpecialFlag((PlayerData::eSpecialFlags)atoi(cellResults[FIELD_PLAYER_SPECIAL].c_str()));
		themPlayer->SetTownHallLevel((PlayerData::eTownHallLevel)atoi(cellResults[FIELD_PLAYER_OPPONENT_TH].c_str()));
		
		warData->AddUsPlayer(usPlayer);
		warData->AddThemPlayer(themPlayer);
	}
	
	cellResults = GetCellsFromLine();
	int attackTotal = 0;
	while (cellResults.size())
	{
		++attackTotal;
		
		// we are attacking them
		if (cellResults[FIELD_ATTACK_US_STARS].length() && cellResults[FIELD_ATTACK_US_PCT].length())
		{
			int opponentId = atoi(cellResults[FIELD_ATTACK_THEM_INDEX].c_str());
			warData->AddAttack(new AttackData(opponentId, (AttackData::StarType)atoi(cellResults[FIELD_ATTACK_US_STARS].c_str()), atoi(cellResults[FIELD_ATTACK_US_PCT].c_str())),
								 atoi(cellResults[FIELD_ATTACK_US_INDEX].c_str())-1);
			
		}
		// they are attacking us
		else if (cellResults[FIELD_ATTACK_THEM_STARS].length() && cellResults[FIELD_ATTACK_THEM_PCT].length())
		{
			int opponentId = atoi(cellResults[FIELD_ATTACK_THEM_INDEX].c_str());
			warData->AddDefend(new AttackData(opponentId, (AttackData::StarType)atoi(cellResults[FIELD_ATTACK_THEM_STARS].c_str()), atoi(cellResults[FIELD_ATTACK_THEM_PCT].c_str())),
								 atoi(cellResults[FIELD_ATTACK_US_INDEX].c_str())-1);
		}
		
		// get next attack/defense
		cellResults = GetCellsFromLine();
	}
}
