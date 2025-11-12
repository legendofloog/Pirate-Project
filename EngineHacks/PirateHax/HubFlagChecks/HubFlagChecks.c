#include "gbafe.h"

#include "HubFlagChecks.h"

void CheckIfHubVisitedASMC(){
    bool visitedHub = CheckEventId(0x83) | CheckEventId(0x8B) | CheckEventId(0x8c) | CheckEventId(0x8d) | CheckEventId(0x8e) | CheckEventId(0x8f);
    if (visitedHub != 0){
        gEventSlot[0xC] = 1; //ORs all flags, if one is set then visitedHub = 1 and we have visited
    }
    else{
        gEventSlot[0xC] = 0; //if not, return 0
    }
}

void CheckIfHubChaptersCompletedASMC(){
    bool completedChapters = CheckEventId(0x83) & CheckEventId(0x8B) & CheckEventId(0x8c) & CheckEventId(0x8d) & CheckEventId(0x8e) & CheckEventId(0x8f);
    if (completedChapters != 0){
        gEventSlot[0xC] = 1; //ANDs all flags, if all are set then completedChapters = 1 
    }
    else{
        gEventSlot[0xC] = 0; //if not, return 0
    }
}

void SetHubChapterFlagASMC(){
    int flagId = gEventSlot[0x1];
    if (flagId == 1){
        SetEventId(0x83);
    }
    else if (flagId == 2){
        SetEventId(0x8B);
    }
    else if (flagId == 3){
        SetEventId(0x8c);
    }
    else if (flagId == 4){
        SetEventId(0x8d);
    }
    else if (flagId == 5){
        SetEventId(0x8e);
    }
    else if (flagId == 6){
        SetEventId(0x8f);
    }
    else{

    }
    UnsetEventId(0xA0); //unset this bc we are no longer in a hub
}

void UnsetAllHubChapterFlagsASMC(){
    UnsetEventId(0x83);
    UnsetEventId(0x8b);
    UnsetEventId(0x8c);
    UnsetEventId(0x8d);
    UnsetEventId(0x8e);
    UnsetEventId(0x8f);
}


bool LuaIsInHub(Unit* unit){
    if (CheckEventId(0xA0)) //if we're in a hub and not in the prep screen
    {
        return true; //give 15 move
    }
    return false;
}

int ReturnNumberOfHubChaptersVisited(){
    int hubChapters = (CheckEventId(0x83) + CheckEventId(0x8B) + CheckEventId(0x8c) + CheckEventId(0x8d) + CheckEventId(0x8e) + CheckEventId(0x8f) - 1);
    
    if (LuaIsInHub(gActiveUnit)) //unit shouldn't matter: point here is to show what units (if they do show up in your hub) WOULD look like if you used them in the next chapter
    {
        hubChapters++;
    }
    
    if (hubChapters <= 0){
        return 0;
    }
    else{
        return hubChapters;
    }
}

void CheckIfOneHubAChapterCompletedASMC(){
    int numOfChapters = ReturnNumberOfHubChaptersVisited();
    if (numOfChapters >= 1){
        gEventSlot[0xC] = 1;
    }
    else{
        gEventSlot[0xC] = 0;
    }
}

void CheckHowManyHubChaptersCompletedASMC(){
    gEventSlot[0xC] = ReturnNumberOfHubChaptersVisited();
}

void CalculateYodsenPriceASMC(){
    int basePrice = 3000;
    int discount = ReturnNumberOfHubChaptersVisited() * 500;
    gEventSlot[0xC] = basePrice - discount;
}

void GetTextFromMemorySlot1ASMC(){
    int text = gEventSlot[0x1];
    SetTalkNumber(text); //sets the talk number to be the value in s1
}

// also threw in redrawing Chapter Status for Hub A purposes
const char * GetChapterTitleName(unsigned chIndex)
{
    if (chIndex != 0x7F)
    {
        if (LuaIsInHub(gActiveUnit)) //unit doesn't matter, this is checking if we're in hub mode but in the chapter 2 section
        {
            return GetStringFromIndex((int)(&GetChapterDefinition(chIndex)->chapTitleTextIdInHectorStory)); //if we're in a hub, then we use a separate title
        }
        return GetStringFromIndex((int)(&GetChapterDefinition(chIndex)->chapTitleTextId));
    }     

    return GetStringFromIndex((int)(&GetChapterDefinition(chIndex)->chapTitleTextId)); //no skirmishes so i think i'm fine
}

int GetChapterTitleWM(struct ChapterState* chapterData)
{
    if (chapterData == 0) {
        return 0x54; // No Data
    }

    if (LuaIsInHub(gActiveUnit) && chapterData->chapterIndex == 8) //unit doesn't matter, this is checking if we're in hub mode but in the chapter 2 section
    {
       return GetChapterDefinition(1)->chapTitleId; //have to hard code in this case for some reason, returning a different chaptertitleid slot just results in a hang
    }
    return GetChapterDefinition(chapterData->chapterIndex)->chapTitleId;
}
