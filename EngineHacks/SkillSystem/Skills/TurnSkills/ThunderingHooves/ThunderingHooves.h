#include "gbafe.h"

extern u8 AuraSkillCheck(Unit* unit, int skillID, int allyOption, int maxRange);
extern u8 ThunderingHoovesID_Link;
extern u8 ThunderingHoovesDamage_Link;
extern u8 ThunderingHoovesRange_Link;
extern bool IsUnitOnField(Unit* unit);
void CallMapEventEngine(const void* scene, int runKind);
const void* SavageBlowEvent();