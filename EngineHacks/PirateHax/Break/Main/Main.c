#include  "Main.h"

bool DidUnitBreak(){
	u32* entry = GetUnitDebuffEntry(&gBattleTarget.unit);
	if (CheckBit(entry, BreakBitOffset_Link)){
		return false;
	}
	int j = 0;
	while (BreakExemptCharacterList[j] != 0){
		if (BreakExemptCharacterList[j] == gBattleTarget.unit.pCharacterData->number){
			return false; //if target's char ID is on exempt list, no break allowed
		}
		j++;
	}
	int k = 0;
	while (BreakExemptClassList[k] != 0){
		if (BreakExemptClassList[k] == gBattleTarget.unit.pClassData->number){
			return false; //if target's class ID is on exempt list, no break allowed
		}
		k++;
	}
	if ((gBattleActor.battleAttack > gBattleTarget.battleDefense)){ //did unit do damage
		if (gBattleActor.wTriangleHitBonus > 0 && gBattleTarget.wTriangleHitBonus < 0)
		{
			return true;
		}
		
	}
	//Hand Cannon skill
	if (gSkillTester(&gBattleActor.unit,HandCannonIDLink)) return true;
	return false;
}

void BreakPostBattle(){

	// ignore non combat actions
	if (gActionData.unitActionType != UNIT_ACTION_COMBAT){
		return;
	}

	Unit* target = &gBattleTarget.unit;

	// unset break
	bool alreadyBroken = false;
	u32* entry = GetUnitDebuffEntry(target);
	if (CheckBit(entry, BreakBitOffset_Link)){
		UnsetBit(entry, BreakBitOffset_Link);
		alreadyBroken = true;
	}

	// ignore dead people
	if (target->curHP <= 0){
		return;
	}

	// make sure the enemy is real
	if (!target->pClassData){
		return;
	}

	// try to apply break
	if(DidUnitBreak() && (alreadyBroken == false) && (CheckBit(entry, BreakInBattleBitOffset_Link))){
		SetBit(entry, BreakBitOffset_Link);
	}
	//unset the broken in battle bit at the end
	UnsetBit(entry, BreakInBattleBitOffset_Link);
}

void ClearActiveFactionBreakStatus(){
	Unit* unit;
	int i;
	u32* entry;
	for (i = gChapterData.currentPhase + 1; i < gChapterData.currentPhase + 0x40; i++) {
        unit = GetUnit(i);
		entry = GetUnitDebuffEntry(unit);
        if (UNIT_IS_VALID(unit)){
			UnsetBit(entry, BreakBitOffset_Link);
			UnsetBit(entry, BreakInBattleBitOffset_Link);
		} 
    }
}

void PulverizePreBattle(BattleUnit* unit1, BattleUnit* unit2)
{
	if (gSkillTester(&unit1->unit, PulverizeIDLink))
	{
		u32* entry = GetUnitDebuffEntry(&unit2->unit);
		if (CheckBit(entry, BreakBitOffset_Link))
		{
			unit1->battleCritRate += 40; //add 40 crit to the unit with Pulverize if target is broken
		}
	}
}