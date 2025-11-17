#include "gbafe.h"

#include "Tonic_New.h"

bool TonicUsabilityFunc(struct Unit* unit, int item)
{
    int usedTonicBit = GetUsedTonicBit(unit, item);

    if (usedTonicBit == 0xFFFF)
    {
        if (CheckNumberOfTonics(unit) == 8) return false;

        return true; //return true if they aren't all set
    }
    else if (usedTonicBit == 0x0)
    {
        return false; //dunno how'd this happen, but return false if so
    }

    if (CheckBit(GetUnitDebuffEntry(unit), usedTonicBit)) return false; //if it's already set, then don't allow
    
    return true; //otherwise, go ahead
}

int GetUsedTonicBit(struct Unit* unit, int item)
{
    u8 itemDurability = item >> 8; //takes the item and then right shifts it, getting just the durability ?
    
    u32 tonicBit = 0x0;

    switch (itemDurability)
    {
        case HP_TONIC:
            tonicBit = HPTonicOffset_Link;
            break;
        case STR_TONIC:
            tonicBit = StrTonicOffset_Link;
            break;
        case MAG_TONIC:
            tonicBit = MagTonicOffset_Link;
            break;
        case DEX_TONIC:
            tonicBit = DexTonicOffset_Link;
            break;
        case SPD_TONIC:
            tonicBit = SpdTonicOffset_Link;
            break;
        case DEF_TONIC:
            tonicBit = DefTonicOffset_Link;
            break;
        case RES_TONIC:
            tonicBit = ResTonicOffset_Link;
            break;
        case EVA_TONIC:
            tonicBit = EvaTonicOffset_Link;
            break;
        case RAINBOW_TONIC:
            tonicBit = 0xFFFF; //special snowflake ass
        default:
            break;
    }

    return tonicBit;
    
}   

bool TonicPrepUsabilityFunc(struct Unit* unit, int item)
{
    return TonicUsabilityFunc(unit, item); //no specific preps rules afaik
}

void TonicEffectFunc(Proc* proc)
{
    struct Unit* unit = gActiveUnit;
    int itemSlot = gActionData.itemSlotIndex;

    gBattleTarget.statusOut = -1;
    
    DoTonicEffect(unit, itemSlot);

    Popup_Create(TonicPopup, 60, 0, proc);

    return;
}

int TonicPrepEffectFunc(struct Unit* unit, int itemSlot)
{
    DoTonicEffect(unit, itemSlot);
    return TonicUseBoxTextPointer;
}

int AddTonicBonus(struct Unit* unit, int statIndex)
{
    u8 tonicBit = GetTonicBitFromStat(statIndex);
    
    if (tonicBit == 0xFF) return 0; //how? this should never happen

    if (!CheckBit(GetUnitDebuffEntry(unit), tonicBit)) return 0;//if it's not set, then return nothing

    u8 tonicBoost = TonicStatBonusTable[statIndex]; //gets the associated boost

    if (unit->pCharacterData->number == 0x44) tonicBoost *= 3; //hard checking for the unit since checking for the skill is often puzzlingly bad

    return tonicBoost;
}

int GetTonicBitFromStat(int statIndex) //this is based off of stat getter indices
{
    u32 tonicBit = 0xFF;
    switch (statIndex)
    {
        case STAT_HP:
            tonicBit = HPTonicOffset_Link;
            break;
        case STAT_STRENGTH:
            tonicBit = StrTonicOffset_Link;
            break;
        case STAT_MAGIC:
            tonicBit = MagTonicOffset_Link;
            break;
        case STAT_SKILL:
            tonicBit = DexTonicOffset_Link;
            break;
        case STAT_SPEED:
            tonicBit = SpdTonicOffset_Link;
            break;
        case STAT_DEFENSE:
            tonicBit = DefTonicOffset_Link;
            break;
        case STAT_RESISTANCE:
            tonicBit = ResTonicOffset_Link;
            break;
        case STAT_LUCK:
            tonicBit = EvaTonicOffset_Link;
            break;
        default:
            break;
    }

    return tonicBit;
}

