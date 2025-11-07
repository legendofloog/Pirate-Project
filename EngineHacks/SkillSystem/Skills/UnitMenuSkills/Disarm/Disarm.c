#include "Disarm.h"

struct SelectInfo const SelectInfo_Disarm =
{
    .onInit = DisarmSelection_OnInit,
    .onSwitchIn = DisarmSelection_OnChange,
    .onSelect = DisarmSelection_OnSelect,
    .onCancel = GenericSelection_BackToUM,
};

u8 DisarmUsability(const struct MenuItemDef* def, int number) {
	if (gActiveUnit->state & US_HAS_MOVED) {
        return MENU_NOTSHOWN;
    }
    if (gBmSt.unk3D & 1) {
        return MENU_NOTSHOWN;
    }
	if (gSkillTester(gActiveUnit, DisarmIDLink) == 0){ // dont use not for some magic reason
		return MENU_NOTSHOWN;
	}
    MakeDisarmTargetList(gActiveUnit);
    if (GetSelectTargetCount() == 0) {
        return MENU_NOTSHOWN;
    }
    return MENU_ENABLED;
}

u8 DisarmEffect(struct MenuProc* menu, struct MenuItemProc* menuItem) {
	MakeDisarmTargetList(gActiveUnit);
    NewTargetSelection(&SelectInfo_Disarm);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

u8 DisarmSelection_OnSelect(ProcPtr proc, struct SelectTarget* target) {
    gActionData.unitActionType = UNIT_ACTION_TAKE; //this should do nothing
    gActionData.targetIndex = target->uid;
    UnitFinalizeMovement(GetUnit(gActionData.targetIndex));
    DoDisarmEffect(target->uid);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

void DisarmSelection_OnInit(ProcPtr menu) {
    ClearBg0Bg1();
    StartSubtitleHelp(menu, GetStringFromIndex(DisarmHelpTextLink));
    return;
}

u8 DisarmSelection_OnChange(ProcPtr proc, struct SelectTarget* target) {
    ChangeActiveUnitFacing(target->x, target->y);
    return 0;
}

void DoDisarmEffect(int targetUid) { //left off here, keep it going
    gActionData.unitActionType = UNIT_ACTION_TAKE; //should do nothing
	struct Unit* targetUnit = GetUnit(targetUid);
	u32* entry = GetUnitDebuffEntry(targetUnit);
	SetBit(entry, BreakBitOffset_Link);
    ApplyDebuff(targetUnit, 5, 3);
    ApplyDebuff(targetUnit, 5, 2);
    CallMapEventEngine(DisarmMapEvent, EV_EXEC_CUTSCENE);
    //BeginMapAnimForSteal();
}


void MakeDisarmTargetList(struct Unit* unit) {
    int x = unit->xPos;
    int y = unit->yPos;
    gSubjectUnit = unit;
    BmMapFill(gBmMapRange, 0);
    ForEachAdjacentUnit(x, y, TryAddUnitToDisarmTargetList);
    return;
}

void TryAddUnitToDisarmTargetList(struct Unit* unit) {
	if (AreUnitsAllied(gSubjectUnit->index, unit->index)) { //if they're allies, don't do it
        return;
    }
    AddTarget(unit->xPos, unit->yPos, unit->index, 0);
    return;
}