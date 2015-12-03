//
//  Database.cpp
//  ClashStats
//
//  Created by Atlas on 11/27/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#include "Database.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <iostream>

#define WAR_DATABASE_NAME				("wardata.sqlite")
#define WAR_DATABASE_SCHEMA_VERSION		(1)

#define TABLE_VERSION					("Version")
#define TABLE_PLAYER_TAG				("PlayerTagTable")
#define TABLE_ATTACK					("AttackTable")
#define TABLE_DEFEND					("DefendTable")
#define TABLE_WAR						("WarTable")
#define TABLE_HISTORIC					("Historic")

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
			"CREATE TABLE 'AttackTable' ('pk' INTEGER PRIMARY KEY AUTOINCREMENT,"
										"'playerTagKey' TEXT,"
										"'warKey' INTEGER,"
										"'playerTH' INTEGER,"
										"'opponentTH' INTEGER,"
										"'starCnt' INTEGER,"
										"'pctDmg' INTEGER,"
										"'isSalt' INTEGER,"
										"'isClose' INTEGER,"
										"'attemptNum' INTEGER );"
			""
			"DROP TABLE IF EXISTS 'DefendTable';"
			"CREATE TABLE 'DefendTable' ('pk' INTEGER PRIMARY KEY AUTOINCREMENT,"
										"'playerTagKey' TEXT,"
										"'warKey' INTEGER,"
										"'playerTH' INTEGER,"
										"'opponentTH' INTEGER,"
										"'starCnt' INTEGER,"
										"'pctDmg' INTEGER );"
			""
			"DROP TABLE IF EXISTS 'WarTable';"
			"CREATE TABLE 'WarTable' ('pk' INTEGER PRIMARY KEY AUTOINCREMENT,"
										"'opponentName' VARCHAR,"
										"'playerCnt' INTEGER,"
										"'usScore' INTEGER,"
										"'themScore' INTEGER,"
										"'date' NUMERIC );"
			""
			"DROP TABLE IF EXISTS 'Historic';"
			"CREATE TABLE 'Historic' ('pk' INTEGER PRIMARY KEY AUTOINCREMENT,"
										"'playerTagKey' TEXT,"
										"'warTotal' INTEGER,"
										"'closerStars' INTEGER,"
										"'holds' INTEGER,"
										"'bleeds' INTEGER,"
										"'nuked' INTEGER,"
										"'starsTotal' INTEGER,"
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