int CheckNumberOfTonics(struct Unit* unit)
{
    int count = 0;
    
    if (CheckBit(GetUnitDebuffEntry(unit), HPTonicOffset_Link)) count++;
    if (CheckBit(GetUnitDebuffEntry(unit), StrTonicOffset_Link)) count++;
    if (CheckBit(GetUnitDebuffEntry(unit), MagTonicOffset_Link)) count++;
    if (CheckBit(GetUnitDebuffEntry(unit), DexTonicOffset_Link)) count++;
    if (CheckBit(GetUnitDebuffEntry(unit), SpdTonicOffset_Link)) count++;
    if (CheckBit(GetUnitDebuffEntry(unit), DefTonicOffset_Link)) count++;
    if (CheckBit(GetUnitDebuffEntry(unit), ResTonicOffset_Link)) count++;
    if (CheckBit(GetUnitDebuffEntry(unit), EvaTonicOffset_Link)) count++;

    return count;
}

int ReturnHangoverPenalty(struct Unit* unit)
{

    if (CheckNumberOfTonics(unit) == 1) return 1; //minimum value

    return (CheckNumberOfTonics(unit) / 2);
}

void ClearTonicDebuffs() //don't think there are any inputs
{
    int i = 0;
    u8 chapterNum = gChapterData.chapterIndex;
    bool isExcludedChapter = false;
    while(TonicChapterExclusionTable[i] != 0xFF)
    {
        if (chapterNum == TonicChapterExclusionTable[i])
        {
            isExcludedChapter = true;
            break;
        }
    }

    if (isExcludedChapter) return;

    struct Unit* curUnit;
    u32* address;

    for (int i = 0; i < FACTION_PURPLE; i++) //should loop through blues, greens, reds, and unset all tonic bits
    {   
        curUnit = GetUnit(i);
        address = GetUnitDebuffEntry(curUnit);

        UnsetBit(address, HPTonicOffset_Link); //unsets all of these at the end of a chapter
        UnsetBit(address, StrTonicOffset_Link);
        UnsetBit(address, MagTonicOffset_Link);
        UnsetBit(address, DexTonicOffset_Link);
        UnsetBit(address, SpdTonicOffset_Link);
        UnsetBit(address, DefTonicOffset_Link);
        UnsetBit(address, ResTonicOffset_Link);
        UnsetBit(address, EvaTonicOffset_Link);
    }   

}

void DoTonicEffect(struct Unit* unit, int itemSlot)
{
    int tonicBit = GetUsedTonicBit(unit, unit->items[itemSlot]);
    u32* address = GetUnitDebuffEntry(unit);

    if (tonicBit != 0xFFFF) SetBit(address, tonicBit);

    int hpBonus = 0;
    if (tonicBit == 0xFFFF || tonicBit == HPTonicOffset_Link) //we need to increase their current HP here when this happens
    {
        hpBonus += TonicStatBonusTable[STAT_HP];
        if (unit->pCharacterData->number == 0x44) //apparently a skilltester check won't work here: seems like it doesn't work in prep screen item use because there's no battle units?
        {
            hpBonus += TonicStatBonusTable[STAT_HP] * 2; //do it twice more since the boost is triple: just hard checking for medwyn
        }
    } 

    if (tonicBit == 0xFFFF) //lastly, let's do a RainbowTonic mass set bit
    {
        SetBit(address, HPTonicOffset_Link);
        SetBit(address, StrTonicOffset_Link);
        SetBit(address, MagTonicOffset_Link);
        SetBit(address, DexTonicOffset_Link);
        SetBit(address, SpdTonicOffset_Link);
        SetBit(address, DefTonicOffset_Link);
        SetBit(address, ResTonicOffset_Link);
        SetBit(address, EvaTonicOffset_Link);
    }

    unit->curHP += hpBonus;

    unit->items[itemSlot] = 0; //lastly, we remove the tonic and wrap it up
    UnitRemoveInvalidItems(unit);
}