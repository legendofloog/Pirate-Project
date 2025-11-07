#include "ASMCS.h"

#define DesiderioCharId 3
extern u8 SuppliesItemIDLink;
void SetEventId(int eventId);
void UnsetEventId(int eventId);
extern u8 BombCharIDLink;
extern bool(*gSkillTester)(Unit* unit, int skillID);
extern u8 ForestFriendIDLink;

void A3ReturnLogDroppedStatusASMC(){

    gEventSlot[0xC] = 0;

    u8 action = gActionData.unitActionType;

    Unit* target = GetUnit(gActionData.targetIndex);

    if (action != UNIT_ACTION_DROP || target->pClassData->number != 0x8C){
        return; //if action was not drop or target was not log, return 0 (nothing happens)
    }

    if (target->xPos == 0 && target->yPos == 14){

    }
    else if (target->xPos == 10 && target->yPos == 9){

    }
    else if (target->xPos == 18 && target->yPos == 0){

    }
    else{
        return; //if the target wasn't dropped at one of these three pos, return 0
    }

    Unit* currentLog;
    u8 logNum = 0; //number of logs on ruins

    for (int i = FACTION_GREEN + 0x1; i < FACTION_GREEN + 0x4; i++){
        currentLog = gUnitLookup[i]; //whatever log we're at
        if (currentLog->pClassData->number != 0x8C){ 
            break; //not the log class so not something we care about
        }
        if (currentLog->xPos == 0 && currentLog->yPos == 14){
            logNum++; //first pos
        }
        else if (currentLog->xPos == 10 && currentLog->yPos == 9){
            logNum++; //second pos
        }
        else if (currentLog->xPos == 18 && currentLog->yPos == 0){
            logNum++; //third pos
        }
        else{

        }
    }
    
    gEventSlot[0xC] = logNum;
}

void EyeForAnEyeResetASMC(){
    Unit* unit = GetUnitByCharId(DesiderioCharId);
    if (unit->state & US_DEAD){
        //don't do anything because he's goneski
    }
    else{
        unit->supports[5] = 0; //last support byte set to be 0
    }
}

void DoesBattleTargetHaveSuppliesASMC(){
    u16* targetInventory = gBattleTarget.unit.items;
    u16 targetInventoryItem = 0;
    for (int i = 0; i < ITEM_SLOT_COUNT; i++){
        targetInventoryItem = targetInventory[i];
        if (GetItemIndex(targetInventoryItem) == 0xED){ //is the given item a supplies item
            gEventSlot[0xC] = 1;
            break;
        }
        if (targetInventoryItem == 0){
            gEventSlot[0xC] = 0;
        }
    } 
}

void GetNumberOfSuppliesItemsOnHandASMC(){
    u16* convoyArray = GetConvoyItemArray();
    u16 convoyArrayItem;
    int suppliesItemCount = 0;
    for (int i = 0; i <= 200; i++){
        convoyArrayItem = convoyArray[i];
        if (convoyArrayItem == 0){
            break;
        }
        if (GetItemIndex(convoyArrayItem) == SuppliesItemIDLink){
            suppliesItemCount++;
        }
    }

    Unit* someUnit;
    u16 someItem;
    for (int i = 0; i <= 60; i++){
        someUnit = &gUnitArrayBlue[i];
        for (int j = 0; j < 5; j++){
            someItem = someUnit->items[j];
            if (GetItemIndex(someItem) == SuppliesItemIDLink){
                suppliesItemCount++;
            }
        }
    }
    gEventSlot[0xC] = suppliesItemCount;
}

void PlayBombMusicIfBombReadyASMC(){
    int bombX = gEventSlot[0x1];
    int bombY = gEventSlot[0x2];
    Unit* currentUnit = 0;
    bool isBombThere = false;
    for (int i = FACTION_RED; i < FACTION_PURPLE; i++){
        currentUnit = gUnitLookup[i];
        if (currentUnit->pCharacterData->number == BombCharIDLink){ //if this enemy is a bomb
            if (currentUnit->xPos == bombX && currentUnit->yPos == bombY){ //if this enemy is at the right location
                if (currentUnit->state & US_UNAVAILABLE){ //if they are dead, don't do the thing

                }
                else{
                    SetEventId(0x4);
                    isBombThere = true;
                    break;
                }
            }
        }
    }
    if (isBombThere == true){
        return;
    }
    UnsetEventId(0x4); //otherwise, we unset it just to be safe
}

void PlayBombMusicIfEitherBombReadyASMC(){
    int bomb1X = gEventSlot[0x1];
    int bomb1Y = gEventSlot[0x2];
    int bomb2X = gEventSlot[0x3];
    int bomb2Y = gEventSlot[0x4];
    Unit* currentUnit = 0;
    bool isBombThere = false;
    for (int i = FACTION_RED; i < FACTION_PURPLE; i++){
        currentUnit = gUnitLookup[i];
        if (currentUnit->pCharacterData->number == BombCharIDLink){
            if (currentUnit->xPos == bomb1X && currentUnit->yPos == bomb1Y){ //checks the first location first
                if (currentUnit->state & US_UNAVAILABLE){ //if they are dead, don't do the thing

                }
                else{
                    SetEventId(0x4);
                    isBombThere = true;
                    break;
                }
            }
            if (currentUnit->xPos == bomb2X && currentUnit->yPos == bomb2Y){ //then checks the second location
                if (currentUnit->state & US_UNAVAILABLE){ //if they are dead, don't do the thing

                }
                else{
                    SetEventId(0x4);
                    isBombThere = true;
                    break;
                }
            }
        }
    }
    if (isBombThere == true){
        return;
    }
    UnsetEventId(0x4); //otherwise, we unset it just to be safe
}

