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

#ifndef _WIN
class sqlite3;
#else
struct sqlite3;
#endif

struct WarRecord;
struct PlayerWarRecord;
struct AttackRecord;
struct DefendRecord;
struct PlayerRecord;

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
	void	ReadWarsBetweenDates(std::vector<WarRecord> &list, int startDate, int endDate);
	void	ReadWarsWithUserMeta(std::vector<WarRecord> &list, std::string userMeta);
	
	void	ReadAllAttackData(std::vector<AttackRecord> &list);
	
	// read player specific data
	void	ReadAllPlayerAttackData(std::string playerName, std::vector<AttackRecord> &list);
	void	ReadAllPlayerDefendData(std::string playerName, std::vector<DefendRecord> &list);
	void	ReadAllPlayerWarRecordData(std::string playerName, std::vector<PlayerWarRecord> &list);

	// read player specific data using a war meta key
	void	ReadWarAttackData(std::string playerTag, std::string warMeta, std::vector<AttackRecord> &list);
	void	ReadWarDefendData(std::string playerTag, std::string warMeta, std::vector<DefendRecord> &list);
	void	ReadWarRecordData(std::string playerTag, std::string warMeta, std::vector<PlayerWarRecord> &list);
	
	void	ReadPlayerIDsWithWarUserMeta(std::string warMeta, std::vector<int> &list);

	void	ReadPlayerRecord(const int pk, PlayerRecord &record);
	void	ReadPlayerRecord(std::string playerName, PlayerRecord &record);

	// date of our "epoch" (2010-Jan-01)
	boost::gregorian::date	GetEpochDate();
	int						GetTotalSecondsFromEpoch(std::string dateString);
	int						GetTotalSecondsBetweenEpochAndDate(boost::gregorian::date d);
	std::string				StringFromDate(const int seconds);
	
protected:
	bool	IsDatabasePresent() const;
	bool	CreateDatabase();
	int		ReadDatabaseVersion() const;
	
	// Migration routines
	const char *MigrateV1toV2();
	
	
	sqlite3		*m_database;
	
private:
	const char *CreateVersion1();
};

typedef atSingleton<Database> DATABASE;

#endif /* Database_hpp */
