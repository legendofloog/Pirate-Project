#include "gbafe.h"

extern u8 MomentumIDLink;
extern bool(*gSkillTester)(Unit* unit, int skillID);

extern s8 WTHitBonus_Link;
extern s8 WTMtBonus_Link;

void Momentum(BattleUnit* bunitA, BattleUnit* bunitB) {
	if(gSkillTester(&bunitA->unit, MomentumIDLink)) //does unit have mov skill
	{
		if(gActionData.moveCount == 0xFF)
		{
			return;
		}
		else if((gActionData.moveCount * 2) >= (bunitA->unit.movBonus)) //has unit moved half or more of their move
		{
			bunitA->wTriangleHitBonus = 1 * WTHitBonus_Link;
			bunitA->wTriangleDmgBonus = 1 * WTMtBonus_Link;
			bunitB->wTriangleHitBonus = -1 * WTHitBonus_Link;
			bunitB->wTriangleDmgBonus = -1 * WTMtBonus_Link;
		}
	}
}