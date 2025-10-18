#include "ConcentrateStaff.h"

bool IsConcentrateBitSet(Unit* unit) {
	return CheckBit(GetUnitDebuffEntry(unit), ConcentrateStaffBitOffset_Link);
}

void SetConcentrateBit(Unit* unit) {
	SetBit(GetUnitDebuffEntry(unit), ConcentrateStaffBitOffset_Link);
}

void UnsetConcentrateBit(Unit* unit) {
	UnsetBit(GetUnitDebuffEntry(unit), ConcentrateStaffBitOffset_Link);
}

void ConcentrateCritBoost(BattleUnit* bunitA, BattleUnit* bunitB) {
	if (IsConcentrateBitSet(&bunitA->unit)) bunitA->battleCritRate += ConcentrateStaffBuffAmount_Link;
}

void ClearConcentrateBitEachTurn() {
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
		
		//clear Concentrate bit
		UnsetConcentrateBit(curUnit);
		
		unitID++;
	}	
}

void ExecConcentrateStaff(Proc* proc) {
	//set attacker & the item to apply the effect of
	BattleInitItemEffect(GetUnit(gActionData.subjectIndex),gActionData.itemSlotIndex);
	//set defender
	BattleInitItemEffectTarget(GetUnit(gActionData.targetIndex));
	
	//do the staff-specific effect action
	SetConcentrateBit(GetUnit(gActionData.targetIndex));
	
	//perform the item effect & do battle anims
	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
}

void AddUnitToTargetListIfUnconcentrated(Unit* unit) {
	if (AreAllegiancesAllied(gUnitSubject->index, unit->index)
		&& !IsConcentrateBitSet(unit)) {
		AddTarget(unit->xPos, unit->yPos, unit->index, 0);
	}
}

void MakeTargetListForConcentrate(Unit* unit) {
	int x = unit->xPos;
	int y = unit->yPos;
	gUnitSubject = unit;
	
	InitTargets(x, y);
	
	// ier thingamabob
	Item_TURange(unit, AddUnitToTargetListIfUnconcentrated, ConcentrateStaffID_Link);
}

void ConcentrateUsabilityWrapper() {
	asm("mov r0,r4;	\
		 mov r1,r5; \
		 bl ConcentrateUsability; \
		 pop {r4,r5}; \
		 pop {r1}; \
         bx r1; \
	");	
}

bool ConcentrateUsability(Unit* unit) {
	MakeTargetListForConcentrate(unit);
	return GetTargetListSize() != 0;
}

void ConcentrateStaffTargeting(struct Unit* unit) {

	MakeTargetListForConcentrate(unit);
	
	BmMapFill(gMapMovement, -1);
	
	StartBottomHelpText(
		StartTargetSelection(&ConcentrateStaffSelectInfo),
		GetStringFromIndex(ConcentrateStaffSubtitleText_Link));
}

void ConcentrateStaffTargetingWrapper() {
	asm("	mov r0, r5; \
			mov r2, r4; \
			bl ConcentrateStaffTargeting; \
			pop {r4-r5}; \
			pop {r0}; \
			bx r0; \
	");
}

void StartUnitCritChangeInfoWindow(struct Proc* parent) {
	struct UnitInfoWindowProc* proc = NewUnitInfoWindow(parent);
	Text_InitDB(proc->lines, 13); // second parameter is width
}

void DrawUnitCritChangeText(struct TextHandle* text, struct Unit* unit, int bonus) {
    Text_Clear(text);

    Text_InsertString(text, 0, 3, GetStringFromIndex(0x501)); // Crit[.][X]
	Text_InsertString(text, 16, 3, GetStringFromIndex(0x20D)); // :
    Text_InsertString(text, 52, 3, GetStringFromIndex(HealArrowIDLink)); // ->

    Text_InsertNumberOr2Dashes(text, 80, 2, GetItemCrit(GetUnitEquippedWeapon(unit)) + bonus);
    Text_InsertNumberOr2Dashes(text, 40, 2, GetItemCrit(GetUnitEquippedWeapon(unit)));

    return;
}

void RefreshUnitCritChangeInfoWindow(struct Unit* unit) {
	int y = 0;
    int x = GetUnitInfoWindowX(unit, 13);

    struct UnitInfoWindowProc* proc = UnitInfoWindow_DrawBase(0, unit, x, y, 13, 1); // last parameter is lines
	
    DrawUnitCritChangeText(proc->lines + 0, unit, ConcentrateStaffBuffAmount_Link);
    Text_Display(proc->lines, gBg0MapBuffer + TILEMAP_INDEX(x + 1, y + 3));
}

int ConcentrateStaffInitSelect(struct Proc* proc) {
    StartUnitCritChangeInfoWindow(proc);
}

u8 ConcentrateStaffTargetChange(struct Proc* proc, struct TargetEntry* target) {
    ChangeActiveUnitFacing(target->x, target->y);
    RefreshUnitCritChangeInfoWindow(GetUnit(target->unitIndex));
}