//
//  AttackData.hpp
//  ClashStats
//
//  Created by Atlas on 11/21/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef AttackData_hpp
#define AttackData_hpp

#include <stdio.h>
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
	
	const int GetTargetId() const	{ return m_OpponentId; }
	const int GetPctDamage() const	{ return m_PctDamage; }
	const StarType GetStars() const { return m_Stars; }
	
protected:
	int				m_OpponentId;		// index (1's based) of attacker or defender - based on context
	StarType		m_Stars;
	int				m_PctDamage;
	eTownHallLevel 	m_TownHall;
};

#endif /* AttackData_hpp */