void DoesAnyUnitHaveForestFriendASMC(){ //here, we will loop through our entire group of units and see if any have Forest Friend
    Unit* someUnit;
    gEventSlot[0xC] = 0; //set sC to False in advance
    for (int i = 0; i <= 60; i++){
        someUnit = &gUnitArrayBlue[i]; //current unit in the party at this array index
        if (gSkillTester(someUnit, ForestFriendIDLink))
        {
            gEventSlot[0xC] = 1; // return True if someone is a friend of the forest, then break
            break;
        }
    }
}

void CheckIfTileChangeTriggered(){
    if (AreMapChangeTriggered(gEventSlot[0x1])){
        gEventSlot[0xC] = 1;
        return;
    }
    gEventSlot[0xC] = 0;
}

void SetDifficultyASMC(){
    int result = gEventSlot[0x1];
    if (result == 1) // normal mode
    {
        if (IsDifficultMode()) //if we're on hard, unset flag
        {
            gPlaySt.chapterStateBits ^= PLAY_FLAG_HARD; 
        }
        else if (TUTORIAL_MODE()) //if we're on easy mode, do some stuff
        {   
            gPlaySt.config.controller = 1; //unsets easy mode
        }
        else // normal mode, so do nothing
        {

        }
    }
    else if (result == 2) // hard mode
    {
        if (IsDifficultMode()) //if we're already on hard, don't do anything
        {

        }
        else if (TUTORIAL_MODE()) //if we're on easy mode, do some stuff
        {
            gPlaySt.config.controller = 1;
            gPlaySt.chapterStateBits |= PLAY_FLAG_HARD; //makes sure hard flag's set
        }
        else //all we have to do for normal
        {
            gPlaySt.chapterStateBits |= PLAY_FLAG_HARD; 
        }
    }
    else //can only be 0, so easy mode
    {
        if (IsDifficultMode()) //if we're on hard, unset flag
        {
            gPlaySt.chapterStateBits ^= PLAY_FLAG_HARD; //this unsets the hard flag, i think.
            gPlaySt.config.controller = 0; //this sets easy mode
            //gPlaySt.chapterStateBits |= PLAY_FLAG_TUTORIAL;
        }
        else if (TUTORIAL_MODE()) //if we're on easy mode, do nothing
        {   

        }
        else // normal mode, so do nothing
        {
            gPlaySt.config.controller = 0;
            //gPlaySt.chapterStateBits |= PLAY_FLAG_TUTORIAL;
        }
    }
}

void UnsetBattleUnitDebuffBits(struct Unit* unit)
{
    u32* unitDebuffs = GetUnitDebuffEntry(unit);
    UnsetBit(unitDebuffs, BreakBitOffset_Link);
    UnsetBit(unitDebuffs, BreakInBattleBitOffset_Link);
    UnsetBit(unitDebuffs, AstralBlessingSklOffset_Link);
    UnsetBit(unitDebuffs, AstralBlessingSpdOffset_Link);
    UnsetBit(unitDebuffs, AstralBlessingLckOffset_Link);
    UnsetBit(unitDebuffs, LiquidCourageBitOffset_Link);
    ClearAllChapterKillCounters(); //hardworking's thing
    UnsetBit(unitDebuffs, FortuneStaffBitOffset_Link);
    UnsetBit(unitDebuffs, MiraclemakerBitOffset_Link);
    UnsetBit(unitDebuffs, NailedDownBitOffset_Link);
    UnsetBit(unitDebuffs, DelegationBitOffset_Link);
    UnsetBit(unitDebuffs, SwiftBitOffset_Link);
    UnsetBit(unitDebuffs, PollenateBitOffset_Link);
    UnsetBit(unitDebuffs, NoMoveBitOffset_Link);
    UnsetBit(unitDebuffs, RushStaffBitOffset_Link);
    UnsetBit(unitDebuffs, DoubleFalconBitOffset_Link);
    UnsetBit(unitDebuffs, ProtectStaffBitOffset_Link);
    UnsetBit(unitDebuffs, ConcentrateStaffBitOffset_Link);
}

void UnsetAllBattleUnitDebuffBitsOnPrepScreenASMC()
{
	int unitID = 0;
	int maxCount = FACTION_PURPLE; //we want to loop through blue/red/green and clear all these bits on preps
	
	
	while (unitID < maxCount) {
		//get the unit unitID
		struct Unit* curUnit = GetUnit(unitID);
		
		//clear Fortune bit
		UnsetBattleUnitDebuffBits(curUnit);
		
		unitID++;
	}	
}

