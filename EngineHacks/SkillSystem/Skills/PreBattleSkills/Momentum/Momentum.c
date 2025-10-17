#include "gbafe.h"

extern u8 MomentumIDLink;
extern bool(*gSkillTester)(Unit* unit, int skillID);

extern bool(*gSkillTester)(Unit* unit, int skillID);

void MomentumPreBattle(BattleUnit* bunitA, BattleUnit* bunitB) {
	if(gSkillTester(&bunitA->unit, MomentumIDLink) && (bunitA->unit.pCharacterData->number == gActiveUnit->pCharacterData->number)){ // is this the active unit and do they have momentum
        if (gBattleStats.config & BATTLE_CONFIG_BIT2)
        {

        }
        if(gActionData.moveCount == 0xFF)
		{
			return;
		}
		else if((gActionData.moveCount) >= (bunitA->unit.movBonus)) //has unit moved their full move?
		{
			bunitA->weaponAttributes |= IA_NEGATE_DEFENSE; //then tang ignores their defenses
		}
    }
}
