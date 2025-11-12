#include "gbafe.h"

extern u8 AvaritiaEffectIDLink;

extern bool(*gSkillTester)(Unit* unit, int skillID);

void AvaritiaEffect(BattleUnit* bunitA, BattleUnit* bunitB) {
	if (bunitA == &gBattleActor){
		return;
	}
    
    if (gBattleStats.config & BATTLE_CONFIG_BIT2)
    {
        return;
    }

    if (gSkillTester(&bunitA->unit, AvaritiaEffectIDLink)) //if the first unit has avaritia
    {
        int targetHalfAtk = bunitB->battleAttack;

        bunitA->battleAttack += targetHalfAtk; //they steal half of their enemy's atk
        bunitB->battleAttack -= targetHalfAtk;
    } 
}
