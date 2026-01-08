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
        int targetAtkReduction = bunitB->battleAttack / 4;

        bunitA->battleAttack += targetAtkReduction; //they steal half of their enemy's atk
        bunitB->battleAttack -= targetAtkReduction;
    }
    else if (gSkillTester(&bunitB->unit, AvaritiaEffectIDLink)) //if the first unit has avaritia
    {
        int targetAtkReduction = bunitA->battleAttack / 4;

        bunitB->battleAttack += targetAtkReduction; //they steal half of their enemy's atk
        bunitA->battleAttack -= targetAtkReduction;
    } 
}
