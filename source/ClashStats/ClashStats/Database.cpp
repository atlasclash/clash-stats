//
//  Database.cpp
//  ClashStats
//
//  Created by Atlas on 11/27/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "Database.hpp"
#include "WarRecord.hpp"
#include "PlayerWarRecord.hpp"
#include "AttackRecord.hpp"
#include "DefendRecord.hpp"
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "sqlite3.h"
#ifndef _WIN
#include <unistd.h>
#endif

#define WAR_DATABASE_NAME				("wardata.sqlite")
#define WAR_DATABASE_SCHEMA_VERSION		(1)

#define TABLE_VERSION					("Version")
#define TABLE_PLAYER_TAG				("PlayerTagTable")
#define TABLE_ATTACK					("AttackTable")
#define TABLE_DEFEND					("DefendTable")
#define TABLE_WAR						("WarTable")
#define TABLE_HISTORIC					("Historic")

#ifndef YES
#define YES								(1)
#endif

#ifndef NO
#define NO								(0)
#endif

static boost::gregorian::date s_epoch(boost::gregorian::from_simple_string("2010/01/01"));

boost::gregorian::date Database::GetEpochDate()
{
	return s_epoch;
}

std::string Database::StringFromDate(const int seconds)
{
	boost::posix_time::ptime posixTimeSec(s_epoch, boost::posix_time::time_duration(0,0,seconds));
	return boost::posix_time::to_simple_string(posixTimeSec);
}

int Database::GetTotalSecondsFromEpoch(std::string dateString)
{
	boost::gregorian::date d(boost::gregorian::from_simple_string(dateString));
	return GetTotalSecondsBetweenEpochAndDate(d);
}

int Database::GetTotalSecondsBetweenEpochAndDate(boost::gregorian::date d)
{
	boost::posix_time::ptime posixTimeDateA(d);
	boost::posix_time::ptime posixTimeEpochDate(s_epoch);
	boost::posix_time::time_duration td = posixTimeDateA - posixTimeEpochDate;
	
	return td.total_seconds();
}

bool Database::IsDatabasePresent() const
{
	struct stat buffer;
	return (stat(WAR_DATABASE_NAME, &buffer) == 0);
}


bool Database::OpenDatabase()
{
	if (IsDatabasePresent())
	{
		if (sqlite3_open(WAR_DATABASE_NAME, &m_database) != SQLITE_OK)
		{
			m_database = NULL;
			return false;
		}
		
		// check to see if it is the most recent?
		
		// TODO
		
		return true;
	}
	
	return CreateDatabase();
}

bool Database::CreateDatabase()
{
	char *errMsg;
	sqlite3_open(WAR_DATABASE_NAME, &m_database);
	int result = sqlite3_exec(m_database, CreateVersion1(), NULL, NULL, &errMsg);
	return (result == SQLITE_OK);
}

