#include "gbafe.h"

extern u8 ArcaneArtificeIDLink;

extern bool(*gSkillTester)(Unit* unit, int skillID);

void ArcaneArtificePreBattle(BattleUnit* bunitA, BattleUnit* bunitB) {
	if (gSkillTester(&bunitA->unit, ArcaneArtificeIDLink)){
        if (gBattleStats.config & BATTLE_CONFIG_BIT2)
        {

        }
        else if (GetItemData(GetItemIndex(bunitA->weapon))->attributes & (IA_MAGIC | IA_MAGICDAMAGE)){
            if (GetItemData(GetItemIndex(bunitB->weapon))->attributes & (IA_MAGIC | IA_MAGICDAMAGE)){
            
            }
            else{
                bunitA->battleAvoidRate += (bunitA->battleAvoidRate / 2);
            }
        }
    }
    if (gSkillTester(&bunitB->unit, ArcaneArtificeIDLink)){
        if (gBattleStats.config & BATTLE_CONFIG_BIT2)
        {

        }
        else if (GetItemData(GetItemIndex(bunitB->weapon))->attributes & (IA_MAGIC | IA_MAGICDAMAGE)){
            if (GetItemData(GetItemIndex(bunitA->weapon))->attributes & (IA_MAGIC | IA_MAGICDAMAGE)){
            
            }
            else{
                bunitB->battleAvoidRate += (bunitB->battleAvoidRate / 2);
            }
        }
    }
}
