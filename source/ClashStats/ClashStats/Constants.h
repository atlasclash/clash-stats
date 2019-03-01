//
//  Constants.h
//  ClashStats
//
//  Created by Atlas on 11/25/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef Constants_h
#define Constants_h

// town hall levels
enum eTownHallLevel
{
	kTH0 = 0,
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
    kTH12,
};

#define MAX_STARS_PER_ATTACK		(3)
#define MIN_STARS_PER_ATTACK		(0)

#define MIN_PCT_DAMAGE				(0)
#define MAX_PCT_DAMAGE				(100)
#define NEAR_MISS_PCT_THRESHOLD		(95)
#define LOW_PCT_WARNING_THRESHOLD   (25)

#define MIN_ATTACKER_ID				(1)

enum eWarMode
{
    kRegular = 0,
    kCWL
};

/**
 Get the maximum war weight by townhall level

 @param thLvl Townhall level
 @return The maximum war weight of the townhall
 */
int GetMaxTHWeight(const int thLvl);


/**
 Get the minimum war weight by townhall level

 @param thLvl Townhall level
 @return The minimum war weight of the townhall
 */
int GetMinTHWeight(const int thLvl);


void SetMaxPlayerAttacks(const int maxAttacks);
int GetMaxPlayerAttacks();

void SetMaxStarsPerWar(const int maxStarsPerWar);
int GetMaxStarsPerWar();

void SetWarMode(const int warMode);
int GetWarMode();

#endif /* Constants_h */
