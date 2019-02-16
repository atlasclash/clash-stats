//
//  WarData.hpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef WarData_hpp
#define WarData_hpp

#include <string>
#include <vector>
#include "DataObject.hpp"
#include "PlayerData.hpp"

class AttackData;

class WarData : public DataObject
{
public:
	// Constructor / Destructor
	WarData() {}
	WarData(std::string clanName, std::string clanTag);
	virtual ~WarData();
	
	// Access methods
	void SetWarSize(const int size)			{ m_WarSize = size; }
	int GetWarSize() const					{ return m_WarSize; }
	
	void SetWarDate(const std::string date) { m_DateStr = date; }
	std::string GetWarDate() const			{ return m_DateStr; }

	void SetThemClanName(const std::string name){ m_OpponentClanName = name; }
	void SetThemClanTag(const std::string tag)	{ m_OpponentClanTag = tag; }
	
	void SetUsClanName(const std::string name)	{ m_UsClanName = name; }
	void SetUsClanTag(const std::string tag)	{ m_UsClanTag = tag; }
	
	void AddUsPlayer(const PlayerData *us);
	const int GetUsPlayerCount() const;
	
	void AddThemPlayer(const PlayerData *them);
	const int GetThemPlayerCount() const;
	
	void AddUsAttack(const AttackData *ad, const int attackerIndex);
	void AddThemAttack(const AttackData *ad, const int attackerIndex);
	
	void AddUsDefend(const AttackData *def, const int defenderIndex);
	void AddThemDefend(const AttackData *def, const int defenderIndex);
	
	void SetUserMeda(const std::string meta) { m_UserDefinedMeta = meta; }
	std::string GetUserDefinedMeta() const	{ return m_UserDefinedMeta; }
	
	std::vector<PlayerData> GetUsList() { return m_UsList; }
	std::vector<PlayerData> GetThemList() { return m_ThemList; }
	
	// 1s based routines
	PlayerData* GetThem(const unsigned int which);
	PlayerData* GetUs(const unsigned int which);
	
	// 1s based routines
	eTownHallLevel GetUsTHLevel(const int usId) const;
	eTownHallLevel GetThemTHLevel(const int themId) const;
	
	// calculates post-read war stats
	void CalcWarStats();
	void CalcCloserStars();
	
	// displays results of the war stats
	void RunReports() const;
	void ReportFinalScore() const;
	void ReportPlayerStats() const;
	void ReportWarningMissingInAction() const;
	void ReportWarningNuke() const;
	void ReportWarningSnipe() const;
	
	bool SaveWarToDB();
    
    void CheckDataRanges() const;
	
protected:
	void CalcTotalScores();

	int GetTotalSecondsFromEpochOfWarDate() const;
	
	int							m_WarSize;										// number of players in the war
	std::string					m_UserDefinedMeta;								// optional field for meta-data
	std::string					m_OpponentClanName;								// opponent clan name
	std::string					m_OpponentClanTag;								// opponent clan tag, optional
	std::string					m_UsClanName;									// our clan name
	std::string					m_UsClanTag;									// our clan tag
	std::string					m_DateStr;										// date of war (mm/dd/yy)
	std::vector<PlayerData>		m_ThemList;										// list of players in enemy clan
	std::vector<PlayerData>		m_UsList;										// list of players in our clan
	
	int							m_UsScore;										// must call CalcWarStats()
	int							m_ThemScore;									// must call CalcWarStats()
};

#endif /* WarData_hpp */
