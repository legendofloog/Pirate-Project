#include "gbafe.h"

extern bool SkillTester(Unit* unit, u8 skillID);
extern s8 AreUnitsAllied(int left, int right);

extern u8 MiraclemakerID_Link;
extern u32 MiraclemakerBitOffset_Link;

extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(Unit* unit);
extern bool IsUnitOnField(Unit* unit);

void Miraclemaker(Unit* unit) {
	if(gActionData.unitActionType == UNIT_ACTION_STAFF && SkillTester(unit, MiraclemakerID_Link) && AreUnitsAllied(gActionData.subjectIndex, gActionData.targetIndex)) {
		u32* entry = GetUnitDebuffEntry(GetUnit(gActionData.targetIndex));
		SetBit(entry, MiraclemakerBitOffset_Link);
	}
}

void Miraclemaker_BitReset() {
	//this runs in start of turn calc loop
	//for each unit of the current phase, clear miraclemaker bit
	
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
		
		//are they on the map?
		if (IsUnitOnField(curUnit)) {

			//clear miraclemaker bit
			u32* entry = GetUnitDebuffEntry(curUnit);
			UnsetBit(entry,MiraclemakerBitOffset_Link);
			
		}
		
		unitID++;
	}	
}
