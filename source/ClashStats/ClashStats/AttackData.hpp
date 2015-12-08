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
	
	AttackData(int opponentId, StarType stars, int pctDmg, eTownHallLevel th);
	AttackData(int opponentId, StarType stars, int pctDmg, eTownHallLevel th, bool isSalt, bool isClose, int attempt);
	
	const int GetTargetId() const	{ return m_OpponentId; }
	const int GetPctDamage() const	{ return m_PctDamage; }
	const StarType GetStars() const { return m_Stars; }
	
protected:
	int				m_OpponentId;		// index (1's based) of attacker or defender - based on context
	StarType		m_Stars;
	int				m_PctDamage;
	eTownHallLevel 	m_TownHall;
	bool			m_IsSalt;			// is this attack vs a 'max' defender
	bool			m_IsClose;			// did this attack close the base (first 3-star attack)
	int				m_AttemptNumber;	// this is the 1st, 2nd, 3rd, etc try
};

#endif /* AttackData_hpp */
