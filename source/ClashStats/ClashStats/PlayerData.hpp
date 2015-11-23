//
//  PlayerData.hpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef PlayerData_hpp
#define PlayerData_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "DataObject.hpp"
#include "AttackData.hpp"

class PlayerData : public DataObject
{
public:
	// town hall levels
	enum eTownHallLevel
	{
		kTH0,
		kTH1,
		kTH2,
		kTH3,
		kTH4,
		kTH5,
		kTH6,
		kTH7,
		kTH8,
		kTH9,
		kTH10,
		kTH11,
	};
	
	// special bitflags
	enum eSpecialFlags
	{
		kSpecialNone	= 0,
		kSpecialSalt	= 1 << 1,
	};
	
	// construction / destruction
	PlayerData();
	virtual ~PlayerData();
	
	// accessor methods
	void SetPlayerName(const std::string name);
	std::string GetPlayerName() const;
	
	void SetTownHallLevel(const eTownHallLevel lvl);
	eTownHallLevel GetTownHallLevel() const;
	
	void SetPlayerTag(const std::string tag);
	std::string GetPlayerTag() const;
	
	void SetSpecialFlag(const eSpecialFlags flags);
	bool IsSalt() const;
	
	// methods
	void AddAttack(const AttackData *attack);
	void AddDefend(const AttackData *defend);
	
protected:
	eTownHallLevel				m_TownHallLevel;
	eSpecialFlags				m_SpecialFlag;
	std::string					m_PlayerTag;
	std::string					m_PlayerName;
	std::vector<AttackData>		m_Attacks;
	std::vector<AttackData>		m_Defends;
};

#endif /* PlayerData_hpp */
