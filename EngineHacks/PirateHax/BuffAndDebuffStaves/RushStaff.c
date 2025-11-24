#include "RushStaff.h"

bool IsRushStaffBitSet(Unit* unit) {
	if (UnpackData(GetUnitDebuffEntry(unit), RushStaffBitOffset_Link, 3)) return true;

	return false;
}

void SetRushBit(Unit* unit) {
	PackData(GetUnitDebuffEntry(unit), RushStaffBitOffset_Link, 3, 3);
}

void ReduceRushBit(Unit* unit) {
	if (IsRushStaffBitSet(unit))
	{
		PackData(GetUnitDebuffEntry(unit), RushStaffBitOffset_Link, 3, UnpackData(GetUnitDebuffEntry(unit), RushStaffBitOffset_Link, 3) - 1);
	}		
}

void UnsetRushBits(Unit* unit)
{
	PackData(GetUnitDebuffEntry(unit), RushStaffBitOffset_Link, 3, 0);
}

void ClearRushBitEachTurn() {
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
		struct Unit* curUnit = GetUnit(unitID);
		
		//reduce Concentrate bit
		ReduceRushBit(curUnit);
		
		unitID++;
	}	
}