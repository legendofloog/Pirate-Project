#include "gbafe.h"

// fortune staff EA labels
extern u16 BarrierStaffSubtitleText_Link;
extern struct TargetSelectionDefinition CONST_DATA BarrierStaffSelectInfo;

extern void MakeTargetListForBarrier(Unit* unit);

extern u8 BarrierStaffID_Link;
void AddUnitToTargetListIfUnbarriered(Unit* unit);
void MakeTargetListForBarrier(struct Unit* unit);

extern u32 BarrierStaffBitOffset_Link;
extern u8 BarrierStaffBuffAmount_Link;