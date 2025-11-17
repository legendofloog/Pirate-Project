#include "WellRested.h"

u32 WellRested(u32 stat, Unit* unit) {

	//does unit have skill?
	if (SkillTester(unit,WellRestedIDLink)) {
		//add our bonus value to the stat
		stat += WellRested_GetBonus(unit);
		//if we were deployed last time, -1 to bonus at prep
		//commenting this part out for now: seems like it confuses some people as to whether the change has already happened or not - Loog
		//if (!WellRested_CheckUndeployedFlag(unit) && (gChapterData.chapterStateBits & 0x10) && (gChapterData.chapterIndex != A3ChapterLink)) stat--;
		
	}
	return stat;
}

u8 WellRested_GetBonus(Unit* unit) {
	//get the current well-rested bonus
	return WellRestedRAMLoc->wellRestedBoost;
}

void WellRested_IncrementBonus(Unit* unit) {
	int val = WellRestedRAMLoc->wellRestedBoost;
	val++;
	val++;
	if (val > 15) val = 15;
	WellRestedRAMLoc->wellRestedBoost = val;	
}

void WellRested_DecrementBonus(Unit* unit) {
	int val = WellRestedRAMLoc->wellRestedBoost;
	val--;
	if (val < 0) val = 0;
	WellRestedRAMLoc->wellRestedBoost = val;	
}

bool WellRested_CheckAndUnsetUndeployedFlag(Unit* unit) {
	bool retVal = WellRestedRAMLoc->wellRestedFlag;
	WellRestedRAMLoc->wellRestedFlag = 0;
	return retVal;
}

bool WellRested_CheckUndeployedFlag(Unit* unit) {
	return WellRestedRAMLoc->wellRestedFlag;
}

void WellRested_SetUndeployedFlag(Unit* unit) {
	WellRestedRAMLoc->wellRestedFlag = 1;
}

void PostPrep_WellRestedBonus() {
	//for each player unit, if they have WellRested, either increment or decrement their bonus
	
	for (int i = 1; i < 64; i++) {
		
		Unit* unit = GetUnit(i);
		if (unit->maxHP == 0) continue;
	
		if (SkillTester(unit, WellRestedIDLink)) {
			if (unit->state & US_NOT_DEPLOYED) { 
			//we weren't deployed, so set a bit to denote this
			WellRested_SetUndeployedFlag(unit);
			//then increment our bonus
			WellRested_IncrementBonus(unit); 
			}
			else { 
			//if we were not deployed last chapter, do nothing
			if (WellRested_CheckAndUnsetUndeployedFlag(unit)) continue;
			//otherwise, decrement the bonus
			WellRested_DecrementBonus(unit); }
		}
	}
}

WellRestedData* WellRested_SaveByte(WellRestedData* saveData) {
	if (saveData->wellRestedFlag != 0xFF) {
		saveData->wellRestedBoost = WellRestedRAMLoc->wellRestedBoost;
		saveData->wellRestedFlag = WellRestedRAMLoc->wellRestedFlag;
	}
	else {
		saveData->wellRestedBoost = 0;
		saveData->wellRestedFlag = 0;
	}
}

Unit* WellRested_LoadByte(WellRestedData* saveData) {
	if (saveData->wellRestedFlag != 0xFF) {
		WellRestedRAMLoc->wellRestedBoost = saveData->wellRestedBoost;
		WellRestedRAMLoc->wellRestedFlag = saveData->wellRestedFlag;	
	}
	else {
		saveData->wellRestedBoost = 0;
		saveData->wellRestedFlag = 0;
		WellRestedRAMLoc->wellRestedFlag = 0;
		WellRestedRAMLoc->wellRestedBoost = 0; 
	}
}
