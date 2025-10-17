#include "ProtectStaff.h"

bool IsProtectBitSet(Unit* unit) {
	return CheckBit(GetUnitDebuffEntry(unit), ProtectStaffBitOffset_Link);
}

void SetProtectBit(Unit* unit) {
	SetBit(GetUnitDebuffEntry(unit), ProtectStaffBitOffset_Link);
}

void UnsetProtectBit(Unit* unit) {
	UnsetBit(GetUnitDebuffEntry(unit), ProtectStaffBitOffset_Link);
}

void ClearProtectBitEachTurn() {
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
		
		//clear Protect bit
		UnsetProtectBit(curUnit);
		
		unitID++;
	}	
}

void ExecProtectStaff(Proc* proc) {
	//set attacker & the item to apply the effect of
	BattleInitItemEffect(GetUnit(gActionData.subjectIndex),gActionData.itemSlotIndex);
	//set defender
	BattleInitItemEffectTarget(GetUnit(gActionData.targetIndex));
	
	//do the staff-specific effect action
	SetProtectBit(GetUnit(gActionData.targetIndex));
	
	//perform the item effect & do battle anims
	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
}

void AddUnitToTargetListIfUnprotected(Unit* unit) {
	if (AreAllegiancesAllied(gUnitSubject->index, unit->index)
		&& !IsProtectBitSet(unit)) {
		AddTarget(unit->xPos, unit->yPos, unit->index, 0);
	}
}

void MakeTargetListForProtect(Unit* unit) {
	int x = unit->xPos;
	int y = unit->yPos;
	gUnitSubject = unit;
	
	InitTargets(x, y);
	
	// ier thingamabob
	Item_TURange(unit, AddUnitToTargetListIfUnprotected, ProtectStaffID_Link);
}

void ProtectUsabilityWrapper() {
	asm("mov r0,r4;	\
		 mov r1,r5; \
		 bl ProtectUsability; \
		 pop {r4,r5}; \
		 pop {r1}; \
         bx r1; \
	");	
}

bool ProtectUsability(Unit* unit) {
	MakeTargetListForProtect(unit);
	return GetTargetListSize() != 0;
}

void ProtectStaffTargeting(struct Unit* unit) {

	MakeTargetListForProtect(unit);
	
	BmMapFill(gMapMovement, -1);
	
	StartBottomHelpText(
		StartTargetSelection(&ProtectStaffSelectInfo),
		GetStringFromIndex(ProtectStaffSubtitleText_Link));
}

void ProtectStaffTargetingWrapper() {
	asm("	mov r0, r5; \
			mov r2, r4; \
			bl ProtectStaffTargeting; \
			pop {r4-r5}; \
			pop {r0}; \
			bx r0; \
	");
}

void StartUnitDefChangeInfoWindow(struct Proc* parent) {
	struct UnitInfoWindowProc* proc = NewUnitInfoWindow(parent);
	Text_InitDB(proc->lines, 11); // second parameter is width
	Text_InitDB(proc->lines + 1, 11);
}

void DrawUnitDefChangeText(struct TextHandle* text, struct Unit* unit, int bonus) {
    Text_Clear(text);

    Text_InsertString(text, 0, 3, GetStringFromIndex(0x4EF)); // Def[.][X]
	Text_InsertString(text, 16, 3, GetStringFromIndex(0x20D)); // :
    Text_InsertString(text, 44, 3, GetStringFromIndex(HealArrowIDLink)); // ->

    Text_InsertNumberOr2Dashes(text, 64, 2, (GetUnitDefense(unit)) + bonus);
    Text_InsertNumberOr2Dashes(text, 32, 2, (GetUnitDefense(unit)));
}

void RefreshUnitDefChangeInfoWindow(struct Unit* unit) {
	int y = 0;
    int x = GetUnitInfoWindowX(unit, 11);

    struct UnitInfoWindowProc* proc = UnitInfoWindow_DrawBase(0, unit, x, y, 11, 2); // last parameter is lines
	
    DrawUnitDefChangeText(proc->lines, unit, ProtectStaffBuffAmount_Link);
	DrawUnitResChangeText(proc->lines + 1, unit, ProtectStaffBuffAmount_Link);

    Text_Display(proc->lines, gBg0MapBuffer + TILEMAP_INDEX(x + 1, y + 3));
	Text_Display(proc->lines + 1, gBg0MapBuffer + TILEMAP_INDEX(x + 1, y + 5));
}

int ProtectStaffInitSelect(struct Proc* proc) {
    StartUnitDefChangeInfoWindow(proc);
}

u8 ProtectStaffTargetChange(struct Proc* proc, struct TargetEntry* target) {
    ChangeActiveUnitFacing(target->x, target->y);
    RefreshUnitDefChangeInfoWindow(GetUnit(target->unitIndex));
}
