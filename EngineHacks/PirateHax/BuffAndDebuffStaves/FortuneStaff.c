#include "FortuneStaff.h"

bool IsFortuneBitSet(Unit* unit) {
	return CheckBit(GetUnitDebuffEntry(unit), FortuneStaffBitOffset_Link);
}

void SetFortuneBit(Unit* unit) {
	SetBit(GetUnitDebuffEntry(unit), FortuneStaffBitOffset_Link);
}

void UnsetFortuneBit(Unit* unit) {
	UnsetBit(GetUnitDebuffEntry(unit), FortuneStaffBitOffset_Link);
}

void FortuneHitBoost(BattleUnit* bunitA, BattleUnit* bunitB) {
	if (IsFortuneBitSet(&bunitA->unit)) bunitA->battleHitRate += FortuneStaffBuffAmount_Link;
}

void ClearFortuneBitEachTurn() {
	//this runs in start of turn calc loop
	//for each unit of the current phase, look for one with the bit set
	
	int faction = gChapterData.currentPhase;
	int unitID = faction+1;
	int maxCount = 0;
	
	switch (faction) {
		case UA_BLUE:
		maxCount = 62;
		break;
		
		case UA_RED:
		maxCount = 50;
		break;
		
		case UA_GREEN:
		maxCount = 20;
		break;
	}
	
	while ((unitID - faction) < maxCount) {
		//get the unit unitID
		Unit* curUnit = GetUnit(unitID);
		
		//clear Fortune bit
		UnsetFortuneBit(curUnit);
		
		unitID++;
	}	
}

void ExecFortuneStaff(Proc* proc) {
	//set attacker & the item to apply the effect of
	BattleInitItemEffect(GetUnit(gActionData.subjectIndex),gActionData.itemSlotIndex);
	//set defender
	BattleInitItemEffectTarget(GetUnit(gActionData.targetIndex));
	
	//do the staff-specific effect action
	SetFortuneBit(GetUnit(gActionData.targetIndex));
	
	//perform the item effect & do battle anims
	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
}

void AddUnitToTargetListIfUnfortunate(Unit* unit) {
	if (AreAllegiancesAllied(gUnitSubject->index, unit->index)
		&& !IsFortuneBitSet(unit)) {
		AddTarget(unit->xPos, unit->yPos, unit->index, 0);
	}
}

void MakeTargetListForFortune(Unit* unit) {
	int x = unit->xPos;
	int y = unit->yPos;
	gUnitSubject = unit;
	
	InitTargets(x, y);
	
	// ier thingamabob
	Item_TURange(unit, AddUnitToTargetListIfUnfortunate, FortuneStaffID_Link);
}

bool FortuneUsability(Unit* unit) {
	MakeTargetListForFortune(unit);
	return GetTargetListSize() != 0;
}

void FortuneStaffTargeting(struct Unit* unit) {

	MakeTargetListForFortune(unit);
	
	BmMapFill(gMapMovement, -1);
	
	StartBottomHelpText(
		StartTargetSelection(&FortuneStaffSelectInfo),
		GetStringFromIndex(FortuneStaffSubtitleText_Link));
}

void StartUnitHitChangeInfoWindow(struct Proc* parent) {
	struct UnitInfoWindowProc* proc = NewUnitInfoWindow(parent);
	Text_InitDB(proc->lines, 12); // second parameter is width
}

void DrawUnitHitChangeText(struct TextHandle* text, struct Unit* unit, int bonus) {
    Text_Clear(text);

    Text_InsertString(text, 0, 3, GetStringFromIndex(0x4F4)); // Hit[.][X]
	Text_InsertString(text, 12, 3, GetStringFromIndex(0x20D)); // :
    Text_InsertString(text, 44, 3, GetStringFromIndex(HealArrowIDLink)); // ->

    Text_InsertNumberOr2Dashes(text, 72, 2, (GetUnitSkill(unit) * 4) + GetItemHit(GetUnitEquippedWeapon(unit)) + bonus);
    Text_InsertNumberOr2Dashes(text, 32, 2, (GetUnitSkill(unit) * 4) + GetItemHit(GetUnitEquippedWeapon(unit)));

    return;
}

void RefreshUnitHitChangeInfoWindow(struct Unit* unit) {
	int y = 0;
    int x = GetUnitInfoWindowX(unit, 12);

    struct UnitInfoWindowProc* proc = UnitInfoWindow_DrawBase(0, unit, x, y, 12, 1); // last parameter is lines
	
    DrawUnitHitChangeText(proc->lines + 0, unit, FortuneStaffBuffAmount_Link);
    Text_Display(proc->lines, gBg0MapBuffer + TILEMAP_INDEX(x + 1, y + 3));
}

int FortuneStaffInitSelect(struct Proc* proc) {
    StartUnitHitChangeInfoWindow(proc);
	return 1;
}

u8 FortuneStaffTargetChange(struct Proc* proc, struct TargetEntry* target) {
    ChangeActiveUnitFacing(target->x, target->y);
    RefreshUnitHitChangeInfoWindow(GetUnit(target->unitIndex));
	return 1;
}
