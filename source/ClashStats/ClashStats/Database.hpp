//
//  Database.hpp
//  ClashStats
//
//  Created by Atlas on 11/27/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef Database_hpp
#define Database_hpp

#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include "Singleton.h"
#include "PlayerData.hpp"

class sqlite3;
struct WarRecord;
struct PlayerWarRecord;
struct AttackRecord;
struct DefendRecord;

class Database
{
public:
	enum
	{
		INVALID_KEY = -1,
	};
	
	// open the database, creating it if necessary
	bool	OpenDatabase();
	
	// write player tags from a war
	void	WritePlayerTags(std::vector<PlayerData> list);
	
	// write war data
	void	WriteWarRecord(WarRecord &warRecord);
	
	// write player war record
	void	WritePlayerWarRecord(PlayerWarRecord &playerWarRecord);
	
	// write player attack record
	void	WritePlayerAttackRecord(AttackRecord &attackRecord);
	
	// write player defend record
	void	WritePlayerDefendRecord(DefendRecord &defendRecord);
	
	// read all wars
	void	ReadAllWars(std::vector<WarRecord> &list);
	
	// date of our "epoch" (2010-Jan-01)
	boost::gregorian::date	GetEpochDate();
	std::string				StringFromDate(const int seconds);
	
protected:
	bool	IsDatabasePresent() const;
	bool	CreateDatabase();
	
	
	sqlite3		*m_database;
	
private:
	const char *CreateVersion1();
};

typedef atSingleton<Database> DATABASE;

#endif /* Database_hpp */
