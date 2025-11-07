#include "gbafe.h"

#include "AI_EnemyDeterminer.h"

s8 AreUnitsAllied(int left, int right) {
    int a = left & 0x80;
    int b = right & 0x80;
    return (a == b);
}

s8 IsUnitEnemyWithActiveUnit(struct Unit* unit) { //for AI specifically, unit is the target

    if (AreUnitsAllied(gActiveUnit->index, unit->index)) { //the standard check
        return 0;
    }

    //A-3 checks for log
    if (unit->pCharacterData->number == A3LogIDLink){
        return 0; //do not attack the a3 logs, as they aren't enemies
    }

    // We need to do D-3 checks here for the gimmick to work: Want Incendivis troops to ignore each other

    if ((gActiveUnit->pCharacterData->number == D3TorchUnitCharIDLink) && (UNIT_FACTION(unit) == FACTION_GREEN)){
        return 0; //D-3 incendivis dudes will not attack any green unit, most common issue
    }
    if ((gActiveUnit->pCharacterData->number == EnriqueCharIDLink) && (UNIT_FACTION(unit) == FACTION_GREEN)){
        return 0; //neither will their boss
    }
    if ((UNIT_FACTION(gActiveUnit) == FACTION_GREEN && gActiveUnit->pCharacterData->number == D3TorchUnitCharIDLink)){
        return 0; //any green units here (only incendivis and fidel) will not attack torch units
    }
    if ((UNIT_FACTION(gActiveUnit) == FACTION_GREEN && gActiveUnit->pCharacterData->number == EnriqueCharIDLink)){
        return 0; //any green units here (only incendivis and fidel) will not attack enrique, either
    }

    if ((UNIT_FACTION(gActiveUnit) == FACTION_RED) && ((unit->pCharacterData->number == D3Thief1CharIDLink) || (unit->pCharacterData->number == D3Thief2CharIDLink)))
    {
        return 0;
    }
    
    return 1;
}


/*
bool IsUnitAValidTarget(Unit* actor, Unit* target){ //this is for player units i believe, don't actually need anything for this currently but copied it over in case I do
    if (AreUnitsAllied(target->index, actor->index)){
        return false;
    }
    if (gSkillTester(target, NonCombatantIDLink)){
        return false;
    }
    return true;
}
*/

/*
void AddUnitToTargetListIfNotAllied(struct Unit* unit) {

    if (IsUnitAValidTarget(gActiveUnit, unit)){
        AddTarget(unit->xPos, unit->yPos, unit->index, 0);
    }
    return;
}*/