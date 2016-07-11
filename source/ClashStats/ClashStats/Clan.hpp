//
//  Clan.hpp
//  ClashStats
//
//  Created by Atlas on 12/12/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef Clan_hpp
#define Clan_hpp

#include <stdio.h>
#include <vector>

class Player;
struct WarRecord;

class Clan
{
public:
	Clan();
	virtual ~Clan();
	
	void CreateClanWithUserMeta(std::string userMeta);
	void CreateWarRecord();
	
	void Reset();
	
protected:
	std::vector<WarRecord> m_WarRecordList;
	std::vector<Player*> m_PlayerList;
};

#endif /* Clan_hpp */
