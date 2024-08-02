#include "gbafe.h"

extern bool SkillTester(Unit* unit, u8 skill);
extern int AttackCommandUsability(MenuCommandDefinition* command, int number);
extern int (*UnitActionMenu_Attack)(MenuProc* menu, MenuCommandProc* menuItem);
extern u8 ReapingBladeID_Link;
extern u8 ReapingBladeAttackType_Link;
extern void GaleforceEvent;
extern u8* AttackTypeByte_Link;


int ReapingBladeUsability(MenuCommandDefinition* command, int number) {
	//true if can attack and has reaping blade skill
	if (!SkillTester(gActiveUnit, ReapingBladeID_Link)) return MCA_NONUSABLE;
	return AttackCommandUsability(command, number);
}

int ReapingBladeEffect(MenuProc* menu, MenuCommandProc* menuItem) {
	//set attack type to reaping blade, call attack effect
	*AttackTypeByte_Link = ReapingBladeAttackType_Link;
	
	return UnitActionMenu_Attack(menu, menuItem);
}

void Proc_ReapingBlade(BattleUnit* bunitA, BattleUnit* bunitB, struct BattleHit* curRound, struct BattleStats* stats) {
	//if reaping blade is active (this means that we are bunitA)
	if (*AttackTypeByte_Link == ReapingBladeAttackType_Link
		&& bunitA == &gBattleActor
	) {
		//if we will kill this round		
		//relevant info for that isn't calculated until after this runs,
		//so do the calculation here to check
		if ( 
			!(curRound->attributes & BATTLE_HIT_ATTR_MISS)
			&& bunitB->unit.curHP - (bunitA->battleAttack - bunitB->battleDefense) <= 0
		) {
			//moving again handled post-battle; set a flag to check then
			
			
			//destroy equipped weapon
			bunitA->weapon &= 0xFF;
			bunitA->weaponBroke = true;
		}
	}
}

void ReapingBlade_Refresh() {
	if (
		//if active unit is the attacker (sanity check, probably unnecessary)
		GetUnit(gBattleActor.unit.index) == gActiveUnit
		//and attacker is not dead
		&& gActiveUnit->curHP != 0
		//and defender is dead
		&& GetUnit(gBattleTarget.unit.index)->curHP == 0
		//and we attacked this turn
		&& gActionData.unitActionType == UNIT_ACTION_COMBAT
		//and we activated Reaping Blade
		&& *AttackTypeByte_Link == ReapingBladeAttackType_Link
		//and we haven't galeforced this turn already
		//&& gBattleActor->unit.state & 0x0400
	) {
		//unset status 0x42, set 0x400
		gActiveUnit->state &= 0xFFFFFFBD;
		gActiveUnit->state |= 0x0400;
		
		//call galeforce event (just does a sound effect)
		CallMapEventEngine(&GaleforceEvent, EV_RUN_CUTSCENE);
	}
}