#include "gbafe.h"

#include "NewCalculations.h"


int GetCurrentPromotedLevelBonus(){
    return 0; //promo'd units don't get anything bc level doesn't reset
}

void ComputeBattleUnitHitRate(BattleUnit* bu) {
    bu->battleHitRate = (bu->unit.skl * 4) + GetItemHit(bu->weapon) + bu->wTriangleHitBonus;
}

void ComputeBattleUnitAvoidRate(BattleUnit* bu) {
    bu->battleAvoidRate = bu->terrainAvoid + (bu->unit.lck * 3) + bu->battleSpeed;

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
            int initialKillExp = 30 + 6 * levelDiff;

			if(initialKillExp < 6){
				return 6;
			}
            else if (initialKillExp > 100){
                return 100;
            }
			else{
				return initialKillExp; //50 kill exp cap
			}
		}

		// hit
		int initialHitExp = 10 + 2 * levelDiff;

			if(initialHitExp < 2){
				return 2;
			}
			else if(initialHitExp > 33){
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
        exp += 18;
    }
    else if (staffRank == C_WEXP){
        exp += 21;
    }
    else if (staffRank == B_WEXP){
        exp += 24;
    }
    else if (staffRank == A_WEXP){
        exp += 27;
    }
    else if(staffRank == S_WEXP){
        exp += 30;
    }
    else{

    }

    return exp;
}

