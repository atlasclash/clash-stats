//
//  Database.hpp
//  ClashStats
//
//  Created by Atlas on 11/27/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef Database_hpp
#define Database_hpp

#include <vector>
#include "Singleton.h"
#include "sqlite3.h"
#include "PlayerData.hpp"

class Database
{
public:
	bool	OpenDatabase();
	
	// write player tags from a war
	void	WritePlayerTags(std::vector<PlayerData> list);
	
protected:
	bool	IsDatabasePresent() const;
	bool	CreateDatabase();
	
	sqlite3		*m_database;
	
private:
	const char *CreateVersion1();
};

typedef atSingleton<Database> DATABASE;

#endif /* Database_hpp */
