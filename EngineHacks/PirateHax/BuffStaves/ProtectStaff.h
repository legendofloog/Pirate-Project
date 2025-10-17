#include "gbafe.h"

// fortune staff EA labels
extern u32 ProtectStaffBitOffset_Link;
extern u8 ProtectStaffBuffAmount_Link;
extern u16 ProtectStaffSubtitleText_Link;
extern struct TargetSelectionDefinition CONST_DATA ProtectStaffSelectInfo;
extern u8 ProtectStaffID_Link;

//function prototypes
bool IsProtectBitSet(Unit* unit);
void SetProtectBit(Unit* unit);
void UnsetProtectBit(Unit* unit);
void ProtectHitBoost(BattleUnit* bunitA, BattleUnit* bunitB);
void ClearProtectBitEachTurn();
void AddUnitToTargetListIfUnfortunate(Unit* unit);
void MakeTargetListForProtect(Unit* unit);
void ProtectUsabilityWrapper();
bool ProtectUsability(Unit* unit);

