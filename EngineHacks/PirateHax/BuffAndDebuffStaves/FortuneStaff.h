#include "gbafe.h"

// fortune staff EA labels
extern u32 FortuneStaffBitOffset_Link;
extern u8 FortuneStaffBuffAmount_Link;
extern u16 FortuneStaffSubtitleText_Link;
extern struct TargetSelectionDefinition CONST_DATA FortuneStaffSelectInfo;
extern u8 FortuneStaffID_Link;

//function prototypes
bool IsFortuneBitSet(Unit* unit);
void SetFortuneBit(Unit* unit);
void UnsetFortuneBit(Unit* unit);
void FortuneHitBoost(BattleUnit* bunitA, BattleUnit* bunitB);
void ClearFortuneBitEachTurn();
void AddUnitToTargetListIfUnfortunate(Unit* unit);
void MakeTargetListForFortune(Unit* unit);
void FortuneUsabilityWrapper();
bool FortuneUsability(Unit* unit);

