//
//  PlayerData.hpp
//  ClashStats
//
//  Created by Atlas on 11/14/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef PlayerData_hpp
#define PlayerData_hpp

#include <string>
#include <vector>
#include "Constants.h"
#include "DataObject.hpp"
#include "AttackData.hpp"

#define MAX_PLAYER_ATTACKS		(2)

class PlayerData : public DataObject
{
public:
	
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
	
	void SetCloserStars(const int stars);
	const int GetCloserStars() const;
	
	void SetSpecialFlag(const eSpecialFlags flags);
	bool IsSalt() const;
	
	// methods
	void AddAttack(const AttackData *attack);
	const std::vector<AttackData> GetAttacks() const;
	void AddDefend(const AttackData *defend);
	const std::vector<AttackData> GetDefends() const;
	
	const unsigned long GetAttackCount() const;
	const int GetMaxStarsGiven() const;
	const int GetTotalStars() const;
	
	const AttackData* GetCloserAttack() const;
	
protected:
	int 						m_CloserStars;
	eTownHallLevel				m_TownHallLevel;
	eSpecialFlags				m_SpecialFlag;
	std::string					m_PlayerTag;
	std::string					m_PlayerName;
	std::vector<AttackData>		m_Attacks;
	std::vector<AttackData>		m_Defends;
};

#endif /* PlayerData_hpp */
