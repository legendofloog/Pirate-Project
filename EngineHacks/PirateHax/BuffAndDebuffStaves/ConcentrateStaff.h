#include "gbafe.h"

// Concentrate staff EA labels
extern u32 ConcentrateStaffBitOffset_Link;
extern u8 ConcentrateStaffBuffAmount_Link;
extern u16 ConcentrateStaffSubtitleText_Link;
extern struct TargetSelectionDefinition CONST_DATA ConcentrateStaffSelectInfo;
extern u8 ConcentrateStaffID_Link;

//function prototypes
bool IsConcentrateBitSet(Unit* unit);
void SetConcentrateBit(Unit* unit);
void ReduceConcentrateBit(Unit* unit);
void ClearConcentrateBitEachTurn();
void AddUnitToTargetListIfUnfortunate(Unit* unit);
void MakeTargetListForConcentrate(Unit* unit);
void ConcentrateUsabilityWrapper();
bool ConcentrateUsability(Unit* unit);

