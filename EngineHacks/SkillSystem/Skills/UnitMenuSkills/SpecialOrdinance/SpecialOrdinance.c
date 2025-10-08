#include "gbafe.h"

#define US_GALEFORCE 0x400
#define SONG_SE_SYS_WINDOW_SELECT1 0x6A
#define TARGETSELECTION_ACTION_ENDFAST  1<<0
#define TARGETSELECTION_ACTION_END      1<<1
#define TARGETSELECTION_ACTION_SE_6A    1<<2
#define TARGETSELECTION_ACTION_CLEARBGS 1<<4

#define ITEM_MINE 0x7A
#define TRAP_FIRE_THIEF 15
#define TRAP_MINE_ASSASSIN 16

extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(struct Unit* unit);
extern u32 GaleforceEvent;

extern int BreakBitOffset_Link;
extern int BreakInBattleBitOffset_Link;




struct SelectTarget
{
    /* 00 */ s8 x, y;
    /* 02 */ s8 uid;
    /* 03 */ s8 extra;

    /* 04 */ struct SelectTarget* next;
    /* 08 */ struct SelectTarget* prev;
};

struct ProcBmTrap {
    /* 00 */ PROC_HEADER;

    
	u8 pad27[0x27];

    /* 50 */ s16 post_exec_type;

    /* 54 */ struct Unit* unit;
	
	/* 58 */ u8 trapExt;
};

extern bool SkillTester(Unit* unit, u8 skill);
extern u8 SpecialOrdinanceID_Link;
extern u8 BreakMineActionID_Link;



int SpecialOrdinanceUsability(MenuCommandDefinition* command, int number);
int SpecialOrdinanceEffect(MenuProc* menu, MenuCommandProc* menuItem);
void DoUsePutTrap_BreakMine(Unit* unit);
u8 OnSelectPutTrap_BreakMine(Proc* proc, struct SelectTarget* target);
void ExecBreakMine(Proc* proc);
//Override
void MineFireTrap_80375A0(struct ProcBmTrap* proc);
void SpecialOrdinance_PostBattle();


int SpecialOrdinanceUsability(MenuCommandDefinition* command, int number) {
	
	//has skill + valid place for a trap + haven't used it this turn already (galeforce bit?)
	
	MakeTargetListForMine(gActiveUnit);
	
	//if target list empty or don't have skill, unusable
	if (GetTargetListSize() <= 0 || (gActiveUnit->state & US_GALEFORCE) || (gActiveUnit->state & US_HAS_MOVED) || !SkillTester(gActiveUnit, SpecialOrdinanceID_Link)) return MCA_NONUSABLE;
	
	return MCA_USABLE;
	
}


int SpecialOrdinanceEffect(MenuProc* menu, MenuCommandProc* menuItem) {
	
	ClearBG0BG1();
	
	//calls DoUsePutTrap_BreakMine with active unit
	DoUsePutTrap_BreakMine(gActiveUnit);
	
	return ME_DISABLE | ME_END | ME_PLAY_BEEP;
	
}


void DoUsePutTrap_BreakMine(Unit* unit) {
    MakeTargetListForMine(unit);

    BmMapFill(gMapMovement, -1);

    StartBottomHelpText(
        StartTargetSelectionExt((0x859D2D8), OnSelectPutTrap_BreakMine),
        GetStringFromIndex(0x87D));

    PlaySoundEffect(SONG_SE_SYS_WINDOW_SELECT1);
}


u8 OnSelectPutTrap_BreakMine(Proc* proc, struct SelectTarget* target) {
    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    HideMoveRangeGraphics();

    BG_Fill(gBg2MapBuffer, 0);
    EnableBgSyncByMask(BG2_SYNC_BIT);

    gActionData.unitActionType = BreakMineActionID_Link;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}



void ExecBreakMine(Proc* proc) {
    BattleInitItemEffect(GetUnit(gActionData.subjectIndex),
        gActionData.itemSlotIndex);

    AddTrap(gActionData.xOther, gActionData.yOther, TRAP_MINE, 1);

    //BattleApplyItemEffect(proc);

    gBattleTarget.statusOut = -1;

    BeginMineMapAnim(proc, gActionData.xOther, gActionData.yOther);
	
	//set galeforce bit(?) on active unit
	//gActiveUnit->state |= US_GALEFORCE;

    return;
}

void SpecialOrdinance_PostBattle() {
	if (gActionData.unitActionType == BreakMineActionID_Link) {
		if ((gActiveUnit->state & 0x400) == 0) {
		u32 newState = gActiveUnit->state;
		newState = newState & (0xFFFFFFFF - 0x42); //unset 0x42
		newState = newState | 0x400; //set 0x400
		gActiveUnit->state = newState;
		CallMapEventEngine(&GaleforceEvent,1); //play sfx
		}
	}
}






void MineFireTrap_80375A0(struct ProcBmTrap* proc) {
    Unit* unit = proc->unit;

    ApplyHazardHealing(proc, unit, -10, -1);

    if (GetUnitCurrentHp(unit) == 0)
    {
        struct Unit * tmp = gActiveUnit;
        gActiveUnit = unit;
        
        BWL_AddWinOrLossIdk(unit->pCharacterData->number, 0, 3);

        if (CheckForPostActionEvents() != 0)
            RunPostActionEvents();

        gActiveUnit = tmp;
    }
	
	//if the trap type is a mine and the value set earlier is 1, break the unit too
	if (proc->trapExt == 1) SetBit(GetUnitDebuffEntry(unit), BreakBitOffset_Link);
	
	
}

//Override
int ExecTrap(Proc* proc, Unit* unit, int exec_type)
{
    struct ProcBmTrap * proc2;

    switch (GetTriggeredTrapType(unit)) {
        case TRAP_8:
            proc2 = ProcStartBlocking(0x859E5AC, proc);
            proc2->post_exec_type = exec_type;
            proc2->unit = unit;
            break;

        case TRAP_MINE:
            int tmp = GetSpecificTrapAt(unit->xPos, unit->yPos, TRAP_MINE)->data[0];
			RemoveTrap(GetSpecificTrapAt(unit->xPos, unit->yPos, TRAP_MINE));
			proc2 = ProcStartBlocking(0x859E5FC, proc);
            proc2->post_exec_type = exec_type;
            proc2->unit = unit;
            proc2->trapExt = tmp;
			break;

        case TRAP_FIRE_THIEF:
            RemoveTrap(GetTrapAt(unit->xPos, unit->yPos));
            PlaySoundEffect(0xB1);
            NewPopup2_PlanA(proc, -1, GetStringFromIndex(0x20));    /* Disabled trap. */
            break;

        case TRAP_MINE_ASSASSIN:
            RemoveTrap(GetTrapAt(unit->xPos, unit->yPos));
            PlaySoundEffect(0xB1);
            NewPopup2_PlanA(proc, -1, GetStringFromIndex(0x21));    /* Recovered mine. */
            UnitAddItem(unit, MakeNewItem(ITEM_MINE));
            break;
    }

    return 0;
}
