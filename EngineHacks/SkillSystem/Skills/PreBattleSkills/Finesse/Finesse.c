#include "gbafe.h"

extern bool SkillTester(BattleUnit* unit, u8 skillID);
extern u8 FinesseID_Link;

void Finesse(BattleUnit* bunitA, BattleUnit* bunitB) {
	if (SkillTester(bunitA, FinesseID_Link)) {
		bunitA->battleCritRate += (bunitA->unit.skl/2);
	}
}

