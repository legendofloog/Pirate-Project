#include "gbafe.h"

#include "NewCalculations.h"


int GetCurrentPromotedLevelBonus(){
    return 0; //promo'd units don't get anything bc level doesn't reset
}

void ComputeBattleUnitHitRate(BattleUnit* bu) {
    bu->battleHitRate = (bu->unit.skl * 3) + GetItemHit(bu->weapon) + bu->wTriangleHitBonus;
}

void ComputeBattleUnitAvoidRate(BattleUnit* bu) {
    bu->battleAvoidRate = bu->terrainAvoid + (bu->unit.lck * 3);

    if (bu->battleAvoidRate < 0){
        bu->battleAvoidRate = 0;
    }
}

void ComputeBattleUnitCritRate(struct BattleUnit* bu) {
	if (GetItemCrit(bu->weapon) != 255){
		bu->battleCritRate = GetItemCrit(bu->weapon);
	}
    else{
		bu->battleCritRate = 0;
	}
}

void ComputeBattleUnitEffectiveCritRate(struct BattleUnit* attacker, struct BattleUnit* defender) {
    int i;
	Item item;

    attacker->battleEffectiveCritRate = attacker->battleCritRate;

    if (attacker->battleEffectiveCritRate < 0)
        attacker->battleEffectiveCritRate = 0;
	
    for (i = 0; (i < UNIT_ITEM_COUNT); ++i) {
		item = defender->unit.items[i];
        if (GetItemAttributes(item) & IA_NEGATE_CRIT) {
            attacker->battleEffectiveCritRate = 0;
            break;
        }
    }
}

void ComputeBattleUnitWeaponRankBonuses(struct BattleUnit* bu) {
    return;
}

int GetBattleUnitExpGain(BattleUnit* actor, BattleUnit* target){
	
	if (CanBattleUnitGainExp(actor, target)){

        // tinked or missed
		if (!(actor->nonZeroDamage)){
			return 0;
		}

        int levelDiff = GetLevelDifference(actor, target);

		// killed
		if (target->unit.curHP == 0){
			
            int bossFactor = 1;

			if (target->unit.pCharacterData->attributes & CA_BOSS){
				bossFactor = 2;
			}
			
            int initialKillExp = (30 + 3 * levelDiff) * bossFactor;

			if(initialKillExp <= 3){
				return 3;
			}
			else if(initialKillExp >= 100){
				return 100;
			}
			else{
				return initialKillExp;
			}
		}

		// hit
		int initialHitExp = 10 + 1 * levelDiff;

			if(initialHitExp <= 1){
				return 1;
			}
			else if(initialHitExp >= 33){
				return 33;
			}
			else{
				return initialHitExp;
			}

	}

	return 0;
}

bool CanBattleUnitGainExp(BattleUnit* actor, BattleUnit* target){

	// is the unit exp maxed
	if (!CanBattleUnitGainLevels(actor)){
		return false;
	}

	// is the unit alive
	if (actor->unit.curHP == 0){
		return false;
	}

	// does the opponent prevent exp gain
	if (((target->unit.pCharacterData->attributes) | (target->unit.pClassData->attributes)) & CA_NO_EXP){
		return false;
	}

	return true;

}

int GetUnitEffectiveLevel(Unit* unit){

	int effectiveLevel = unit->level;

    if (unit->pClassData->attributes & CA_PROMOTED)
    {
        effectiveLevel += 10; //add 10 if promoted
    }

	return effectiveLevel;

}

int GetLevelDifference(BattleUnit* actor, BattleUnit* target){

	int diff = GetUnitEffectiveLevel(&target->unit) - GetUnitEffectiveLevel(&actor->unit);
	return diff;

}

bool CanBattleUnitGainLevels(BattleUnit* battleUnit) {
    if (battleUnit->unit.exp == 0xFF){
    	return false;
    }

    if (UNIT_FACTION(&battleUnit->unit) != FACTION_BLUE){
    	return false;
    }

    return true;
}

int GetBattleUnitStaffExp(BattleUnit* actor){
	if (!CanBattleUnitGainLevels(actor)){
		return 0;
	}

	// is the unit alive
	if (actor->unit.curHP == 0){
		return 0;
	}

	const ItemData* staffData = GetItemData(GetItemIndex(actor->weapon));
	int staffRank = staffData->weaponRank;
	
    if( staffRank == D_WEXP ){ // d rank
		return 20;
	}
	else if( staffRank == C_WEXP){ // c rank
		return 25;
	} 
	else if( staffRank == B_WEXP){ // b rank
		return 30;
	}
	else if( staffRank == A_WEXP ){ // a rank
		return 35;
	}
	else{ // s rank
		return 40;
	}
}

