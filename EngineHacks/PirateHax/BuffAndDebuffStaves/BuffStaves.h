#include "gbafe.h"

// decompnathan
#define SECTION(name) __attribute__((section(name)))
#define CONST_DATA SECTION(".data")

typedef void* ProcPtr;
typedef void(*ProcFunc)(ProcPtr proc);

extern struct UnitInfoWindowProc* NewUnitInfoWindow(ProcPtr parent);

extern int GetUnitInfoWindowX(struct Unit* unit, int width);
extern struct UnitInfoWindowProc* UnitInfoWindow_DrawBase(struct UnitInfoWindowProc* proc, struct Unit* unit, int x, int y, int width, int lines);


extern u16 HealArrowIDLink;

extern Proc* StartBottomHelpText(struct TargetSelectionProc*, const char*); //! FE8U = 0x8035709


enum { LINES_MAX = 5 };

struct Text {
    u16 chr_position;
    u8 x;
    u8 colorId;
    u8 tile_width;
    s8 db_enabled;
    u8 db_id;
    u8 is_printing;
};

struct UnitInfoWindowProc {
    /* 00 */ PROC_HEADER;

    /* 2C */ struct Unit* unit;

    /* 30 */ struct TextHandle nameText;
    /* 38 */ struct TextHandle lines[LINES_MAX];

    /* 60 */ u8 x;
    /* 61 */ u8 y;
    /* 62 */ u8 xUnitSprite;
    /* 63 */ u8 xNameText;
};

//stuff defined in the reference file but not the headers
extern void BattleInitItemEffect(Unit* unit, u8 itemIndex);
extern void BattleInitItemEffectTarget(Unit* unit);
extern void BattleApplyItemEffect(Proc* proc);
extern void BeginBattleAnimations();

extern Unit* gUnitSubject;

//debuff functions
extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(Unit* unit);
extern void Item_TURange(struct Unit* unit, void* func, int item);
