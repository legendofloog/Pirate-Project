#include "gbafe.h"

#include "ArmsScroll.h"

bool ArmsScrollUsability(struct Unit* unit, int item)
{
    //usability should just be: check the user's top weapon in their inventory
    //get its type and check the user's wexp for that rank
    //first, they have to have a rank in it already
    //next, they can't be capped out in it
    //false if both of these are not true
    //then, if you've looped through their whole inventory, then also return false because nothing exists

    if (ReturnArmsScrollWeaponType(unit) == 0xFF)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool ArmsScrollPrepUsability(struct Unit* unit, int item)
{
    return ArmsScrollUsability(unit, item); //no specific preps rules afaik
}

void ArmsScrollEffect(Proc* proc)
{
    struct Unit* unit = gActiveUnit;
    int itemSlot = gActionData.itemSlotIndex;

    gBattleTarget.statusOut = -1;
    
    DoArmsScrollEffect(unit, itemSlot);

    Popup_Create(ArmsScrollPopup, 60, 0, proc);

    return;
}

int ArmsScrollPrepEffect(struct Unit* unit, int itemSlot)
{
    DoArmsScrollEffect(unit, itemSlot);
    return ArmsScrollPopupTextWordLink;
}

void DoArmsScrollEffect(struct Unit* unit, int itemSlot)
{
    u8 weaponType = ReturnArmsScrollWeaponType(unit);

    unit->ranks[weaponType] += ArmsScrollBoostLink;

    if (unit->ranks[weaponType] > S_WEXP)
    {
        unit->ranks[weaponType] = S_WEXP;
    }

    unit->items[itemSlot] = 0; //lastly, we remove the arms scroll and all that before we wrap it up
    UnitRemoveInvalidItems(unit);
}

u8 ReturnArmsScrollWeaponType(struct Unit* unit)
{ 
    u16 item = 0;
    const struct ItemData* itemInfo;
    u8 weaponType = 0;
    u8 weaponEXP = 0;
    u8 foundWeaponType = 0xFF;

    for (int i = 0; i < ITEM_SLOT_COUNT; i++)
    {
        item = unit->items[i];
        if (item == 0) //if the item doesn't exist, then we've reached the bottom of the list and found nothing; end the loop
        {   
            break;
        }
        
        itemInfo = GetItemData(GetItemIndex(item)); //now let's check more about the actual item

        weaponType = itemInfo->weaponType;

        if (weaponType == ITYPE_ITEM) //if it's an item, then we don't care about it; keep looping
            continue;
        
        weaponEXP = unit->ranks[weaponType];
        
        if (weaponEXP == NO_WEXP) 
            continue;

        if (weaponEXP == S_WEXP) //maxxed out weapon exp or none at all, either case is not acceptable
            continue;

        foundWeaponType = weaponType; //we've found a real weapon type to increase
        break;
    }

    if (foundWeaponType == 0xFF)
    {
        return 0xFF;
    }
    else
    {
        return foundWeaponType; //functionally, a false return
    }
}