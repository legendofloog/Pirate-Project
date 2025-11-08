#include "gbafe.h"

extern bool AuraSkillCheck(Unit* unit, int skillID, int allyOption, int maxRange);

extern u8 EqualizerID_Link;
extern u8 EqualizerRange_Link;

void Equalizer(BattleUnit* bunitA, BattleUnit* bunitB) {
	if (bunitA == &gBattleActor){
		return;
	}
	
	if (gBattleStats.config & BATTLE_CONFIG_BIT2)
    {
        return;
    }

	int meanAtk;
	if (bunitA->battleAttack == bunitB->battleAttack)
	{
		return; //don't do anything, we're already equalized
	}

	if(AuraSkillCheck(&bunitA->unit, EqualizerID_Link, 0x0, EqualizerRange_Link)) { //does an ally check for equalizer skill
		if(bunitA->battleAttack < bunitB->battleAttack && (bunitB->canCounter == true))
		{	//if equalizer unit's damage is less than enemy's damage
			meanAtk = (bunitA->battleAttack + bunitB->battleAttack) / 2;
			bunitA->battleAttack = meanAtk;
			bunitB->battleAttack = meanAtk;
		}
	}
	else if (AuraSkillCheck(&bunitB->unit, EqualizerID_Link, 0x0, EqualizerRange_Link)) { //does an ally check for equalizer skill
		if(bunitB->battleAttack < bunitA->battleAttack && (bunitA->canCounter == true))
		{	//if equalizer unit's damage is less than enemy's damage
			meanAtk = (bunitA->battleAttack + bunitB->battleAttack) / 2;
			bunitA->battleAttack = meanAtk;
			bunitB->battleAttack = meanAtk;
		}
	}
}
