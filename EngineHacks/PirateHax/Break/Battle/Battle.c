#include "Battle.h"

s8 BattleGenerateRoundHits(struct BattleUnit* attacker, struct BattleUnit* defender) {
    int i, count;
    u16 attrs; // NOTE: this is a bug! attrs are 19 bits in FE8 (they're 16 bits in previous games)

    if (!(attacker->weapon)){
		return FALSE;
	}
        

	if (gDebuffTable[attacker->unit.index].skillState & SKILLSTATE_BROKEN_IN_BATTLE){
		return FALSE; //checks if attacker is not broken and not the battle actor
	}

    attrs = gBattleHitIterator->attributes;
    count = GetBattleUnitHitCount(attacker);

    for (i = 0; i < count; ++i) {
        gBattleHitIterator->attributes |= attrs;

        if (BattleGenerateHit(attacker, defender))
            return TRUE;
    }

    return FALSE;
}

void BattleGenerateHitEffects(struct BattleUnit* attacker, struct BattleUnit* defender) {
    attacker->wexpMultiplier++;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS)) {
        if (DidUnitBreak() && (attacker->unit.pCharacterData->number == gBattleActor.unit.pCharacterData->number)){
            if (gBattleStats.config & BATTLE_CONFIG_REAL){
                if (gDebuffTable[gBattleTarget.unit.index].skillState & SKILLSTATE_BROKEN_IN_BATTLE){ //if this already set, do not set break

                }
                else{
                    gDebuffTable[gBattleTarget.unit.index].skillState |= SKILLSTATE_BROKEN_IN_BATTLE;
			        gBattleHitIterator->attributes |= BATTLE_HIT_BREAK;
                }
                
            }
		}
		
		switch (GetItemWeaponEffect(attacker->weapon)) {

        case WPN_EFFECT_POISON:
            // Poison defender

            defender->statusOut = UNIT_STATUS_POISON;
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_POISON;

            // "Ungray" defender if it was petrified (as it won't be anymore)
            if (defender->unit.statusIndex == UNIT_STATUS_PETRIFY || defender->unit.statusIndex == UNIT_STATUS_13)
                defender->unit.state = defender->unit.state &~ US_UNSELECTABLE;

            break;

        case WPN_EFFECT_HPHALVE:
            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPHALVE;
            break;

        } // switch (GetItemWeaponEffect(attacker->weapon))

    	if ((GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_DEVIL) && (BattleRoll1RN(31 - attacker->unit.lck, FALSE))) {
        	gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_DEVIL;

        	attacker->unit.curHP -= gBattleStats.damage;

        	if (attacker->unit.curHP < 0)
            	attacker->unit.curHP = 0;
    		}
		else {
        	if (gBattleStats.damage > defender->unit.curHP)
            	gBattleStats.damage = defender->unit.curHP;

        	defender->unit.curHP -= gBattleStats.damage;

        	if (defender->unit.curHP < 0)
            	defender->unit.curHP = 0;
    	}

        if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_HPDRAIN) {
            if (attacker->unit.maxHP < (attacker->unit.curHP + gBattleStats.damage))
                attacker->unit.curHP = attacker->unit.maxHP;
            else
                attacker->unit.curHP += gBattleStats.damage;

            gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_HPSTEAL;
        }

            if (GetItemWeaponEffect(attacker->weapon) == WPN_EFFECT_PETRIFY) {
                switch (gChapterData.currentPhase) {

                case FACTION_BLUE:
                    if (UNIT_FACTION(&defender->unit) == FACTION_BLUE)
                        defender->statusOut = UNIT_STATUS_13;
                    else
                        defender->statusOut = UNIT_STATUS_PETRIFY;

                    break;

                case FACTION_RED:
                    if (UNIT_FACTION(&defender->unit) == FACTION_RED)
                        defender->statusOut = UNIT_STATUS_13;
                    else
                        defender->statusOut = UNIT_STATUS_PETRIFY;

                    break;

                case FACTION_GREEN:
                    if (UNIT_FACTION(&defender->unit) == FACTION_GREEN)
                        defender->statusOut = UNIT_STATUS_13;
                    else
                        defender->statusOut = UNIT_STATUS_PETRIFY;

                    break;

                } // switch (gRAMChapterData.faction)

                gBattleHitIterator->attributes |= BATTLE_HIT_ATTR_PETRIFY;
            }
    }

    gBattleHitIterator->hpChange = gBattleStats.damage;

    if (!(gBattleHitIterator->attributes & BATTLE_HIT_ATTR_MISS) || attacker->weaponAttributes & (IA_UNCOUNTERABLE | IA_MAGIC)) {
        attacker->weapon = GetItemAfterUse(attacker->weapon);

        if (!(attacker->weapon))
            attacker->weaponBroke = TRUE;
    }
}


void New_BattleInitTargetCanCounter(){

	// eggs
	if (gBattleTarget.unit.pClassData->number == 0x34 || gBattleTarget.unit.pClassData->number == 0x62){
        gBattleTarget.weapon = 0;
		gBattleTarget.canCounter = false;
		return;
	}

	// attacker/target weapon is uncounterable
	if (gBattleActor.weaponAttributes & IA_UNCOUNTERABLE || gBattleTarget.weaponAttributes & IA_UNCOUNTERABLE){
        gBattleTarget.weapon = 0;
		gBattleTarget.canCounter = false;
		return;
	}

	// attacker is berserked and both units are blue
	if ((gBattleActor.unit.statusIndex == UNIT_STATUS_BERSERK) && (gBattleActor.unit.index & FACTION_BLUE) && (gBattleTarget.unit.index & FACTION_BLUE)){
        gBattleTarget.weapon = 0;
		gBattleTarget.canCounter = false;
		return;
	}

    // attacker has dazzle
	if (gSkillTester(&gBattleActor.unit, DazzleIDLink)){
        gBattleTarget.weapon = 0;
		gBattleTarget.canCounter = false;
		return;
	}

	// defender is broken
	if (gDebuffTable[gBattleTarget.unit.index].skillState & SKILLSTATE_BREAK){
        gBattleTarget.weapon = 0;
		gBattleTarget.canCounter = false;
		return;
	}

}

int MapMain_SwitchPhases(void) {

    ClearActiveFactionGrayedStates();
    SMS_UpdateFromGameData();
    SwitchGameplayPhase();
    ClearActiveFactionBreakStatus(); //after the switch, clear acting faction break status

    if (RunPhaseSwitchEvents() == 1) {
        return 0;
    }

    return 1;
}