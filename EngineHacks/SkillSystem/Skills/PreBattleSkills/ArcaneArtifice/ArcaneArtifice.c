#include "gbafe.h"

extern u8 ArcaneArtificeIDLink;

extern bool(*gSkillTester)(Unit* unit, int skillID);

void ArcaneArtificePreBattle(BattleUnit* bunitA, BattleUnit* bunitB) {
	if (gBattleStats.config & BATTLE_CONFIG_BIT2)
    {
        return;
    }

    if (bunitB->unit.pClassData->number == 0)
    {
        return; //shouldn't do it if the second unit doesn't exist yet
    }
    
    if (gSkillTester(&bunitA->unit, ArcaneArtificeIDLink)){
        if (GetItemData(GetItemIndex(bunitA->weapon))->attributes & (IA_MAGIC | IA_MAGICDAMAGE)){
            if (GetItemData(GetItemIndex(bunitB->weapon))->attributes & (IA_MAGIC | IA_MAGICDAMAGE)){
            
            }
            else{
                bunitA->battleAvoidRate += (bunitA->battleAvoidRate / 2);
            }
        }
    }
}
