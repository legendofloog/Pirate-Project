
void PutObj8A18582_WithConfigs(int xOam1, int yOam0, int a3, u16 oam2);
void PutObj8A18590_WithConfigs(int xOam1, int yOam0, int a3, u16 oam2);
void PutChapterNumber(int xOam1, int yOam0, int a3, u16 oam2);
void PutHubChapterNumber(int xOam1, int yOam0, int a3, int oam2);

extern const struct ObjData *objs_8A18614[];
extern const struct ObjData obj_8A18590[];
extern const struct ObjData obj_8A18582[];
extern const struct ObjData obj_8A185AC[];
typedef void* ProcPtr;

struct ProcPrepSpecialChar {
     PROC_HEADER;
    /* 29 */ u8 _pad29_[0x2A - 0x29];
    /* 2A */ u8 unk2A;
    /* 2B */ u8 unk2B;
    /* 29 */ u8 _pad2C_[0x2F - 0x2C];
    /* 2F */ u8 unk2F;
    /* 30 */ u8 unk30;
    /* 31 */ u8 unk31;
    /* 32 */ u8 unk32;
    /* 33 */ u8 blink_Start;
    /* 34 */ u8 blink_B;
    /* 35 */ u8 unk35;
    /* 36 */ u16 unk36;
    /* 38 */ ProcPtr apProc;
};

void ProcPrepSpChar_OnInit(struct ProcPrepSpecialChar *proc);

#define STRUCT_PAD(from, to) unsigned char _pad_ ## from[(to) - (from)]

struct ChapterStatusProc
{
    /* 00 */ PROC_HEADER;

    /* 29 */ u8 restoreStateOnExit;
    /* 2A */ u8 focusUnitOnExit;
    /* 2B */ u8 timesCompleted;
    /* 2C */ u8 numObjectiveTextLines;
    /* 2D */ u8 unk_2d; // initialized to 0 and unreferenced
    /* 2E */ u8 unitIndex;
    /* 2F */ u8 unitIndexPrev;
    /* 30 */ u8 numAllyUnits;
    /* 31 */ u8 numEnemyUnits;

    /* 34 */ struct Unit * units[2];

    /* 3C */ u16 unk_3c; // initialized to 0 and unreferenced
    /* 3E */ u8 helpTextActive;
    /* 3F */ u8 unk_3f;
    /* 40 */ u8 timer;

    /* 41 */ STRUCT_PAD(0x41, 0x64);

    /* 64 */ u16 unk_64; // initialized to 0 and unreferenced
};


extern u8 PrepChapterDisplaySheet[];
extern u16 PrepChapterDisplaySheetPal[];
#define OBJ_VRAM0      (void *)(VRAM + 0x10000)