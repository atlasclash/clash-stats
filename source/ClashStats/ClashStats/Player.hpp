//
//  Player.hpp
//  ClashStats
//
//  Created by Atlas on 12/12/15.
//  Copyright © 2015 JTJ. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <vector>
#include <string>
#include "Constants.h"

struct PlayerWarRecord;
struct AttackRecord;
struct DefendRecord;

struct PlayerAttackSummary
{
	PlayerAttackSummary();
	void Reset();
	bool isEqual(const PlayerAttackSummary a) const;

	int m_TotalAttacks;
	int	m_TotalStars;
	int m_TotalPctDmg;
	int m_TotalThreeStars;
	int m_TotalCloserStars;
	int	m_TotalNearMiss;			// 95% dmg >
	
	// same TH vs TH level
	int m_NumSaltAttacks;
	int m_NumSaltStars;
	int m_NumSaltPctDmg;
	int m_NumSaltThreeStars;
	int m_NumSaltCloserStars;
	int m_NumSaltFirstAttempts;		// 3-star on the first attempt
	
	// same TH vs TH level
	int m_NumBootAttacks;
	int m_NumBootStars;
	int m_NumBootPctDmg;
	int m_NumBootThreeStars;
	int m_NumBootCloserStars;
	int m_NumBootFirstAttempts;
	
	// player TH > opponent TH
	int m_NumNukeAttacks;
	int m_NumNukeStars;
	int m_NumNukePctDmg;
	int m_NumNukeThreeStars;
	int m_NumNukeCloserStars;
	int m_NumNukeFirstAttempts;
	
	// player TH < opponent TH
	int m_NumSnipeAttacks;
	int m_NumSnipeStars;
	int m_NumSnipePctDmg;
	int m_NumSnipeThreeStars;
	int m_NumSnipeCloserStars;
	int m_NumSnipeFirstAttempts;
};

struct PlayerDefendSummary
{
	PlayerDefendSummary();
	void Reset();
	bool isEqual(const PlayerDefendSummary d) const;
	
	int m_TotalDefends;
	int m_TotalStarsYielded;
	int m_TotalDamage;
	int m_TotalThreeStars;
	int m_TotalMisses;
	
	int m_NumPeerAttacks;
	int m_NumPeerStarsYielded;
	int m_NumPeerDamage;
	int m_NumPeerThreeStars;
	int m_NumPeerMisses;
	
	int m_NumSnipeAttacks;
	int m_NumSnipeStarsYielded;
	int m_NumSnipeDamage;
	int m_NumSnipeThreeStars;
	int m_NumSnipeMisses;
	
	int m_NumNukeAttacks;
	int m_NumNukeStarsYielded;
	int m_NumNukeDamage;
	int m_NumNukeThreeStars;
	int m_NumNukeMisses;
};

struct PlayerWarSummary
{
	PlayerWarSummary();
	void Reset();
	bool isEqual(const PlayerWarSummary w) const;
	
	int m_TotalWars;
	int m_TotalCloserStars;
	int m_TotalHolds;
	int m_TotalBleeds;
	int m_TotalNuked;
	int m_TotalStars;
	int m_TotalThreeStars;
};

class Player
{
public:
	Player();
	Player(const int pk);
	virtual ~Player();
	
	std::string GetName() const { return m_Name; }
	std::string GetTag() const  { return m_Tag;  }
	
	void Reset();
	bool Compare(const Player *p) const;

	void GenerateHistoryWithName(std::string name);
	void GenerateSeasonHistoryWithMeta(std::string warMeta);
	
	void AddWarRecord(const PlayerWarRecord &record);
	void AddAttackRecord(const AttackRecord &record);
	void AddDefendRecord(const DefendRecord &record);
	
	void WritePlayerStatsData(std::ofstream &outputFile);
	
protected:
	void GenerateAttackSummaryData();
	void GenerateDefendSummaryData();
	void GenerateWarSummaryData();
	
	std::vector<PlayerWarRecord> m_WarRecordList;
	std::vector<AttackRecord> m_AttackRecordList;
	std::vector<DefendRecord> m_DefendRecordList;
	
	std::string m_Name;
	std::string m_Tag;
	int			m_Key;
	PlayerAttackSummary m_AttackSummary[eTownHallLevel::kTH11+1];
	PlayerDefendSummary m_DefendSummary[eTownHallLevel::kTH11+1];
	PlayerWarSummary m_WarSummary[eTownHallLevel::kTH11+1];
};

#endif /* Player_hpp */
