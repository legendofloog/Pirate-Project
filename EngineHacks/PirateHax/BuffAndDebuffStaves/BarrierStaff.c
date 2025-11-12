#include "BarrierStaff.h"

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
	
    DrawUnitResChangeText(proc->lines + 0, unit, 7); //it's barrier chief
    Text_Display(proc->lines, gBg0MapBuffer + TILEMAP_INDEX(x + 1, y + 3));
}
 
u8 BarrierStaffTargetChange(struct Proc* proc, struct TargetEntry* target) {
    ChangeActiveUnitFacing(target->x, target->y);
    RefreshUnitResChangeInfoWindow(GetUnit(target->unitIndex));
	return 1;
}

