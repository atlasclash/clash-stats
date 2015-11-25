//
//  WarData.hpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef WarData_hpp
#define WarData_hpp

#include <stdio.h>
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

	void SetClanName(const std::string name){ m_OpponentClanName = name; }
	void SetClanTag(const std::string tag)	{ m_OpponentClanTag = tag; }
	
	void AddUsPlayer(const PlayerData *us);
	const int GetUsPlayerCount() const;
	
	void AddThemPlayer(const PlayerData *them);
	const int GetThemPlayerCount() const;
	
	void AddClanAttack(const AttackData *ad, const int attackerIndex);
	void AddThemAttack(const AttackData *ad, const int attackerIndex);
	
	void AddClanDefend(const AttackData *def, const int defenderIndex);
	void AddThemDefend(const AttackData *def, const int defenderIndex);
	
protected:
	int							m_WarSize;										// number of players in the war
	std::string					m_OpponentClanName;								// opponent clan name
	std::string					m_OpponentClanTag;								// opponent clan tag, optional
	std::string					m_DateStr;										// date of war (mm/dd/yy)
	std::vector<PlayerData>		m_ThemList;										// list of players in enemy clan
	std::vector<PlayerData>		m_UsList;										// list of players in our clan
};

#endif /* WarData_hpp */
