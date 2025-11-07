#include "AstralBlessing.h"

bool AuraSkillCheckWrapper(Unit* unit, int skill, int param, int maxRange) {

	long long result = AuraSkillCheck(unit, skill, param, maxRange);
	return (u8)result;
	
}

void AstralBlessing() {
	//this runs in start of turn calc loop
	//for each unit of the current phase, look for one having AstralBlessing
	
	int faction = gChapterData.currentPhase;
	int unitID = faction+1;
	int maxCount;
	
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
		
		//are they on the map?
		if (IsUnitOnField(curUnit)) {

			//clear astral blessing bits
			u32* entry = GetUnitDebuffEntry(curUnit);
			UnsetBit(entry,AstralBlessingSklOffset_Link);
			UnsetBit(entry,AstralBlessingSpdOffset_Link);
			UnsetBit(entry,AstralBlessingLckOffset_Link);

			//do they have astral blessing?
			if (AuraSkillCheckWrapper(curUnit, AstralBlessingIDLink, 0, AstralBlessing_Range)) ApplyAstralBlessing(curUnit);
		}
		
		unitID++;
	}	
}
	
void ApplyAstralBlessing(Unit* unit) { 
	//get a random number from 0-2
	int rn = NextRN_N(3);
	u32* entry = GetUnitDebuffEntry(unit);
	switch (rn) {
		case 0:
		//Skl+4
		SetBit(entry, AstralBlessingSklOffset_Link);
		break;
		
		case 1:
		//Spd+4
		SetBit(entry, AstralBlessingSpdOffset_Link);
		break;
		
		case 2:
		//Lck+5
		SetBit(entry, AstralBlessingLckOffset_Link);
		break;
	}
	
}

long long AstralBlessingSpdBoost(u8 stat, Unit* unit) {
	if (CheckBit(GetUnitDebuffEntry(unit), AstralBlessingSpdOffset_Link)) stat += AstralBlessing_Spd;
	
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

long long AstralBlessingSklBoost(u8 stat, Unit* unit) {
	if (CheckBit(GetUnitDebuffEntry(unit), AstralBlessingSklOffset_Link)) stat += AstralBlessing_Skl;

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

long long AstralBlessingLukBoost(u8 stat, Unit* unit) {
	if (CheckBit(GetUnitDebuffEntry(unit), AstralBlessingLckOffset_Link)) stat += AstralBlessing_Luk;

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

void DoAstralBlessingBuffFx(Unit* unit) {
	if (IsUnitOnField(unit)) StartBuffFx(unit,0x80,AstralBlessing_Range);

/* 	
	Proc* parentProc = ProcFind(&BuffAnimationSkillProc);	
	FxProc* newProc = ProcStartBlocking(&BuffFxProc,parentProc);

	newProc->unit = unit;
	newProc->effectBits = 0x80; //rally spectrum animation
	newProc->range = AstralBlessing_Range;
 */	
	/*
	mov r4, r0 @ unit 
	mov r5, r1 @ rally bit(s) to set 
	mov r6, r2 @ effect range 
	ldr r0, =BuffFxProc @ arg r0 = proc scr
	mov r1, #3           @ arg r1 = parent
	blh StartProc
	str r4, [r0, #0x30] @ unit 
	str r5, [r0, #0x34] @ bits 
	str r6, [r0, #0x38] @ effect range 
	*/
	
}

bool IsAstralBlessingApplicable() {
	return true;
}