// makes autolevels fixed
int GetAutoleveledStatIncrease(int growth, int levelCount) {
    return GetNPCStatIncrease(growth * (levelCount));
}

int GetNPCStatIncrease(int growth){
	int result = 0;
	
	while (growth >= 100) {
        result++;
        growth -= 100;
    }

	return result;
}

void UnitLoadStatsFromChracter(struct Unit* unit, const struct CharacterData* character) {
    int i;
    
    unit->maxHP = character->baseHP + unit->pClassData->baseHP;
    unit->pow   = character->basePow + unit->pClassData->basePow;
	unit->mag   = MagCharTable[character->number].baseMag + MagClassTable[unit->pClassData->number].baseMag;
    unit->skl   = character->baseSkl + unit->pClassData->baseSkl;
    unit->spd   = character->baseSpd + unit->pClassData->baseSpd;
    unit->def   = character->baseDef + unit->pClassData->baseDef;
    unit->res   = character->baseRes + unit->pClassData->baseRes;
    unit->lck   = character->baseLck + unit->pClassData->baseLck;

    unit->conBonus = 0;

    for (i = 0; i < 8; ++i) {
        unit->ranks[i] = unit->pClassData->baseRanks[i];

        if (unit->pCharacterData->baseRanks[i])
            unit->ranks[i] = unit->pCharacterData->baseRanks[i];
    }

    if (UNIT_FACTION(unit) == FACTION_BLUE && (unit->level != UNIT_LEVEL_MAX))
        unit->exp = 0;
    else
        unit->exp = UNIT_EXP_DISABLED;
}

int GetWeaponLevelFromExp(int wexp) {
    if (wexp < D_WEXP)
        return NO_RANK;

    if (wexp < C_WEXP)
        return D_RANK;

    if (wexp < B_WEXP)
        return C_RANK;

    if (wexp < A_WEXP)
        return B_RANK;

    if (wexp < S_WEXP)
        return A_RANK;

    return S_RANK;
}

void GetWeaponExpProgressState(int wrank, int* valOut, int* maxOut) {
    int currentRank = GetWeaponLevelFromExp(wrank);
	if (currentRank == NO_RANK){
		*valOut = 0;
        *maxOut = 0;
	}
	else if(currentRank == D_RANK){
		*valOut = wrank      - D_WEXP;
        *maxOut   = C_WEXP - D_WEXP;		
	}
	else if(currentRank == C_RANK){
		*valOut = wrank      - C_WEXP;
        *maxOut   = B_WEXP - C_WEXP;
	}
	else if(currentRank == B_RANK){
		*valOut = wrank      - B_WEXP;
        *maxOut   = A_WEXP - B_WEXP;
	}
	else if(currentRank == A_RANK){
		*valOut = wrank      - A_WEXP;
        *maxOut   = S_WEXP - A_WEXP;
	}
	else{
		*valOut = 0;
        *maxOut   = 0;
	}
	return;
}

int CanUnitRescue(const struct Unit* actor, const struct Unit* target){
    
    int actorAid  = GetUnitAid(actor);
    int targetCon = UNIT_CON(target);

    return (actorAid >= targetCon) ? TRUE : FALSE;
}

int GetUnitAid(const struct Unit* unit) {
    int aid;
	if (UNIT_CATTRIBUTES(unit) & CA_MOUNTED){
		aid = 25 - UNIT_CON(unit);
	}
	else if (UNIT_CATTRIBUTES(unit) & CA_WYVERN || UNIT_CATTRIBUTES(unit) & CA_PEGASUS){
		aid = 20 - UNIT_CON(unit);
	}
	else{
		aid = UNIT_CON(unit) - 1;
	}
    return aid;
}

