#include "gbafe.h"

#include "TrapReworkPirate.h"



struct Trap* GetTrapAt(int x, int y)
{
    struct Trap* it;

lop:
    for (it = GetTrap(0); it->type != TRAP_NONE; ++it)
    {
        // Check trap position
        if ((x == it->xPosition) && (y == it->yPosition))
            return it;

        // Check if we are on a wall, and there is a wall above
        // In which case the trap would be on the topmost wall tile
        if (gMapTerrain[y][x] == TERRAIN_WALL_1B) //instead of just checking that there's a wall one tile higher, let's do another for loop to see if it extends further (3, 4, etc.)
        { 
            for (int i = x-1; i >= 0; i--) //we're going to loop horizontally (to the left) to make horizontal walls merge correctly
            {
                if (gMapTerrain[y][i] == TERRAIN_WALL_1B) //if the tile one to the left is a wall, reduce
                {
                    x = i; //the x we care about is now the reduced value
                    goto lop; //begin the loop again
                }
                else
                {
                    break; // if we are not extending into a wall, then break out of this loop
                }
            }
            for (int j = y-1; j >= 0; j--) //and we'll also loop up in the same way
            {
                if (gMapTerrain[j][x] == TERRAIN_WALL_1B) // if the tile one up is a wall
                {
                    y = j; // same with the y
                    goto lop; //begin the loop again
                }
                else
                {
                    break;
                }
            }
        }
    }

    return NULL;
}


struct Trap* GetTypedTrapAt(int x, int y, int trapType)
{
    struct Trap* it;

    for (it = GetTrap(0); it->type != TRAP_NONE; ++it)
    {
        // Check trap position
        if ((it->xPosition == x) && (it->yPosition == y) && (it->type == trapType))
            return it;

        // Check if we want a wall
        if (trapType == TERRAIN_WALL_1B)
        {
            // Check if we on a wall, and there is a wall above
            // In which case the trap would be on the topmost wall tile
            if (gMapTerrain[y][x] == TERRAIN_WALL_1B)
            {
                if ((y > 0) && gMapTerrain[y-1][x] == TERRAIN_WALL_1B)
                {
                    return GetTrapAt(x, y-1);
                }
                if (x > 0 && gMapTerrain[y][x-1] == TERRAIN_WALL_1B) 
                {
                    return GetTrapAt(x-1, y); // do the same loop horizontally
                }
            }
        }
    }

    return NULL;
}



// REPLACES TRAP REWORK STUFF; Should work fine

void InitMapObstacles(void)
{
    int ix, iy;

    for (iy = gMapSize.y - 1; iy >= 0; --iy)
    {
        for (ix = gMapSize.x - 1; ix >= 0; --ix)
        {
            switch (gMapTerrain[iy][ix])
            {

            case TERRAIN_WALL_1B:
                if (gMapTerrain[iy-1][ix] == TERRAIN_WALL_1B)
                    continue; // walls are stacked, only the topmost tile gets a trap

                if (gMapTerrain[iy][ix-1] == TERRAIN_WALL_1B)
                    continue; //walls are stacked horizontally, only leftmost tile gets a trap

                AddTrap(ix, iy, TRAP_OBSTACLE, GetChapterDefinition(gChapterData.chapterIndex)->mapCrackedWallHeath);
                break;

            case TERRAIN_SNAG:
                AddTrap(ix, iy, TRAP_OBSTACLE, 20);
                break;

            } // switch (gMapTerrain[iy][ix])
        }
    }
}

void TryAddTrapsToTargetList() { 
    struct Trap* trap;

    for (trap = GetTrap(0); trap->type != TRAP_NONE; ++trap) {
        u8 trapXPos = trap->xPosition;
        u8 trapYPos = trap->yPosition;
        u8 trapInfo = trap->data[0]; // info is first piece; it's trap HP

        if (trap->type != TRAP_OBSTACLE)
        {
            continue;
        }

        if ((gMapTerrain[trapYPos][trapXPos] == TERRAIN_WALL_1B) && (gMapRangeSigned[trapYPos][trapXPos] != 0))
        {
            AddTarget(trapXPos, trapYPos, 0, trapInfo);
        }

        if ((gMapTerrain[trapYPos+1][trapXPos] == TERRAIN_WALL_1B) && (gMapRangeSigned[trapYPos+1][trapXPos] != 0))
        {
            AddTarget(trapXPos, trapYPos+1, 0, trapInfo);
        }

        if ((gMapTerrain[trapYPos][trapXPos+1] == TERRAIN_WALL_1B) && (gMapRangeSigned[trapYPos][trapXPos+1] != 0))
        {
            AddTarget(trapXPos+1, trapYPos, 0, trapInfo);
        }
        
        if ((gMapTerrain[trapYPos][trapXPos] == TERRAIN_SNAG) && (gMapRangeSigned[trapYPos][trapXPos] != 0))
        {
            AddTarget(trapXPos, trapYPos, 0, trapInfo); //idc about snags so keep this vanilla
        }
    }

    return;
}

void ComputeBattleObstacleStats(void) {
    gBattleActor.battleEffectiveHitRate = 100;
    gBattleActor.battleEffectiveCritRate = 0;

    gBattleTarget.battleSpeed = 0xFF;
    gBattleTarget.hpInitial = gBattleTarget.unit.curHP;

    gBattleTarget.battleDefense = gBattleActor.battleAttack - (gBattleActor.battleAttack / 2);
    //why am I doing this? class numbers are too high so my walls break too easily :(
    //

    gBattleTarget.wTriangleHitBonus = 0;
    gBattleTarget.wTriangleDmgBonus = 0;
}

//Override
int ExecTrap(Proc* proc, Unit* unit, int exec_type)
{
    struct ProcBmTrap * proc2;

    switch (GetTriggeredTrapType(unit)) {
        case TRAP_8:
            proc2 = ProcStartBlocking(0x859E5AC, proc);
            proc2->post_exec_type = exec_type;
            proc2->unit = unit;
            break;

        case TRAP_MINE:
            RemoveTrap(GetTypedTrapAt(unit->xPos, unit->yPos, TRAP_MINE));
            proc2 = Proc_StartBlocking(sProcScr_ExecTrapMine, proc);
            proc2->post_exec_type = exec_type;
            proc2->unit = unit;
            break;

        case TRAP_FIRE_THIEF:
            RemoveTrap(GetTrapAt(unit->xPos, unit->yPos));
            PlaySoundEffect(SONG_B1);
            NewPopup2_PlanA(proc, -1, GetStringFromIndex(0x20));    /* Disabled trap. */
            break;

        case TRAP_MINE_ASSASSIN:
            RemoveTrap(GetTrapAt(unit->xPos, unit->yPos));
            PlaySoundEffect(SONG_B1);
            NewPopup2_PlanA(proc, -1, GetStringFromIndex(0x21));    /* Recovered mine. */
            UnitAddItem(unit, MakeNewItem(ITEM_MINE));
            break;
    }

    return 0;
}