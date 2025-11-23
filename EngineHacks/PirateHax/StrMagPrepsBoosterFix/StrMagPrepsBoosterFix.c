#include "gbafe.h"

#include "StrMagPrepsBoosterFix.h"

void PrepItemUseBooster_OnInit(struct NewProcPrepItemUseBooster * proc)
{
    int i, item, msg;
    struct ProcPrepItemUse *parent = proc->proc_parent;

    StartManimLevelUpStatGainLabels(0x1C0, 3, 0, proc);

    proc->status_pre[EKRLVUP_STAT_HP] = GetUnitCurrentHp(parent->unit);
    proc->status_pre[EKRLVUP_STAT_STR] = GetUnitPower(parent->unit);
    proc->status_pre[EKRLVUP_STAT_MAG] = GetUnitMagic(parent->unit);
    proc->status_pre[EKRLVUP_STAT_DEX] = GetUnitSkill(parent->unit);
    proc->status_pre[EKRLVUP_STAT_SPD] = GetUnitSpeed(parent->unit); //new column

    proc->status_pre[EKRLVUP_STAT_CON] = GetUnitConstitution(parent->unit);
    proc->status_pre[EKRLVUP_STAT_DEF] = GetUnitDefense(parent->unit);
    proc->status_pre[EKRLVUP_STAT_RES] = GetUnitResistance(parent->unit);
    proc->status_pre[EKRLVUP_STAT_EVA] = GetUnitLuck(parent->unit);
    proc->status_pre[EKRLVUP_STAT_MOV] = GetUnitMovement(parent->unit);

    item = parent->unit->items[parent->slot];

    msg = ApplyStatBoostItem(parent->unit, parent->slot); //don't think this needs to be reworked
    
    DrawPrepScreenItemUseStatBars(parent->unit, 0);
    DrawPrepScreenItemUseStatValues(parent->unit);

    proc->status_post[EKRLVUP_STAT_HP] = GetUnitCurrentHp(parent->unit);
    proc->status_post[EKRLVUP_STAT_STR] = GetUnitPower(parent->unit);
    proc->status_post[EKRLVUP_STAT_MAG] = GetUnitMagic(parent->unit);
    proc->status_post[EKRLVUP_STAT_DEX] = GetUnitSkill(parent->unit);
    proc->status_post[EKRLVUP_STAT_SPD] = GetUnitSpeed(parent->unit); //new column

    proc->status_post[EKRLVUP_STAT_CON] = GetUnitConstitution(parent->unit);
    proc->status_post[EKRLVUP_STAT_DEF] = GetUnitDefense(parent->unit);
    proc->status_post[EKRLVUP_STAT_RES] = GetUnitResistance(parent->unit);
    proc->status_post[EKRLVUP_STAT_EVA] = GetUnitLuck(parent->unit);
    proc->status_post[EKRLVUP_STAT_MOV] = GetUnitMovement(parent->unit);

    PrepItemUseBooster_OnDraw(proc, 0xE, 0xE, msg, item);

    for (i = 0; i < EKRLVUP_STAT_MAX; i++) {
        if (proc->status_pre[i] == proc->status_pst[i])
            continue;

        StartPrepItemBoostStatGainLabelAnim(
            (i >> 2) * 56 + 0xB0,
            (i  & 3) * 16 + 0x2A,
            proc->status_pst[i] - proc->status_pre[i]
        );
    }

    proc->timer = 0x78;
    PlaySfx(SONG_5A);
}

void DrawPrepScreenItemUseStatBars(struct Unit* unit, int mask)
{
    s32 i;
    int stat_pack[8];
    UnpackUiBarPalette(2);

    stat_pack[0] = GetUnitCurrentHp(unit) * 24 / UNIT_MHP_MAX(unit);
    stat_pack[1] = GetUnitPower(unit) * 24 / UNIT_POW_MAX(unit);
    stat_pack[2] = GetUnitSkill(unit) * 24 / UNIT_SKL_MAX(unit);
    stat_pack[3] = GetUnitSpeed(unit) * 24 / UNIT_SPD_MAX(unit);
    stat_pack[4] = GetUnitLuck(unit) * 24 / UNIT_LCK_MAX(unit);
	stat_pack[5] = GetUnitDefense(unit) * 24 / UNIT_DEF_MAX(unit);
	stat_pack[6] = GetUnitResistance(unit) * 24 / UNIT_RES_MAX(unit);
	stat_pack[7] = UNIT_CON(unit) * 24 / UNIT_CON_MAX(unit);

    for (i = 0; i < 8; i++) {
        u32 var = 0x100 * i + 0x7000;

        if ((mask >> i) & 1) {
            u32 x = var << 15;
            if (x) { ++x; --x; }
            DrawStatBarGfx(
                x >> 20,
                4,
                TILEMAP_LOCATED(gBG0TilemapBuffer, (i >> 2) * 7 + 0x12, (i & 3) * 2 + 4),
                0x3000, 0x18, stat_pack[i], 0);
        } else {
            u32 x = var << 15;
            if (x) { ++x; --x; }
            DrawStatBarGfx(
                x >> 20,
                4,
                TILEMAP_LOCATED(gBG0TilemapBuffer, (i >> 2) * 7 + 0x12, (i & 3) * 2 + 4),
                0x2000, 0x18, stat_pack[i], 0);
        }
    }

    BG_EnableSyncByMask(1);
}

