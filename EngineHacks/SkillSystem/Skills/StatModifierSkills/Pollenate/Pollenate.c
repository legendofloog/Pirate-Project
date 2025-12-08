#include "gbafe.h"

#define UNIT_ACTION_POLLENATE 0x2A

// defined in EA
extern bool SkillTester(Unit* unit, u8 skillID);
extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(Unit* unit);
extern u32 UnpackData(u32* address, u8 bitOffset, u8 bitCount);
extern u32 PackData(u32* address, u8 bitOffset, u8 bitCount, u32 data);

extern u8 PollenateID_Link;
extern u8 PollenateBitOffset_Link;
extern u16 PollenateSubtitleHelpTextID_Link;

//defined in ref
extern void GenericSelection_DeleteBBAndBG(TargetSelectionProc* proc);
extern Unit* gUnitSubject;
extern void AddUnitToTargetListIfAllied(Unit* unit);
Proc* StartBottomHelpText(Proc*, const char*);

// function prototypes
u32 GetUnitPollenateValue(Unit* unit);
void SetUnitPollenateValue(Unit* unit, u32 value);
void IncrementUnitPollenateValue(Unit* unit);

long long Pollenate_SpdBoost(u8 stat, Unit* unit);
void Pollenate_IncrementFromStaff();

int PollenateUsability(MenuCommandDefinition* command, int number);
int PollenateEffect(MenuProc* menu, MenuCommandProc* menuItem);

void MakeTargetListForPollenate(Unit* unit);
void AddUnitToPollenateTargetList(Unit* unit);
void PollenateSelection_OnInit(TargetSelectionProc* menu);
int PollenateSelection_OnSelect(TargetSelectionProc* proc, TargetEntry* target);
void PollenateAction();

const struct TargetSelectionDefinition gSelectInfo_Pollenate = 
{
	.onInit = PollenateSelection_OnInit,
	.onEnd = GenericSelection_DeleteBBAndBG,
	.onAPress = PollenateSelection_OnSelect,
	.onBPress = GenericSelection_BackToUM,
};

u32 GetUnitPollenateValue(Unit* unit) {
	return UnpackData(GetUnitDebuffEntry(unit), PollenateBitOffset_Link, 3);
}

void SetUnitPollenateValue(Unit* unit, u32 value) {
	if (value > 5) value = 5;
	PackData(GetUnitDebuffEntry(unit), PollenateBitOffset_Link, 3, value);
}

void IncrementUnitPollenateValue(Unit* unit) {
	SetUnitPollenateValue(unit, UnpackData(GetUnitDebuffEntry(unit), PollenateBitOffset_Link, 3) + 1);
}


long long Pollenate_SpdBoost(u8 stat, Unit* unit) {
	
	//boost stat by amount defined in debuff data for unit
	stat += GetUnitPollenateValue(unit);
	
	union {
		long long asLongLong;
		struct {
			u32 stat;
			Unit* unit;
		};
	} result;

	result.stat = stat;
	result.unit = unit;

	return result.asLongLong;
		
}

void Pollenate_IncrementFromStaff() {
	if (
		//if action taken was staff
		gActionData.unitActionType == UNIT_ACTION_STAFF
		//and unit has Pollenate skill
		&& SkillTester(gActiveUnit, PollenateID_Link)
	) {
		//increment pollenate boost
		IncrementUnitPollenateValue(gActiveUnit);
	}
}

int PollenateUsability(MenuCommandDefinition* command, int number) {
	
	//true if pollenate value isn't 0 and target list isn't empty and has skill and isn't cantoing
	MakeTargetListForPollenate(gActiveUnit);
	if (
		GetUnitPollenateValue(gActiveUnit) != 0 
		&& GetTargetListSize() > 0
		&& SkillTester(gActiveUnit, PollenateID_Link)
		&& !(gActiveUnit->state & US_HAS_MOVED)
	) {
		return MCA_USABLE;
	}
	return MCA_NONUSABLE;
}

int PollenateEffect(MenuProc* menu, MenuCommandProc* menuItem) {
	//start selection with pollenate target list
	ClearBG0BG1();
	
	MakeTargetListForPollenate(gActiveUnit);
	
	StartTargetSelection(&gSelectInfo_Pollenate);
	
	return ME_DISABLE | ME_END | ME_PLAY_BEEP;
	
}

void AddUnitToPollenateTargetList(Unit* unit) {
	if (AreAllegiancesAllied(gUnitSubject->index, unit->index)) {
		AddTarget(unit->xPos, unit->yPos, unit->index, 0);
	}
}

void MakeTargetListForPollenate(Unit* unit) {
	gUnitSubject = unit;
	BmMapFill(gMapRange, 0);
	ForEachAdjacentUnit(unit->xPos, unit->yPos, AddUnitToPollenateTargetList);
}

void PollenateSelection_OnInit(TargetSelectionProc* menu) {
	StartBottomHelpText((Proc*)menu, GetStringFromIndex(PollenateSubtitleHelpTextID_Link));
}

int PollenateSelection_OnSelect(TargetSelectionProc* proc, TargetEntry* target) {
	
	gActionData.xOther = target->x;
	gActionData.yOther = target->y;
	gActionData.targetIndex = target->unitIndex;
	
	gActionData.unitActionType = UNIT_ACTION_POLLENATE;
	
	return ME_DISABLE | ME_END | ME_PLAY_BEEP | ME_CLEAR_GFX;
}

void PollenateAction() {
	//gBattleActor.hasItemEffectTarget = 0;
	Unit* actor = GetUnit(gActionData.subjectIndex);
	Unit* target = GetUnit(gActionData.targetIndex);
	
	//give target actor's pollenate value
	u32 value = GetUnitPollenateValue(actor);
	SetUnitPollenateValue(target, value);
	
	//clear actor's pollenate value
	SetUnitPollenateValue(actor, 0); 	
	
	//possible todo: some effect over the target unit
	
	PlaySfx(0x2F9);
}