const char* Database::CreateVersion1()
{
	return 	"DROP TABLE IF EXISTS 'Version';"
			"CREATE TABLE 'Version' ('version' INTEGER NOT NULL );"
			"INSERT INTO 'Version' VALUES(1);"
			""
			"DROP TABLE IF EXISTS 'PlayerTagTable';"
			"CREATE TABLE 'PlayerTagTable' ('pk' INTEGER PRIMARY KEY AUTOINCREMENT,"
										"'playerTag' TEXT UNIQUE,"
										"'playerName' TEXT );"
			""
			"DROP TABLE IF EXISTS 'AttackTable';"
			"CREATE TABLE 'AttackTable' ('pk' INTEGER PRIMARY KEY AUTOINCREMENT,"		// primary key
										"'playerTagKey' TEXT,"							// player tag key (foreign key)
										"'warKey' INTEGER,"								// war key (foreign key)
										"'attackNum' INTEGER,"
										"'playerTH' INTEGER,"							// player's TH level
										"'opponentTH' INTEGER,"							// opponent's TH level
										"'starCnt' INTEGER,"							// stars earned
										"'pctDmg' INTEGER,"								// percent destruction
										"'isSalt' INTEGER,"								// is this a 'salt' base?
										"'isClose' INTEGER,"							// was the attack a 'close'?
										"'attemptNum' INTEGER );"						// number of attempts on the base
			""
			"DROP TABLE IF EXISTS 'DefendTable';"
			"CREATE TABLE 'DefendTable' ('pk' INTEGER PRIMARY KEY AUTOINCREMENT,"		// primary key
										"'playerTagKey' TEXT,"							// player tag key (foreign key)
										"'warKey' INTEGER,"								// war key (foreign key)
										"'playerTH' INTEGER,"							// player's TH level
										"'opponentTH' INTEGER,"							// opponent's TH level
										"'starCnt' INTEGER,"							// stars earned
										"'pctDmg' INTEGER );"							// percent destruction
			""
			"DROP TABLE IF EXISTS 'WarTable';"
			"CREATE TABLE 'WarTable' ('pk' INTEGER PRIMARY KEY AUTOINCREMENT,"
										"'opponentName' VARCHAR,"
										"'opponentTag' VARCHAR,"
										"'playerCnt' INTEGER,"
										"'usScore' INTEGER,"
										"'themScore' INTEGER,"
										"'date' NUMERIC );"								// date stored in seconds since 'epoch'
			""
			"DROP TABLE IF EXISTS 'PlayerWarTable';"									// stores aggregate totals per war
			"CREATE TABLE 'PlayerWarTable' ('pk' INTEGER PRIMARY KEY AUTOINCREMENT,"	// primary key
										"'playerTagKey' TEXT,"							// player tag key (foreign key)
										"'warKey' INTEGER,"								// war key (foreign key)
										"'playerTH' INTEGER,"
										"'closerStars' INTEGER,"						// closer stars
										"'holds' INTEGER,"								// holds
										"'bleeds' INTEGER,"								// bleeds
										"'nuked' INTEGER,"								// nuked
										"'totalStars' INTEGER,"							// total stars
										"'threeStars' INTEGER );"						// three star attacks
			""
			"DROP TABLE IF EXISTS 'Historic';"											// table to store summary data to bootstrap missing war info
			"CREATE TABLE 'Historic' ('pk' INTEGER PRIMARY TEXT AUTOINCREMENT,"
										"'playerTagKey' TEXT,"
										"'warTotal' INTEGER,"							// total wars participated
										"'closerStars' INTEGER,"
										"'holds' INTEGER,"
										"'bleeds' INTEGER,"
										"'nuked' INTEGER,"
										"'starsTotal' INTEGER,"
										"'threeStars' INTEGER,"
										"'mia' INTEGER,"
										"'scout' INTEGER );"
	;
}

