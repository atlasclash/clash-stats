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
#include "DataObject.hpp"

class PlayerData : public DataObject
{
public:
	enum eTownHallLevel
	{
		kTH7,
		kTH8,
		kTH9,
		kTH10,
	};
	
	
protected:
	std::string			m_PlayerTag;
	eTownHallLevel		m_TownHallLevel;
	std::string			m_PlayerName;
};

#endif /* PlayerData_hpp */
