struct NewProcPrepItemUseBooster {
    PROC_HEADER;

    /* 2C */ int timer;
    /* 30 */ u8 status_pre[EKRLVUP_STAT_MAX];
    /* 38 */ u8 status_pst[EKRLVUP_STAT_MAX];
    /* 40 */ int xpos, ypos, width, height;
};

enum ekr_lvup_status_index {
    EKRLVUP_STAT_HP = 0,
    EKRLVUP_STAT_STR,
    EKRLVUP_STAT_MAG,
    EKRLVUP_STAT_DEX,
    EKRLVUP_STAT_SPD,

    EKRLVUP_STAT_DEF,
    EKRLVUP_STAT_RES,
    EKRLVUP_STAT_EVA,
    EKRLVUP_STAT_CON,
    EKRLVUP_STAT_MOV,
    EKRLVUP_STAT_MAX,

    EKRLVUP_STAT_CLASS = EKRLVUP_STAT_MAX,
    EKRLVUP_STAT_LV_MSG,
    EKRLVUP_STAT_LV_VAL,

    EKRLVUP_STAT_PNAME = EKRLVUP_STAT_MAX,
    EKRLVUP_STAT_LVPRE_MSG,
    EKRLVUP_STAT_LVPRE_VAL
};

struct ProcPrepItemUse {
    PROC_HEADER;

    /* 2C */ struct Unit *unit;
    /* 30 */ int slot;
    /* 34 */ int unk34;
    /* 38 */ int slot_rtext;
    /* 3C */ int pos_subbox;
    /* 40 */ int game_lock;
};

enum proc_label_prep_item_use {
    PROC_LABEL_PREPITEMUSE_INIT,
    PROC_LABEL_PREPITEMUSE_MAIN,
    PROC_LABEL_PREPITEMUSE_CONFIRM,
    PROC_LABEL_PREPITEMUSE_EXEC_ITEMEFFECT,
    PROC_LABEL_PREPITEMUSE_EXEC_JUNA,
    PROC_LABEL_PREPITEMUSE_EXEC_PROMO,
    PROC_LABEL_PREPITEMUSE_EXIT_FADE,
    PROC_LABEL_PREPITEMUSE_END
};

int GetUnitMovement(Unit* unit);
int GetUnitConstitution(Unit* unit);

#define SONG_5A 0x5A