#include "gbafe.h"

#include "CHAI_Pirate.h"

u8 Event39_CHAI(struct EventEngineProc * proc)
{
    u8 subcmd = EVT_SUB_CMD(proc->pEventCurrent);

    u8 ai1 = gEventSlot[1];
    u8 ai2 = (gEventSlot[1] >> 8);
    u8 ai3 = (gEventSlot[1] >> 16); //the second two AIs
    u8 ai4 = (gEventSlot[1] >> 24);

    struct Unit * unit;
    s8 x, y;
    u16 charId;

    switch (subcmd)
    {
        case EVTSUBCMD_CHAI:
            charId = EVT_CMD_ARGV(proc->pEventCurrent)[0];
            if (EVT_CMD_ARGV(proc->pEventCurrent)[0] < 0)
                charId = gEventSlot[2];

            ChangeAiForCharacter(charId, ai1, ai2, ai3, ai4);

            break;

        case EVTSUBCMD_CHAI_AT:
            x = EVT_CMD_ARGV(proc->pEventCurrent)[0];
            y = EVT_CMD_ARGV(proc->pEventCurrent)[0] >> 8;

            if (x < 0 || y < 0)
            {
                x = ((u16 *)(gEventSlot + 0xB))[0];
                y = ((u16 *)(gEventSlot + 0xB))[1];
            }

            if (gMapUnit[y][x] != 0)
                unit = GetUnit(gMapUnit[y][x]);
            else
                unit = NULL;

            ChangeUnitAi(unit, ai1, ai2, ai3, ai4);
            break;
    }
    return EVC_ADVANCE_CONTINUE;
}

void ChangeAiForCharacter(u8 charId, u8 ai1, u8 ai2, u8 ai3, u8 ai4)
{
    int i;

    for (i = FACTION_BLUE + 1; i < FACTION_PURPLE; i++) //this part is identical to vanilla but with AI3 and 4 as inputs, actual AI change happens in the bottom function
    {
        struct Unit * unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
        {
            continue;
        }

        if (unit->pCharacterData->number == charId)
        {
            ChangeUnitAi(unit, ai1, ai2, ai3, ai4);
        }
    }

    return;
}

void ChangeUnitAi(struct Unit * unit, u8 ai1, u8 ai2, u8 ai3, u8 ai4)
{
    if (unit->state & (US_HIDDEN | US_DEAD))
    {
        return;
    }

    if (ai1 != AI_A_INVALID)
    {
        unit->ai1 = ai1;
        unit->ai1data = 0;
    }

    if (ai2 != AI_B_INVALID)
    {
        unit->ai2 = ai2;
        unit->ai2data = 0;

        if (ai2 == AI_B_0C)
        {
            unit->aiFlag |= AI_UNIT_FLAG_3;
        }
    }

    //i'll leave this stuff since it's here already, but i shouldn't be dumb enough to give incorrect ai3s or 4s (lol)

    u16 combinedAI3_4 = ai4 << 8 | ai3; // i think?? left shift ai4 

    unit->ai3And4 = combinedAI3_4; //need to make sure this is set so that they don't accidentally move on a different phase

    if (ai4 == 20) // the boss AI4 setting where they don't move
    {
        unit->ai3And4 |= AI_UNIT_CONFIG_FLAG_STAY; //set the flag to make sure that they don't
    }


    return;
}