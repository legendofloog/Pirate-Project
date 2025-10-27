#include "ForceDeploymentRework.h"

// just replicating what I need, which is that locked/unlocked deployment exists via checking whether the deploy X and Y are 0xFF or not: if not, then deploy the target in the actual slot that I want

bool IsCharacterForceDeployed_(u8 pid)
{
    int i = 0;

    bool isTrue = false;

    while (NewForceDeploymentList[i].unitId != 0) // the terminator will have a unit id of 0, so we loop until we hit 0, then we end
    {
        if ((NewForceDeploymentList[i].route) != 0xFF && (NewForceDeploymentList[i].route != gChapterData.mode)) //if we've got the wrong mode
        {
            i++;
            continue;
        }   
        if ((NewForceDeploymentList[i].chapter) != 0xFF && (NewForceDeploymentList[i].chapter != gChapterData.chapterIndex))
        {
            i++;
            continue;
        }
        if (NewForceDeploymentList[i].unitId != pid)
        {
            i++;
            continue;
        }
        
        isTrue = true; //the char is force deployed; break here
        break;
    }

    return isTrue;
}

bool CanCharacterBePrepMoved(int unitId) //used to determine whether i can move a unit around; where the toggle deployment swapping fun begins
{
    if (unitId == 0x100) //why would I have a unitid this high? keep it anyway i guess
    {
        return false;
    }
    
    if (IsCharacterForceDeployed(unitId))
    {
        if (IsCharacterLocked(unitId)) //if the character has a particular position in this chapter, then they cannot be prep moved
        {
            return false;
        }
    }   
    return true;
}

bool IsCharacterLocked(int unitId)
{
    int i = 0;

    bool isTrue = false;
    while (NewForceDeploymentList[i].unitId != 0)
    {
        if ((NewForceDeploymentList[i].route) != 0xFF && (NewForceDeploymentList[i].route != gChapterData.mode)) //if we've got the wrong mode
        {
            i++;
            continue;
        }   
        if ((NewForceDeploymentList[i].chapter) != 0xFF && (NewForceDeploymentList[i].chapter != gChapterData.chapterIndex)) // if we've got the wrong chapter
        {
            i++;
            continue;
        }
        if (NewForceDeploymentList[i].unitId != unitId)
        {
            i++;
            continue;
        }
        //so we've got the right route, chapter, and unit id: just check for x and y pos
        if (NewForceDeploymentList[i].xPos == 0xFF && NewForceDeploymentList[i].yPos == 0xFF)
        {
            //do nothing
        }
        else
        {
            isTrue = true; //if it's not this specific pos, then they are locked into a particular position
        }

        break; //if we got this far, we found our entry, no more looping
    }

    return isTrue;
}

int CountForceDeployedUnits()
{
    int i, ret = 0;
    for (i = 1; i < 0x40; i++) {
        struct Unit *unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if ((US_DEAD | US_BIT16) & unit->state)
            continue;

        u8 charId = unit->pCharacterData->number;
        
        if (IsCharacterForceDeployed(charId))
        {
            if (IsCharacterLocked(charId))
            {
                ret++; //only increment if they're forced and locked?
            }
        }
    }
    return ret;
}


void SortPlayerUnitsForPrepScreen() //this part seems like the real beastie, not sure how to take it on
{
    int i, state1, state2;
    struct Unit *unit;
    int count = GetChapterAllyUnitCount();
    int _count = 0;

    InitUnitStack(gPrepUnitPool);

    //pushing to the top doesn't really work the way that i want this to work
    //instead of looping, let's just loop 

    int currentChapterId = gChapterData.chapterIndex; // the chapter that we care about!
    int chosenChapterId = 0xFF;

    i = 0;

    while (NewForceDeploymentList[i].unitId != 0)
    {
        if ((NewForceDeploymentList[i].route) != 0xFF && (NewForceDeploymentList[i].route != gChapterData.mode)) //if we've got the wrong mode
        {
            i++;
            continue;
        }   
        if ((NewForceDeploymentList[i].chapter) != 0xFF && (NewForceDeploymentList[i].chapter != gChapterData.chapterIndex))
        {
            i++;
            continue;
        }

        if (chosenChapterId != 0xFF) //if we've settled on a specific chapter
        {  
            if (NewForceDeploymentList[i].chapter != chosenChapterId) //if the chapter we're checking isn't the one we're currently working with (and it's not 0xFF, either),
            {
                break; //then we've moved onto a different chapter in the list, so skip the rest of this
            }
        }

        unit = GetUnitByCharId(NewForceDeploymentList[i].unitId); //if we've got the right unit, and the right chapter/mode, then let's just grab the unit

        if (!UNIT_IS_VALID(unit)) //if the unit is valid?
            continue;

        unit->state &= 0xFDFFFFFF; // what's the point of this?

        if (IsUnitInCurrentRoster(unit))
        {
            PushUnit(unit);
        }    

        if (NewForceDeploymentList[i].chapter != 0xFF && chosenChapterId == 0xFF) //if we're still on the initial id and the entry we're looking at doesn't just apply to any chapter
        {
            chosenChapterId = currentChapterId; //set what chapter we're working with to make sure that we don't go over and start double pushing when we don't mean to
        }
        
        i++; //how did i forget to increment...
    }     

    for (i = 1; i < 64; i++)
    {
        unit = GetUnit(i);
        if (!UNIT_IS_VALID(unit))
            continue;

        if (!IsUnitInCurrentRoster(unit) || !IsCharacterForceDeployed(unit->pCharacterData->number)) //if they're not in the roster or not force deployed, then they get pushed??
        {
            PushUnit(unit);
        }        
    }

    //ideally, this fixes the unit stack correctly and enables some more unit setups

    LoadPlayerUnitsFromUnitStack();

    for (i = 1; i < 64; i++)
    {
        unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (!IsUnitInCurrentRoster(unit))
            continue;

        if (SomeLeftoverFunctionThatReturns0(unit))
        {
            state1 = unit->state;
            state2 = 0x02000008;
        }
        else
        {
            if (count > _count)
            {
                unit->state &= 0xFFFFFFF7;
                _count++;
                continue;
            }

            state1 = unit->state;
            state2 = 0x08;
        }

        unit->state = state1 | state2;
    }
}