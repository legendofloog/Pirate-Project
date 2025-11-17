#include "BarrierStaff.h"

bool IsBarrierBitSet(Unit* unit) {
	if (UnpackData(GetUnitDebuffEntry(unit), BarrierStaffBitOffset_Link, 3)) return true;

	return false;
}

void SetBarrierBit(Unit* unit) {
	PackData(GetUnitDebuffEntry(unit), BarrierStaffBitOffset_Link, 3, 3);
}

void ReduceBarrierBit(Unit* unit) {
	if (IsBarrierBitSet(unit))
	{
		PackData(GetUnitDebuffEntry(unit), BarrierStaffBitOffset_Link, 3, UnpackData(GetUnitDebuffEntry(unit), BarrierStaffBitOffset_Link, 3) - 1);
	}		
}

void UnsetBarrierBits(Unit* unit)
{
	PackData(GetUnitDebuffEntry(unit), BarrierStaffBitOffset_Link, 3, 0);
}

void BarrierStaffTargeting(struct Unit* unit) {

	MakeTargetListForBarrier(unit);
	
	BmMapFill(gMapMovement, -1);
	
	StartBottomHelpText(
		StartTargetSelection(&BarrierStaffSelectInfo),
		GetStringFromIndex(BarrierStaffSubtitleText_Link));
}

void StartUnitResChangeInfoWindow(struct Proc* parent) {
	struct UnitInfoWindowProc* proc = NewUnitInfoWindow(parent);
	Text_InitDB(proc->lines, 11); // second parameter is width
}

int BarrierStaffInitSelect(struct Proc* proc) {
    StartUnitResChangeInfoWindow(proc);
	return 1;
}

void RefreshUnitResChangeInfoWindow(struct Unit* unit) {
	int y = 0;
    int x = GetUnitInfoWindowX(unit, 11);

    struct UnitInfoWindowProc* proc = UnitInfoWindow_DrawBase(0, unit, x, y, 11, 1); // last parameter is lines
	
    DrawUnitResChangeText(proc->lines + 0, unit, BarrierStaffBuffAmount_Link); //it's barrier  but +5 now
    Text_Display(proc->lines, gBg0MapBuffer + TILEMAP_INDEX(x + 1, y + 3));
}
 
u8 BarrierStaffTargetChange(struct Proc* proc, struct TargetEntry* target) {
    ChangeActiveUnitFacing(target->x, target->y);
    RefreshUnitResChangeInfoWindow(GetUnit(target->unitIndex));
	return 1;
}

void MakeTargetListForBarrier(struct Unit* unit) {
	int x = unit->xPos;
	int y = unit->yPos;
	gUnitSubject = unit;
	
	InitTargets(x, y);
	
	// ier thingamabob
	Item_TURange(unit, AddUnitToTargetListIfUnbarriered, BarrierStaffID_Link);
}

void AddUnitToTargetListIfUnbarriered(Unit* unit) {
	if (AreAllegiancesAllied(gUnitSubject->index, unit->index)
		&& !IsConcentrateBitSet(unit)) {
		AddTarget(unit->xPos, unit->yPos, unit->index, 0);
	}
}

bool BarrierUsability(struct Unit* unit) {
	MakeTargetListForBarrier(unit);
	return GetTargetListSize() != 0;
}

void ExecBarrierStaff(Proc* proc) {
	//set attacker & the item to apply the effect of
	BattleInitItemEffect(GetUnit(gActionData.subjectIndex),gActionData.itemSlotIndex);
	//set defender
	BattleInitItemEffectTarget(GetUnit(gActionData.targetIndex));
	
	//do the staff-specific effect action
	SetBarrierBit(GetUnit(gActionData.targetIndex));
	
	//perform the item effect & do battle anims
	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
}