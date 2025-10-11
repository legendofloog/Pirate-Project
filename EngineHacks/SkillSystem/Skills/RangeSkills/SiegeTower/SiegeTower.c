#include "gbafe.h"

extern u8 SiegeTowerIDLink;
extern bool SkillTester(Unit* unit, u8 skillID);

u32 SiegeTowerBonus(Unit* unit, u16 item, u32 minMaxRange);
 
//Siege Tower: When initiating combat with a bow without moving, +4 max range.
u32 SiegeTowerBonus(Unit* unit, u16 item, u32 minMaxRange) {
	//if we are gBattleActor AND we are using a bow AND spent movement is 0 
	if (  ( unit->index == gBattleActor.unit.index ) 
		  && (gBattleActor.weaponType == ITYPE_BOW) 
		  && (gActionData.moveCount == 0) )
		  return minMaxRange+4;
	return minMaxRange;
	
	
}
