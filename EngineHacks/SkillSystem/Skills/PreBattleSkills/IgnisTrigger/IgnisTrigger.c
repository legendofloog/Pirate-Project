#include "gbafe.h"

#define ITYPE_GUN 7

extern u8 IgnisTriggerIDLink;
extern u8 IgnisTriggerMtLink;

extern bool(*gSkillTester)(Unit* unit, int skillID);
int GetUnitDistance(Unit* firstUnit, Unit* secondUnit);
int abs(int num);

extern s8 WTHitBonus_Link;
extern s8 WTMtBonus_Link;

int GetUnitDistance(Unit* firstUnit, Unit* secondUnit){
	return abs(firstUnit->xPos - secondUnit->xPos) + abs(firstUnit->yPos - secondUnit->yPos);
}

int abs(int num){
	if (num < 0){
		num *= -1;
	}
	return num;
}

void IgnisTriggerPreBattle(BattleUnit* bunitA, BattleUnit* bunitB) {
    if (gBattleStats.config & BATTLE_CONFIG_BIT2)
    {
        if (gSkillTester(&bunitA->unit, IgnisTriggerIDLink))
        {   
            bunitA->battleAttack = GetUnitPower(&bunitA->unit) + IgnisTriggerMtLink; //we're in the stat screen for it: show normal atk
        }
        return; 
    }
    if (bunitB->unit.pClassData->number == 0)
    {
        return;
    }
    
    if (gSkillTester(&bunitA->unit, IgnisTriggerIDLink)){
        if (GetUnitDistance(&bunitA->unit, &bunitB->unit) == 1) //if they are adjacent
        {
            bunitA->battleAttack = GetUnitPower(&bunitA->unit) + IgnisTriggerMtLink; //use str
        }
        else
        {
            bunitA->battleAttack = GetUnitSkill(&bunitA->unit) + IgnisTriggerMtLink; //use skl
        }

        if (bunitA->wTriangleHitBonus == WTHitBonus_Link)
        {
            bunitA->battleHitRate += WTHitBonus_Link;
            bunitA->battleAttack += WTMtBonus_Link;
            bunitB->battleHitRate -= WTHitBonus_Link;
            bunitB->battleAttack -= WTMtBonus_Link;
        }
        else if (bunitB->wTriangleHitBonus == WTHitBonus_Link)
        {
            bunitB->battleHitRate += WTHitBonus_Link;
            bunitB->battleAttack += WTMtBonus_Link;
            bunitA->battleHitRate -= WTHitBonus_Link;
            bunitA->battleAttack -= WTMtBonus_Link;
        }
    }
}