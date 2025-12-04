#include "gbafe.h"

#define KiteSummon 0x3D
#define BITPACKED __attribute__((aligned(4), packed))

struct UnitUsageStats {
    /* 000 */ unsigned lossAmt     : 8;
    /* 008 */ unsigned favval      : 16;
    /* 024 */ unsigned actAmt      : 8;
    /* 032 */ unsigned statViewAmt : 8;
    /* 040 */ unsigned deathLoc    : 6;
    /* 046 */ unsigned deathTurn   : 10;
    /* 056 */ unsigned deployAmt   : 6;
    /* 062 */ unsigned moveAmt     : 10;
    /* 072 */ unsigned deathCause  : 4;
    /* 076 */ unsigned expGained   : 12;
    /* 088 */ unsigned winAmt      : 10;
    /* 098 */ unsigned battleAmt   : 12;
    /* 110 */ unsigned killerPid   : 9;
    /* 119 */ unsigned deathSkirm  : 1;
    /* 120 */ /* 8bit pad */
} BITPACKED;

//defined in EA
extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(Unit* unit);
extern u8 NoMoveBitOffset_Link;
extern void GaleforceEvent;

//defined in ref
extern struct UnitUsageStats* BWL_GetEntry(u8 charID);

//prototypes
void ContraryWinds_PostBattle();
long long ContraryWinds_NoMove(u8 stat, Unit* unit);
long long ContraryWinds_KiteDebuff(u8 stat, Unit* unit);
void ContraryWinds_ClearNoMoveBit();


void ContraryWinds_PostBattle() {
	//if active unit's action was summoning a kite
	if (gActionData.unitActionType == UNIT_ACTION_SUMMON) {
		//debuff move to 0
		SetBit(GetUnitDebuffEntry(gActiveUnit), NoMoveBitOffset_Link);
		
		//refresh unit
		//unset status 0x42, set 0x400
		gActiveUnit->state &= 0xFFFFFFBD;
		gActiveUnit->state |= 0x0400;
		
		//call galeforce event (just does a sound effect)
		CallMapEventEngine(&GaleforceEvent, EV_RUN_CUTSCENE);
		
		//find kite (should only ever be 1 on map)
		Unit* kite = GetUnitByCharId(KiteSummon);
		if (kite->pCharacterData == 0) return;
		
		//set kite's BWL data for turn died on to current turn count
		struct UnitUsageStats* bwl = BWL_GetEntry(KiteSummon);
		bwl->deathTurn = gChapterData.turnNumber;

		// set the kite's axe rank down to 0, because apparently i need to do that
		kite->ranks[ITYPE_AXE] = 0;
		
	}
}

long long ContraryWinds_NoMove(u8 stat, Unit* unit) {
	if (CheckBit(GetUnitDebuffEntry(unit), NoMoveBitOffset_Link)) stat = 0;
	
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

long long ContraryWinds_KiteDebuff(u8 stat, Unit* unit) {
	if (unit->pCharacterData->number == KiteSummon) {
		struct UnitUsageStats* bwl = BWL_GetEntry(KiteSummon);
		//debuff stat by 2*(turnNumber - deathTurn)
		stat -= (gChapterData.turnNumber - bwl->deathTurn)*2;
	}
	
	union {
		long long asLongLong;
		struct {
			u32 stat;
			Unit* unit;
		};
	} result;

	if (stat <= 0) stat = 0;

	result.stat = stat;
	result.unit = unit;

	return result.asLongLong;	
		
}

void ContraryWinds_ClearNoMoveBit() {
	//this runs in start of turn calc loop
	//clears NoMoveBit for all units of current phase
	
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
		
		//clear NoMove bit
		u32* entry = GetUnitDebuffEntry(curUnit);
		UnsetBit(entry,NoMoveBitOffset_Link);

		unitID++;
	}	
}

