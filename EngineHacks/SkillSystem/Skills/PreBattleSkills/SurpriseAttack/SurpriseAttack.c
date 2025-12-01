#include "gbafe.h"

extern u8 SurpriseAttackIDLink;

extern bool(*gSkillTester)(Unit* unit, int skillID);

void SurpriseAttackPreBattle(BattleUnit* bunitA, BattleUnit* bunitB) {
	if (gBattleStats.config & BATTLE_CONFIG_BIT2)
    {
        return;
    }

    if (bunitB->unit.pClassData->number == 0)
    {
        return; //shouldn't do it if the second unit doesn't exist yet
    }
    
    if (gSkillTester(&bunitA->unit, SurpriseAttackIDLink) && !(bunitB->canCounter)){
        bunitA->weaponAttributes |= IA_NEGATE_DEFENSE;
    }
}

