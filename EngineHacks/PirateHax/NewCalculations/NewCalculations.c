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
        int bossFactor = 1;
        if (target->unit.pCharacterData->attributes & CA_BOSS){
				bossFactor = 2;
		}
		// killed
		if (target->unit.curHP == 0){		
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

    if (unit->pClassData->attributes & CA_PROMOTED){
        effectiveLevel += 5;
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

    int exp = 0;
    if (staffRank == D_WEXP){
        exp += 15;
    }
    else if (staffRank == C_WEXP){
        exp += 20;
    }
    else if (staffRank == B_WEXP){
        exp += 25;
    }
    else if (staffRank == A_WEXP){
        exp += 30;
    }
    else{
        exp += 40;
    }

    int levelDiff = GetLevelDifference(actor, &gBattleTarget);

    exp += levelDiff * 2;

    if (exp >= 50){
        return 50;
    }
    else if (exp <= 2){
        return 2;
    }
    else{
        return exp;
    }
}

s8 ActionSteal(Proc* proc) {
    int item;

    item = GetUnit(gActionData.targetIndex)->items[gActionData.itemSlotIndex];

    UnitRemoveItem(GetUnit(gActionData.targetIndex), gActionData.itemSlotIndex);
    
    UnitAddItem(GetUnit(gActionData.subjectIndex), item);

    BattleInitItemEffect(GetUnit(gActionData.subjectIndex), -1);
    gBattleTarget.terrainId = 0x1; //plains id
    InitBattleUnit(&gBattleTarget, GetUnit(gActionData.targetIndex));
    gBattleTarget.weapon = item;
    BattleApplyStealAction(proc, item);

    MU_EndAll();
    BeginMapAnimForSteal();

    return 0;
}

void BattleApplyStealAction(struct Proc* proc, int item) {
    BattleApplyStealActionExpGains(item);
    ProcStartBlocking(sProcScr_BattleAnimSimpleLock, proc);
}

void BattleApplyStealActionExpGains(int item) { 
    if (UNIT_FACTION(&gBattleActor.unit) != FACTION_BLUE){
		return;
	}  
    if (!CanBattleUnitGainLevels(&gBattleActor)){
		return;
	}    
    if (gChapterData.chapterStateBits & CHAPTER_FLAG_7){
		return;
	}

    gBattleActor.expGain = GetStealExpValue(item);
    gBattleActor.unit.exp += GetStealExpValue(item);

    CheckBattleUnitLevelUp(&gBattleActor);
}

int GetStealExpValue(int item){
    
    const ItemData* stolenItemData = GetItemData(GetItemIndex(item));
    u16 costPerUse = stolenItemData->costPerUse;
    u8 durability = item >> 8;
    int totalCost = costPerUse * durability; 
    
    int stealExp = totalCost / 50 + GetLevelDifference(&gBattleActor, &gBattleTarget);

    if (stealExp >= 33){
        return 33;
    }
    else if (stealExp <= 1){
        return 1;
    }
    else{
        return stealExp;
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
    const struct ClassData* currentClass = unit->pClassData;

    int i;

    // Apply stat ups

    unit->maxHP += (promotedClass->baseHP - currentClass->baseHP);

    if (unit->maxHP > 60){
        unit->maxHP = 60;
    }
        
    unit->curHP += (promotedClass->baseHP - currentClass->baseHP);

    if (unit->curHP > unit->maxHP){
        unit->curHP = unit->maxHP;
    }

    unit->pow += (promotedClass->basePow - currentClass->basePow);

    if (unit->pow > 30){
        unit->pow = 30;
    }

	unit->mag += (MagClassTable[promotedClass->number].promotionMag - MagClassTable[currentClass->number].promotionMag);

	if (unit->mag > 30){
        unit->mag = 30;
    }
		
    unit->skl += (promotedClass->baseSkl - currentClass->baseSkl);

    if (unit->skl > 30){
        unit->skl = 30;
    }
        

    unit->spd += (promotedClass->baseSpd - currentClass->baseSpd);

    if (unit->spd > 30){
        unit->spd = 30;
    }
        

    unit->def += (promotedClass->baseDef - currentClass->baseDef);

    if (unit->def > 30){
        unit->def = 30;
    }

    unit->res += (promotedClass->baseRes - currentClass->baseRes);

    if (unit->res > 30){
		unit->res = 30;
	}

	unit->lck += (promotedClass->baseLck - currentClass->baseLck);

	if (unit->lck > 30){
        unit->lck = 30;
    }

    // Remove base class' base wexp from unit wexp
    for (i = 0; i < 8; ++i)
        unit->ranks[i] -= unit->pClassData->baseRanks[i];

    // Update unit class
    unit->pClassData = promotedClass;

    // Add promoted class' base wexp to unit wexp
    for (i = 0; i < 8; ++i) {
        int wexp = unit->ranks[i];

        wexp += unit->pClassData->baseRanks[i];

        if (wexp > S_WEXP)
            wexp = S_WEXP;

        unit->ranks[i] = wexp;
    }
}

void ApplyUnitPromotion(struct Unit* unit, u8 classId) {
    const struct ClassData* promotedClass = GetClassData(classId);
    const struct ClassData* currentClass = unit->pClassData;

    int i;

    // Apply stat ups

    unit->maxHP += (promotedClass->baseHP - currentClass->baseHP);

    if (unit->maxHP > 60){
        unit->maxHP = 60;
    }
        
    unit->curHP += (promotedClass->baseHP - currentClass->baseHP);

    if (unit->curHP > unit->maxHP){
        unit->curHP = unit->maxHP;
    }

    unit->pow += (promotedClass->basePow - currentClass->basePow);

    if (unit->pow > 30){
        unit->pow = 30;
    }

	unit->mag += (MagClassTable[promotedClass->number].promotionMag - MagClassTable[currentClass->number].promotionMag);

	if (unit->mag > 30){
        unit->mag = 30;
    }
		
    unit->skl += (promotedClass->baseSkl - currentClass->baseSkl);

    if (unit->skl > 30){
        unit->skl = 30;
    }
        

    unit->spd += (promotedClass->baseSpd - currentClass->baseSpd);

    if (unit->spd > 30){
        unit->spd = 30;
    }
        

    unit->def += (promotedClass->baseDef - currentClass->baseDef);

    if (unit->def > 30){
        unit->def = 30;
    }

    unit->res += (promotedClass->baseRes - currentClass->baseRes);

    if (unit->res > 30){
		unit->res = 30;
	}

	unit->lck += (promotedClass->baseLck - currentClass->baseLck);

	if (unit->lck > 30){
        unit->lck = 30;
    }
		

    // Remove base class' base wexp from unit wexp
    for (i = 0; i < 8; ++i)
        unit->ranks[i] -= unit->pClassData->baseRanks[i];

    // Update unit class
    unit->pClassData = promotedClass;

    // Add promoted class' base wexp to unit wexp
    for (i = 0; i < 8; ++i) {
        int wexp = unit->ranks[i];

        wexp += unit->pClassData->baseRanks[i];

        if (wexp > S_WEXP)
            wexp = S_WEXP;

        unit->ranks[i] = wexp;
    }

}

int GetBattleUnitUpdatedWeaponExp(BattleUnit* battleUnit) {
    int i, result;

    if (UNIT_FACTION(&battleUnit->unit) != FACTION_BLUE){
		return -1;
	}
    if (battleUnit->unit.curHP == 0){
		return -1;
	}
    if (gChapterData.chapterStateBits & CHAPTER_FLAG_7){
        return -1;
	}
    if (gGameState.statebits & 0x40){ // TODO: GAME STATE BITS CONSTANTS
 		return -1;
	} 
    if (!(gBattleStats.config & BATTLE_CONFIG_ARENA)) {
        if (!battleUnit->canCounter){
 			return -1;
		}
        if (!(battleUnit->weaponAttributes & IA_REQUIRES_WEXP)){
			return -1;
		}
            
	}
    
	result = battleUnit->unit.ranks[battleUnit->weaponType] + 1;

    for (i = 0; i < 8; ++i) {
        if (i == battleUnit->weaponType){
            continue;
		}
        if (battleUnit->unit.pClassData->baseRanks[i] == S_WEXP){
            continue;
		}
        if (battleUnit->unit.ranks[i] < S_WEXP){
            continue;
		}
        if (result >= S_WEXP){
			result = S_WEXP;
		}
            
        break;
    }

   	if (result > S_WEXP){
        result = S_WEXP;
    } 

    return result;
}

int CanUnitUseWeapon(struct Unit* unit, int item) {
    if (item == 0){
		return FALSE;
	}

    if (!(GetItemAttributes(item) & IA_WEAPON))
        return FALSE;

    if (GetItemAttributes(item) & IA_LOCK_ANY) {
        // Check for item locks

        if ((GetItemAttributes(item) & IA_LOCK_1) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_1))
            return FALSE;

        if (GetItemAttributes(item) & IA_UNUSABLE)
            if (!(IsItemUnsealedForUnit(unit, item)))
                return FALSE;
    }

    if ((unit->statusIndex == UNIT_STATUS_SILENCED) && (GetItemAttributes(item) & IA_MAGIC)){
		return FALSE;
	}
        
    int wRank = GetItemRequiredExp(item);
    int uRank = (unit->ranks[GetItemType(item)]);

    return (uRank >= wRank) ? TRUE : FALSE;
}