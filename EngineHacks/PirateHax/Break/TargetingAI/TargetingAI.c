#include "TargetingAI.h"

extern bool(*gSkillTester)(struct Unit* unit, int skillID);
extern u8 ShadeIDLink;

void ComputeAiAttackWeight(struct AiCombatSimulationSt* st) {
    int score;

    score = AiBattleGetDamageDealtWeight(); //max score: 110

    score -= AiBattleGetDamageTakenWeight(); //minimum score: -110

    if (score < 1) {
        score = 1;
    }

    if (DidUnitBreak() && gBattleActor.battleEffectiveHitRate > 0){
        score += (50 * gBattleActor.battleEffectiveHitRate) / 100; 
    }
    else if ((gBattleTarget.weapon == 0 || gBattleTarget.canCounter == false) && (gBattleActor.battleAttack - gBattleTarget.battleDefense) > 0) {
        score += (50 * gBattleActor.battleEffectiveHitRate) / 100; 
    }
    else{

    }

    if (gSkillTester(&gBattleTarget.unit, ShadeIDLink))
    {
        score = 0; // if you have shade, then they're gonna ignore you unless they have literally no other option
    }

    st->score = score;

    return;
}

int AiBattleGetDamageDealtWeight(void) {
    int score;
    
    if (gBattleTarget.unit.curHP == 0) {
        return 500; //if enemy dies and actor doesn't, very big value considered
    }

    if ((gBattleActor.battleAttack - gBattleTarget.battleDefense) <= 0){
        return 0; //if do no damage, consider nothing
    }

    if (gBattleActor.battleEffectiveHitRate == 0){
        return 0; //if guaranteed miss, consider nothing
    }

    score = (gBattleActor.battleAttack - gBattleTarget.battleDefense) * (gBattleActor.battleEffectiveHitRate);
    score /= 100; //the score for a single attack = damage * hit rate (as a percent)

    if (score < 0) {
        score = 0;
    }

    if (score > 50) { 
        score = 50; //cap for non-kills is 40
    }

    return score;
}

int AiBattleGetDamageTakenWeight(void) {
    int score;

    if (gBattleTarget.unit.curHP <= 0){
        return 0; //if they would kill, consider no damage taken to prioritize it
    }

    if ((gBattleTarget.weapon == 0 || gBattleTarget.canCounter == false)) {
        return 0; //they have no weapon, so no counter
    }

    if ((gBattleTarget.battleAttack - gBattleActor.battleDefense) <= 0 || gBattleActor.battleEffectiveHitRate == 0){
        return 0; //no damage, no fear
    }

    if (DidUnitBreak() && gBattleActor.battleEffectiveHitRate > 0 ){
        return 0; //if unit would break in this combat, they believe they face no counter
    }
    
    score = (gBattleTarget.battleAttack - gBattleActor.battleDefense) * (gBattleTarget.battleEffectiveHitRate);
    score /= 100; //score for one attack from the enemy

    if (score < 0) {
        score = 0;
    }

    if (score > 50) {
        score = 50;
    }

    return score;
}