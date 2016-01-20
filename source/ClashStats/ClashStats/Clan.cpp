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
#include "Database.hpp"

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

void Clan::CreateClanWithUserMeta(std::string userMeta)
{
	DATABASE::GetInstance().ReadWarsWithUserMeta(m_WarRecordList, userMeta);
	printf("count: %lu\n", m_WarRecordList.size());
}