s8 ActionSteal(Proc* proc) {
    int item;

    item = GetUnit(gActionData.targetIndex)->items[gActionData.itemSlotIndex];

    UnitRemoveItem(GetUnit(gActionData.targetIndex), gActionData.itemSlotIndex);
    
    UnitAddStolenItem(GetUnit(gActionData.subjectIndex), item);

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
    
    int stealExp = totalCost / 50 + GetLevelDifference(&gBattleActor, &gBattleTarget) * 2;

    if (stealExp >= 15){
        return 15;
    }
    else if (stealExp <= 2){
        return 2;
    }
    else{
        return stealExp;
    }
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
    
    if (target->pClassData->attributes & CA_MOUNTEDAID){ //is this unit a mount
        return false; //cannot be rescued
    }

    if (LuaIsInHub(gActiveUnit)){ //unit doesn't matter
        return false;
    }

    int actorAid  = GetUnitAid(actor);
    int targetCon = UNIT_CON(target);

    return (actorAid >= targetCon) ? TRUE : FALSE;
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

	unit->mag += (MagClassTable[promotedClass->number].baseMag - MagClassTable[currentClass->number].baseMag);

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

	unit->mag += (MagClassTable[promotedClass->number].baseMag - MagClassTable[currentClass->number].baseMag);

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
    
	result = battleUnit->unit.ranks[battleUnit->weaponType];

    if (battleUnit->wexpMultiplier <= 0)
    {
        // we don't do anything because you missed or didn't hit anything
    }
    else if (gBattleTarget.unit.curHP <= 0)
    {
        result += 2; //you killed a guy, here's your reward
    }
    else
    {
        result++; //otherwise, here's some wexp
    }

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

        if ((GetItemAttributes(item) & IA_LOCK_4) && !(UNIT_CATTRIBUTES(unit) & CA_LOCK_4))
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

// makes autolevels fixed
int GetAutoleveledStatIncrease(int growth, int levelCount)
{
    return GetNPCStatIncrease(growth * (levelCount + 1));
}

int GetNPCStatIncrease(int growth){
	int result = 0;
	
    if (growth < 0)
    {
        while (growth <= 0) //if there's any negative left over, need to subtract still
        {
            result--;
            growth += 100;
        }
    }
    else
    {
        while (growth >= 100) //if there's not enough for a full level, don't give more
        {
            result++;
            growth -= 100;
        }
    }
	return result;
}

struct Unit* LoadUnit(const struct UnitDefinition* uDef) {

    struct Unit* unit = NULL;

    switch (uDef->allegiance) {
        // TODO: unit definition faction constants
        case 0:
            unit = GetFreeBlueUnit(uDef);
            break;

        case 2:
            unit = GetFreeUnit(FACTION_RED);
            break;

        case 1:
            unit = GetFreeUnit(FACTION_GREEN);
            break;
    }

    if (!unit)
        return NULL;

    ClearUnit(unit);

    UnitInitFromDefinition(unit, uDef);
    UnitLoadStatsFromChracter(unit, unit->pCharacterData);
    HideIfUnderRoof(unit);

    if (uDef->autolevel) {
        if (UNIT_FACTION(unit) != FACTION_BLUE) {
            SetUnitLeaderCharId(unit, uDef->leaderCharIndex);
        }
        UnitAutolevel(unit);
        UnitAutolevelWExp(unit, uDef);

    }
    else{
        int autolevelCount = 0; //autoleveling, but only for enemies, and only based off of the difficulty; catches edge cases like bosses outside of hubs
        if (IsDifficultMode()){
            autolevelCount = 5;
        }
        else if (TUTORIAL_MODE())
        {
            autolevelCount = -5;
        }

        if (autolevelCount != 0)
        {
            bool isUnitPlayer = (unit->pCharacterData->number <= 0x45);
            bool IsUnitBoss = (unit->pCharacterData->attributes & CA_BOSS);
            
            if (isUnitPlayer){ //players get nothing, so skip them
                
            }
            else if (IsUnitBoss) //boss or enemies could get negative stats, so need to floor them just in case
            {
                AutolevelUnit(unit, autolevelCount);
                FloorStats(unit);
            }
            else{
                AutolevelClass(unit, autolevelCount);
                FloorStats(unit);           
            }
        }

    }

    FixROMUnitStructPtr(unit);
    UnitLoadSupports(unit);

    UnitCheckStatCaps(unit);

    unit->curHP = GetUnitMaxHp(unit);

    return unit;
}

void UnitInitFromDefinition(struct Unit* unit, const struct UnitDefinition* uDef) {
    unit->pCharacterData = GetCharacterData(uDef->charIndex);

    if (uDef->classIndex){
        unit->pClassData = GetClassData(uDef->classIndex);
    }  
    else{
        unit->pClassData = GetClassData(unit->pCharacterData->defaultClass);
    }

    unit->level = uDef->level;

    if (uDef->autolevel){
        unit->level += ReturnNumberOfHubChaptersVisited();
    }

    GenUnitDefinitionFinalPosition(uDef, &unit->xPos, &unit->yPos, FALSE);
        
    int i;

    for (i = 0; (i < UNIT_DEFINITION_ITEM_COUNT) && (uDef->items[i]); ++i){
        UnitAddItem(unit, MakeNewItem(uDef->items[i]));
    }

    SetUnitAiFromDefinition(unit, uDef);
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


void UnitAutolevel(struct Unit* unit) {
    UnitAutolevelCore(unit, unit->pClassData->number, unit->level - unit->pCharacterData->baseLevel);
}

void UnitAutolevelCore(struct Unit* unit, int classId, int levelCount) {
    bool isUnitPlayer = (unit->pCharacterData->number <= 0x45);
    bool IsUnitBoss = (unit->pCharacterData->attributes & CA_BOSS);

    int autolevelCount = levelCount;
    int difficultyLevelChange = 0; //for normal mode       
    if (IsDifficultMode())
    {
        difficultyLevelChange = 5;
    }
    else if (TUTORIAL_MODE())
    {
        difficultyLevelChange = -5;
    }

    autolevelCount += difficultyLevelChange; //add the levels from difficulty: example: level 6 fighter on easy gets +5, -5, same as a normal fighter at level 1

    if (levelCount) {
        if (isUnitPlayer){ //players do not get the easy/hard mode difficulty changes, so just use initial level count
            AutolevelUnit(unit, levelCount);
            
        }
        else if (IsUnitBoss) //boss or enemies could get negative stats, so need to floor them just in case
        {
            AutolevelUnit(unit, autolevelCount);
            FloorStats(unit);
        }
        else{
            AutolevelClass(unit, autolevelCount);
            FloorStats(unit);           
        }
        
    }

}

void AutolevelUnit(struct Unit* unit, int levelCount){
    unit->maxHP += GetAutoleveledStatIncrease(unit->pCharacterData->growthHP,  levelCount);
    unit->pow   += GetAutoleveledStatIncrease(unit->pCharacterData->growthPow, levelCount);
    unit->mag   += GetAutoleveledStatIncrease(MagCharTable[unit->pCharacterData->number].growthMag, levelCount);
    unit->skl   += GetAutoleveledStatIncrease(unit->pCharacterData->growthSkl, levelCount);
    unit->spd   += GetAutoleveledStatIncrease(unit->pCharacterData->growthSpd, levelCount);
    unit->def   += GetAutoleveledStatIncrease(unit->pCharacterData->growthDef, levelCount);
    unit->res   += GetAutoleveledStatIncrease(unit->pCharacterData->growthRes, levelCount);
    unit->lck   += GetAutoleveledStatIncrease(unit->pCharacterData->growthLck, levelCount);
}

void AutolevelClass(struct Unit* unit, int levelCount)
{
    unit->maxHP += GetAutoleveledStatIncrease(unit->pClassData->growthHP,  levelCount);
    unit->pow   += GetAutoleveledStatIncrease(unit->pClassData->growthPow, levelCount);
    unit->mag   += GetAutoleveledStatIncrease(MagClassTable[unit->pClassData->number].growthMag, levelCount);
    unit->skl   += GetAutoleveledStatIncrease(unit->pClassData->growthSkl, levelCount);
    unit->spd   += GetAutoleveledStatIncrease(unit->pClassData->growthSpd, levelCount);
    unit->def   += GetAutoleveledStatIncrease(unit->pClassData->growthDef, levelCount);
    unit->res   += GetAutoleveledStatIncrease(unit->pClassData->growthRes, levelCount);
    unit->lck   += GetAutoleveledStatIncrease(unit->pClassData->growthLck, levelCount);
}

void FloorStats(struct Unit* unit)
{
    if (unit->maxHP < 0 || unit->maxHP >= 200) //to prevent underflow, check the high point
    {
        unit->maxHP = 0;
    }
    if (unit->pow < 0 || unit->pow >= 100)
    {
        unit->pow = 0;
    }
    if (unit->mag < 0 || unit->mag >= 100)
    {
        unit->mag = 0;
    }
    if (unit->skl < 0 || unit->skl >= 100)
    {
        unit->skl = 0;
    }
    if (unit->spd < 0 || unit->spd >= 100)
    {
        unit->spd = 0;
    }
    if (unit->def < 0 || unit->def >= 100)
    {
        unit->def = 0;
    }
    if (unit->res < 0 || unit->res >= 100)
    {
        unit->res = 0;
    }
    if (unit->lck < 0 || unit->lck >= 100)
    {
        unit->lck = 0;
    }
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

        //if (LuaIsInHub(unit)){ //actual unit passed in doesn't matter
          //  return; //don't autolevel their wexp if player
        //}

        int j;
        
        bool isUnitPlayer = (unit->pCharacterData->number <= 0x45);
        if (isUnitPlayer){ //if player unit, autolevel their wexp too
            for (j = 0; j < 8; j++){
                if (unit->ranks[j] >= 1 && unit->ranks[j] < 251){ //if it is an existent rank that is not S rank
                    if (unit->ranks[j] >= 246){
                        unit->ranks[j] = 251; //if it would go above 251, just set it to be 251 (S rank)
                    }
                    else{
                        unit->ranks[j] += (5 * (unit->level - uDef->level)); //increase wexp by 5 in all ranks the unit has by however many levels they've gotten
                    }
                }
            }
        }
        
    }
}

void* GetChapterAllyUnitDefinitions(void) {
    const struct ChapterEventGroup* evGroup = GetChapterEventDataPointer(gChapterData.chapterIndex);

    if (gChapterData.chapterIndex <= 7 && gChapterData.chapterIndex >= 2 && ReturnNumberOfHubChaptersVisited() >= 1) {
        return evGroup->playerUnitsInHard; //if we're in Hub A and more than one hub chapter has been visited, bigger unit group (+2 deploy)
    }
    
    return evGroup->playerUnitsInNormal;
}

int GetUnitLuckCap(Unit* unit){
    if (unit->pClassData->attributes & CA_PROMOTED){
        return 40; //unpromo'd units get 25 cap
    }
    else{
        return 25;
    }
}

void CheckBattleUnitStatCaps(struct Unit* unit, struct BattleUnit* bu) {
    if ((unit->maxHP + bu->changeHP) > UNIT_MHP_MAX(unit)){
        bu->changeHP = UNIT_MHP_MAX(unit) - unit->maxHP;
    }
    if ((unit->pow + bu->changePow) > UNIT_POW_MAX(unit)){
        bu->changePow = UNIT_POW_MAX(unit) - unit->pow;
    }
    if ((unit->mag + bu->changeMag) > MagClassTable[unit->pClassData->number].maxMag){
        bu->changeMag = MagClassTable[unit->pClassData->number].maxMag - unit->mag;
    }
    if ((unit->skl + bu->changeSkl) > UNIT_SKL_MAX(unit)){
        bu->changeSkl = UNIT_SKL_MAX(unit) - unit->skl;
    }      
    if ((unit->spd + bu->changeSpd) > UNIT_SPD_MAX(unit)){
        bu->changeSpd = UNIT_SPD_MAX(unit) - unit->spd;
    }
    if ((unit->def + bu->changeDef) > UNIT_DEF_MAX(unit)){
        bu->changeDef = UNIT_DEF_MAX(unit) - unit->def;
    }
    if ((unit->res + bu->changeRes) > UNIT_RES_MAX(unit)){
        bu->changeRes = UNIT_RES_MAX(unit) - unit->res;
    }
    if ((unit->lck + bu->changeLck) > GetUnitLuckCap(unit)){
        bu->changeLck = GetUnitLuckCap(unit) - unit->lck;
    } 
}

void UnitCheckStatCaps(struct Unit* unit) {
    if (unit->maxHP > UNIT_MHP_MAX(unit)){
        unit->maxHP = UNIT_MHP_MAX(unit);
    }      
    if (unit->pow > UNIT_POW_MAX(unit)){
        unit->pow = UNIT_POW_MAX(unit);
    }
    if (unit->mag > MagClassTable[unit->pClassData->number].maxMag){
        unit->mag = MagClassTable[unit->pClassData->number].maxMag;
    } 
    if (unit->skl > UNIT_SKL_MAX(unit)){
        unit->skl = UNIT_SKL_MAX(unit);
    }       
    if (unit->spd > UNIT_SPD_MAX(unit)){
        unit->spd = UNIT_SPD_MAX(unit);
    }   
    if (unit->def > UNIT_DEF_MAX(unit)){
        unit->def = UNIT_DEF_MAX(unit);
    }       
    if (unit->res > UNIT_RES_MAX(unit)){
        unit->res = UNIT_RES_MAX(unit);
    }
    if (unit->lck > GetUnitLuckCap(unit)){
        unit->lck = GetUnitLuckCap(unit);
    }
}