void DrawPrepScreenItemUseStatValues(struct Unit* unit)
{
    // HP
    PutNumberOrBlank( 
        TILEMAP_LOCATED(gBG2TilemapBuffer, 20, 3),
        (GetUnitCurrentHp(unit) == UNIT_MHP_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        GetUnitCurrentHp(unit)
    );

    // POW
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 20, 5),
        (GetUnitPower(unit) == UNIT_POW_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        GetUnitPower(unit)
    );

    // SKL
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 20, 7),
        (GetUnitSkill(unit) == UNIT_SKL_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        GetUnitSkill(unit)
    );

    // SPD
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 20, 9),
        (GetUnitSpeed(unit) == UNIT_SPD_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        GetUnitSpeed(unit)
    );

    // LCK
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 27, 3),
        (GetUnitLuck(unit) == UNIT_LCK_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        GetUnitLuck(unit)
    );

    // DEF
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 27, 5),
        (GetUnitDefense(unit) == UNIT_DEF_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        GetUnitDefense(unit)
    );

    // RES
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 27, 7),
        (GetUnitResistance(unit) == UNIT_RES_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        GetUnitResistance(unit)
    );

    // CON
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 27, 9),
        (UNIT_CON(unit) == UNIT_CON_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        UNIT_CON(unit)
    );

    TileMap_FillRect(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 19, 1),
        1, 1, 0
    );

    // LV
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 20, 1),
        TEXT_COLOR_SYSTEM_BLUE,
        unit->level
    );

    BG_EnableSyncByMask(BG2_SYNC_BIT);
}

void StartManimLevelUpStatGainLabels(int chr, int pal, int sprite_layer, ProcPtr parent)
{
    struct ManimLevelUpStatGainLabelProc * proc_a;
    struct ManimLevelUpLabelColorProc * proc_b;

    proc_a = Proc_Start(ProcScr_ManimLevelUpStatGainLabel, parent);

    proc_a->chr = chr;
    proc_a->pal = pal;
    proc_a->sprite_layer = sprite_layer;

    Decompress(Img_ManimLevelUpStatGain, OBJ_VRAM0 + (OAM2_CHR(chr) << 5));
    ApplyPalette(Pal_ManimLevelUp, 0x10 + pal);
    ApplyPalette(Pal_ManimLevelUp, 0x10 + pal + 1);

    proc_b = Proc_Start(ProcScr_ManimLevelUpLabelColor, proc_a);
    proc_b->pal = pal;
}


void StartPrepItemBoostStatGainLabelAnim(int x, int y, int stat_gain)
{
    int chr_common, chr_this_stat, stat_abs, stat_loss;
    struct ManimLevelUpStatGainLabelProc * proc;
    u8 *digits_chr = Img_ManimLevelUpStatGainDigits;

    stat_abs = ABS(stat_gain);
    proc = Proc_Find(ProcScr_ManimLevelUpStatGainLabel);
    chr_common = proc->chr;

    if (stat_gain >= 0) {
        chr_this_stat = chr_common + 2 * (stat_abs - 1);
        stat_loss = 5;
    } else {
        chr_this_stat = chr_common +16;
        stat_loss = 6;
    }

    APProc_Create(SpriteAnim_ManimStatGain,
        x, y,
        chr_common + OAM2_PAL(proc->pal) + OAM2_LAYER(proc->sprite_layer),
        stat_loss, 2);

    APProc_Create(SpriteAnim_ManimStatGain,
        x - 3, y,
        chr_this_stat + OAM2_PAL(proc->pal) + OAM2_LAYER(proc->sprite_layer),
        3, 2);


    APProc_Create(SpriteAnim_ManimStatGain,
        x - 18, y - 4,
        chr_common + OAM2_PAL(proc->pal) + OAM2_LAYER(proc->sprite_layer),
        0, 2);

    VramCopy(digits_chr + (OAM2_CHR(stat_abs) << 5),
        OBJ_VRAM0 + (OAM2_CHR(chr_this_stat + 0x2D) << 5), CHR_SIZE);

    VramCopy(digits_chr + (OAM2_CHR(stat_abs + 0x20) << 5),
            OBJ_VRAM0 + (OAM2_CHR(chr_this_stat + 0x4D) << 5), CHR_SIZE);
}