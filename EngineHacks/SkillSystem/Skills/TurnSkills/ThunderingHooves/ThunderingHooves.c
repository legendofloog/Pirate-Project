#include "ThunderingHooves.h"

void ThunderingHooves() {
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
		Unit* curUnit = GetUnit(unitID);
		
		if (IsUnitOnField(curUnit)) {
			if (AuraSkillCheck(curUnit, ThunderingHoovesID_Link, 0x2, ThunderingHoovesRange_Link)) { // i think this is for enemies?
				curUnit->curHP = curUnit->curHP - (curUnit->maxHP * ThunderingHoovesDamage_Link / 100);
				if (curUnit->curHP <= 0) curUnit->curHP = 1;
			}
		}
		unitID++;
	}
}

void DoThunderingHoovesEvent(Unit* unit) {
	if (IsUnitOnField(unit)) //nothing yet
	{
		CallMapEventEngine(SavageBlowEvent, EV_RUN_CUTSCENE);
	}	
}

bool IsThunderingHoovesApplicable() {
	return true;
}