void Database::WritePlayerTags(std::vector<PlayerData> list)
{
	// ref: http://stackoverflow.com/questions/15277373/sqlite-upsert-update-or-insert
	//
	//																		  1 				2
	const char *unused;
	std::string update_player_tag_sql = "UPDATE PlayerTagTable SET playerName=? WHERE playerTag=?";
	std::string insert_player_tag_sql = "INSERT OR IGNORE INTO PlayerTagTable (playerName, playerTag) VALUES (?, ?)";
	
	sqlite3_stmt *update_statement;
	sqlite3_stmt *insert_statement;
	
	sqlite3_prepare_v2(m_database, update_player_tag_sql.c_str(), (int)update_player_tag_sql.length(), &update_statement, &unused);
	sqlite3_prepare_v2(m_database, insert_player_tag_sql.c_str(), (int)insert_player_tag_sql.length(), &insert_statement, &unused);

	for (int i = 0; i < list.size(); ++i)
	{
		PlayerData pd = list[i];
		
		sqlite3_bind_text(update_statement, 1, pd.GetPlayerName().c_str(), (int)pd.GetPlayerName().length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(update_statement, 2, pd.GetPlayerTag().c_str(), (int)pd.GetPlayerTag().length(), SQLITE_TRANSIENT);
		
		sqlite3_bind_text(insert_statement, 1, pd.GetPlayerName().c_str(), (int)pd.GetPlayerName().length(), SQLITE_TRANSIENT);
		sqlite3_bind_text(insert_statement, 2, pd.GetPlayerTag().c_str(), (int)pd.GetPlayerTag().length(), SQLITE_TRANSIENT);
		
		sqlite3_step(update_statement);
		sqlite3_step(insert_statement);
		
		sqlite3_reset(update_statement);
		sqlite3_reset(insert_statement);
	}
	
	sqlite3_finalize(update_statement);
	sqlite3_finalize(insert_statement);
}

void Database::WriteWarRecord(WarRecord &warRecord)
{
	warRecord.pk = 0;
	
	std::string insert_war_sql = "INSERT INTO WarTable (opponentName, opponentTag, playerCnt, usScore, themScore, date) VALUES (?, ?, ?, ?, ?, ?)";
	sqlite3_stmt *insert_statement;

	const char *unused;
	sqlite3_prepare_v2(m_database, insert_war_sql.c_str(), (int)insert_war_sql.length(), &insert_statement, &unused);
	
	sqlite3_bind_text(insert_statement, 1, warRecord.opponentName.c_str(), (int)warRecord.opponentName.length(), SQLITE_TRANSIENT);
	sqlite3_bind_text(insert_statement, 2, warRecord.opponentTag.c_str(), (int)warRecord.opponentTag.length(), SQLITE_TRANSIENT);
	sqlite3_bind_int(insert_statement, 3, warRecord.playerCount);
	sqlite3_bind_int(insert_statement, 4, warRecord.usScore);
	sqlite3_bind_int(insert_statement, 5, warRecord.themScore);
	sqlite3_bind_int(insert_statement, 6, warRecord.date);
	
	sqlite3_step(insert_statement);
	warRecord.pk = (int)sqlite3_last_insert_rowid(m_database);
	
	sqlite3_finalize(insert_statement);
}

void Database::WritePlayerWarRecord(PlayerWarRecord &playerWarRecord)
{
	playerWarRecord.pk = 0;
	
	std::string insert_sql = "INSERT INTO PlayerWarTable (playerTagKey, warKey, playerTH, closerStars, holds, bleeds, nuked, totalStars, threeStars) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
	sqlite3_stmt *insert_statement;
	
	const char *unused;
	sqlite3_prepare_v2(m_database, insert_sql.c_str(), (int)insert_sql.length(), &insert_statement, &unused);
	
	sqlite3_bind_text(insert_statement, 1, playerWarRecord.playerTagKey.c_str(), (int)playerWarRecord.playerTagKey.length(), SQLITE_TRANSIENT);
	sqlite3_bind_int(insert_statement, 2, playerWarRecord.warKey);
	sqlite3_bind_int(insert_statement, 3, playerWarRecord.playerTH);
	sqlite3_bind_int(insert_statement, 4, playerWarRecord.closerStars);
	sqlite3_bind_int(insert_statement, 5, playerWarRecord.holds);
	sqlite3_bind_int(insert_statement, 6, playerWarRecord.bleeds);
	sqlite3_bind_int(insert_statement, 7, playerWarRecord.nuked);
	sqlite3_bind_int(insert_statement, 8, playerWarRecord.totalStars);
	sqlite3_bind_int(insert_statement, 9, playerWarRecord.threeStars);
	
	sqlite3_step(insert_statement);
	playerWarRecord.pk = (int)sqlite3_last_insert_rowid(m_database);
	
	sqlite3_finalize(insert_statement);
}

void Database::WritePlayerAttackRecord(AttackRecord &attackRecord)
{
	attackRecord.pk = 0;
	
	std::string insert_sql = "INSERT INTO AttackTable (playerTagKey, warKey, attackNum, playerTH, opponentTH, starCnt, pctDmg, isSalt, isClose, attemptNum) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
	sqlite3_stmt *insert_statement;
	
	const char *unused;
	sqlite3_prepare_v2(m_database, insert_sql.c_str(), (int)insert_sql.length(), &insert_statement, &unused);
	
	sqlite3_bind_text(insert_statement, 1, attackRecord.playerTagPk.c_str(), (int)attackRecord.playerTagPk.length(), SQLITE_TRANSIENT);
	sqlite3_bind_int(insert_statement, 2, attackRecord.warPk);
	sqlite3_bind_int(insert_statement, 3, attackRecord.attackNum);
	sqlite3_bind_int(insert_statement, 4, attackRecord.playerTH);
	sqlite3_bind_int(insert_statement, 5, attackRecord.opponentTH);
	sqlite3_bind_int(insert_statement, 6, attackRecord.starCount);
	sqlite3_bind_int(insert_statement, 7, attackRecord.percentDmg);
	sqlite3_bind_int(insert_statement, 8, (attackRecord.isSalt)?YES:NO);
	sqlite3_bind_int(insert_statement, 9, (attackRecord.isClose)?YES:NO);
	sqlite3_bind_int(insert_statement, 10, attackRecord.attemptNum);
	
	sqlite3_step(insert_statement);
	attackRecord.pk = (int)sqlite3_last_insert_rowid(m_database);
	
	sqlite3_finalize(insert_statement);
}

void Database::WritePlayerDefendRecord(DefendRecord &defendRecord)
{
	defendRecord.pk = 0;
	
	std::string insert_sql = "INSERT INTO DefendTable (playerTagKey, warKey, playerTH, opponentTH, starCnt, pctDmg) VALUES (?, ?, ?, ?, ?, ?)";
	sqlite3_stmt *insert_statement;
	
	const char *unused;
	sqlite3_prepare_v2(m_database, insert_sql.c_str(), (int)insert_sql.length(), &insert_statement, &unused);
	
	sqlite3_bind_text(insert_statement, 1, defendRecord.playerTagPk.c_str(), (int)defendRecord.playerTagPk.length(), SQLITE_TRANSIENT);
	sqlite3_bind_int(insert_statement, 2, defendRecord.warPk);
	sqlite3_bind_int(insert_statement, 3, defendRecord.playerTH);
	sqlite3_bind_int(insert_statement, 4, defendRecord.opponentTH);
	sqlite3_bind_int(insert_statement, 5, defendRecord.starCount);
	sqlite3_bind_int(insert_statement, 6, defendRecord.percentDmg);
	
	sqlite3_step(insert_statement);
	defendRecord.pk = (int)sqlite3_last_insert_rowid(m_database);
	
	sqlite3_finalize(insert_statement);
}

void Database::ReadAllWars(std::vector<WarRecord> &list)
{
	std::string sql = "SELECT * FROM WarTable;";
	sqlite3_stmt *statement;
	
	const char *unused;
	sqlite3_prepare_v2(m_database, sql.c_str(), (int)sql.length(), &statement, &unused);
	
	while (sqlite3_step(statement) == SQLITE_ROW)
	{
		WarRecord war;
		
		war.pk				= sqlite3_column_int(statement, 0);
		war.opponentName	= std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)));
		war.opponentTag		= std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)));
		war.playerCount		= sqlite3_column_int(statement, 3);
		war.usScore			= sqlite3_column_int(statement, 4);
		war.themScore		= sqlite3_column_int(statement, 5);
		war.date			= sqlite3_column_int(statement, 6);
		
		list.push_back(war);
	}
	
	sqlite3_finalize(statement);
}

void Database::ReadWarsBetweenDates(std::vector<WarRecord> &list, int startDate, int endDate)
{
	std::string sql = "SELECT * FROM WarTable WHERE date >= ? AND date <= ? ORDER BY date ASC;";
	sqlite3_stmt *statement;
	
	const char *unused;
	sqlite3_prepare_v2(m_database, sql.c_str(), (int)sql.length(), &statement, &unused);
	
	sqlite3_bind_int(statement, 1, startDate);
	sqlite3_bind_int(statement, 2, endDate);
	
	while (sqlite3_step(statement) == SQLITE_ROW)
	{
		WarRecord war;

		war.pk				= sqlite3_column_int(statement, 0);
		war.opponentName	= std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)));
		war.opponentTag		= std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)));
		war.playerCount		= sqlite3_column_int(statement, 3);
		war.usScore			= sqlite3_column_int(statement, 4);
		war.themScore		= sqlite3_column_int(statement, 5);
		war.date			= sqlite3_column_int(statement, 6);
		
		list.push_back(war);
	}
	
	sqlite3_finalize(statement);
}