void UnitAutolevelWExp(struct Unit* unit, const struct UnitDefinition* uDef) {
    if (uDef->autolevel) {
        int i;

        for (i = 0; i < GetUnitItemCount(unit); ++i) {
            int wType, item = unit->items[i];

            if (!(GetItemAttributes(item) & IA_REQUIRES_WEXP))
                continue;

            if (GetItemAttributes(item) & IA_WEAPON)
                if (CanUnitUseWeapon(unit, item))
                    continue;

            if (GetItemAttributes(item) & IA_STAFF)
                if (CanUnitUseStaff(unit, item))
                    continue;

            wType = GetItemType(item);

            if (unit->ranks[wType] == 0)
                item = 0;

            unit->ranks[wType] = GetItemRequiredExp(item);
        }
    }
}

void ApplyUnitDefaultPromotion(struct Unit* unit) {
    const struct ClassData* promotedClass = GetClassData(unit->pClassData->promotion);

    int baseClassId = unit->pClassData->number;
    int promClassId = promotedClass->number;

    int i;

    // Apply stat ups

    unit->maxHP += promotedClass->promotionHp;

    if (unit->maxHP > promotedClass->maxHP)
        unit->maxHP = promotedClass->maxHP;

    unit->pow += promotedClass->promotionPow;

    if (unit->pow > promotedClass->maxPow)
        unit->pow = promotedClass->maxPow;

    unit->skl += promotedClass->promotionSkl;

    if (unit->skl > promotedClass->maxSkl)
        unit->skl = promotedClass->maxSkl;

    unit->spd += promotedClass->promotionSpd;

    if (unit->spd > promotedClass->maxSpd)
        unit->spd = promotedClass->maxSpd;

    unit->def += promotedClass->promotionDef;

    if (unit->def > promotedClass->maxDef)
        unit->def = promotedClass->maxDef;

    unit->res += promotedClass->promotionRes;

    if (unit->res > promotedClass->maxRes)
        unit->res = promotedClass->maxRes;

    // Remove base class' base wexp from unit wexp
    for (i = 0; i < 8; ++i)
        unit->ranks[i] -= unit->pClassData->baseRanks[i];

    // Update unit class
    unit->pClassData = promotedClass;

    // Add promoted class' base wexp to unit wexp
    for (i = 0; i < 8; ++i) {
        int wexp = unit->ranks[i];

        wexp += unit->pClassData->baseRanks[i];

        if (wexp > WPN_EXP_S)
            wexp = WPN_EXP_S;

        unit->ranks[i] = wexp;
    }

    unit->curHP += promotedClass->promotionHp;

    if (unit->curHP > GetUnitMaxHp(unit))
        unit->curHP = GetUnitMaxHp(unit);
}

void ApplyUnitPromotion(struct Unit* unit, u8 classId) {
    const struct ClassData* promotedClass = GetClassData(classId);

    int baseClassId = unit->pClassData->number;
    int promClassId = promotedClass->number;

    int i;

    // Apply stat ups

    unit->maxHP += promotedClass->promotionHp;

    if (unit->maxHP > promotedClass->maxHP)
        unit->maxHP = promotedClass->maxHP;

    unit->pow += promotedClass->promotionPow;

    if (unit->pow > promotedClass->maxPow)
        unit->pow = promotedClass->maxPow;

    unit->skl += promotedClass->promotionSkl;

    if (unit->skl > promotedClass->maxSkl)
        unit->skl = promotedClass->maxSkl;

    unit->spd += promotedClass->promotionSpd;

    if (unit->spd > promotedClass->maxSpd)
        unit->spd = promotedClass->maxSpd;

    unit->def += promotedClass->promotionDef;

    if (unit->def > promotedClass->maxDef)
        unit->def = promotedClass->maxDef;

    unit->res += promotedClass->promotionRes;

    if (unit->res > promotedClass->maxRes)
        unit->res = promotedClass->maxRes;

    // Remove base class' base wexp from unit wexp
    for (i = 0; i < 8; ++i)
        unit->ranks[i] -= unit->pClassData->baseRanks[i];

    // Update unit class
    unit->pClassData = promotedClass;

    // Add promoted class' base wexp to unit wexp
    for (i = 0; i < 8; ++i) {
        int wexp = unit->ranks[i];

        wexp += unit->pClassData->baseRanks[i];

        if (wexp > WPN_EXP_S)
            wexp = WPN_EXP_S;

        unit->ranks[i] = wexp;
    }

    unit->curHP += promotedClass->promotionHp;

    if (unit->curHP > GetUnitMaxHp(unit))
        unit->curHP = GetUnitMaxHp(unit);
}
