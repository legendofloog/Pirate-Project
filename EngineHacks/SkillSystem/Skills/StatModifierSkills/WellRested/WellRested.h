#include "gbafe.h"

extern bool SkillTester(Unit* unit, u8 skillID);
extern bool CheckBit(u32* address, u8 bitOffset);
extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern void PackData_Signed(u32* address, u8 bitOffset, u8 bitCount, u8 value);
extern int UnpackData_Signed(u32* address, u8 bitOffset, u8 bitCount);
extern u32* GetUnitDebuffEntry(Unit* unit);

struct BWLData {
/*00*/  u8 losses;
/*01*/  u8 skills[4];
/*05*/  u8 mostRecentDeathChapter : 4;
/*05*/	u16 mostRecentDeathTurn : 12;
/*07*/	u8 unused1;
/*08*/	u8 unused2;
/*09*/	u8 pad[7]; //irrelevant for here
} typedef BWLData;

extern BWLData gBWLDataArray[];

struct WellRestedData {
	s8 wellRestedBoost;
	u8 wellRestedFlag;
} typedef WellRestedData;

extern u8 WellRestedIDLink;
extern u8 WellRestedBitOffset_Link;
extern WellRestedData* WellRestedRAMLoc;

u32 WellRested(u32 stat, Unit* unit); 
u8 WellRested_GetBonus(Unit* unit);
void WellRested_IncrementBonus(Unit* unit);
void WellRested_DecrementBonus(Unit* unit);
void PostPrep_WellRestedBonus();
bool WellRested_CheckAndUnsetUndeployedFlag(Unit* unit);
void WellRested_SetUndeployedFlag(Unit* unit);
bool WellRested_CheckUndeployedFlag(Unit* unit);
BWLData* UnitGetBWLData(Unit* unit);
WellRestedData* WellRested_SaveByte(WellRestedData* saveData);
Unit* WellRested_LoadByte(WellRestedData* saveData);

extern u8 A3ChapterLink;