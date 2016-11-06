//
//  AttackData.hpp
//  ClashStats
//
//  Created by Atlas on 11/21/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef AttackData_hpp
#define AttackData_hpp

#include "Constants.h"
#include "DataObject.hpp"

class AttackData : public DataObject
{
public:
	enum StarType
	{
		kZeroStar,
		kOneStar,
		kTwoStar,
		kThreeStar,
	};
	
	AttackData(int opponentId, StarType stars, int pctDmg, eTownHallLevel th, int oppWgt = 0);
	AttackData(int opponentId, StarType stars, int pctDmg, eTownHallLevel th, bool isSalt, bool isClose, int attempt, int attackNum);
	AttackData(int opponentId, StarType stars, int pctDmg, eTownHallLevel th, bool isSalt, bool isClose, int attempt, int attackNum, int oppWgt);
	
	const int GetTargetId() const					{ return m_OpponentId; }
	const int GetPctDamage() const					{ return m_PctDamage; }
	const StarType GetStars() const					{ return m_Stars; }
	const eTownHallLevel GetTownHall() const		{ return m_TownHall; }
	const bool IsSalt() const						{ return m_IsSalt; }
	const bool IsClose() const						{ return m_IsClose; }
	const int GetAttemptNumber() const				{ return m_AttemptNumber; }
	const int GetAttackNumber() const				{ return m_AttackNumber; }
	const int GetOpponentWeight() const				{ return m_OpponentWeight; }
	
	void SetClosed(bool closed)						{ m_IsClose = closed; }
	
protected:
	int				m_OpponentId;		// index (1's based) of attacker or defender - based on context
	int				m_OpponentWeight;	
	StarType		m_Stars;
	int				m_PctDamage;
	eTownHallLevel 	m_TownHall;
	bool			m_IsSalt;			// is this attack vs a 'max' defender
	bool			m_IsClose;			// did this attack close the base (first 3-star attack)
	int				m_AttemptNumber;	// this is the 1st, 2nd, 3rd, etc try
	int				m_AttackNumber;		// shows the sequence of attacks within a war
};

#endif /* AttackData_hpp */
