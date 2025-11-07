#include "gbafe.h"

#define BITPACKED __attribute__((aligned(4), packed))

u8 IsDifficultMode(void);

enum ChapterData_chapterStateBits {
    PLAY_FLAG_STATSCREENPAGE0 = (1 << 0),
    PLAY_FLAG_STATSCREENPAGE1 = (1 << 1),
    PLAY_FLAG_POSTGAME        = (1 << 2),
    PLAY_FLAG_TUTORIAL        = (1 << 3),
    PLAY_FLAG_PREPSCREEN      = (1 << 4),
    PLAY_FLAG_COMPLETE        = (1 << 5),
    PLAY_FLAG_HARD            = (1 << 6),
    PLAY_FLAG_7               = (1 << 7),

    PLAY_FLAG_STATSCREENPAGE_SHIFT = 0,
    PLAY_FLAG_STATSCREENPAGE_MASK = PLAY_FLAG_STATSCREENPAGE0 | PLAY_FLAG_STATSCREENPAGE1,
};

#define TUTORIAL_MODE() (!(gPlaySt.chapterStateBits & PLAY_FLAG_HARD) && gPlaySt.config.controller != 1)


struct PlaySt_30 {
    s32 total_gold;

    u32 unk_4_00 : 20;
    u32 unk_4_14 : 12;

    u32 unk_8_1: 8;
    u32 unk_8_2: 20; // Used by bmdifficulty (Valni/Lagdou)
    u32 unk_8_3: 4;

    u32 unk_C_00 : 6;
    u32 combatRank : 3;
    u32 expRank : 3;
    u32 unk_3D_04 : 3;
    u32 fundsRank : 3;
    u32 tacticsRank : 3;
    u32 survivalRank : 3;
    u32 unk_F_00 : 8;
} BITPACKED;

struct PlaySt_OptionBits {
    // option bits
    u32 unitColor:1; // 1
    u32 disableTerrainDisplay:1; // 1
    u32 unitDisplayType:2; // 2
    u32 autoCursor:1; // 1
    u32 textSpeed:2;
    u32 gameSpeed:1; // 1

    u32 disableBgm:1; // 1
    u32 disableSoundEffects:1; // 1
    u32 windowColor:2;
    u32 unk41_5:1; // 1
    u32 unk41_6:1; // unk
    u32 disableAutoEndTurns:1; // 1
    u32 noSubtitleHelp:1; // 1

    u32 disableGoalDisplay:1; // unk
    u32 animationType:2; // 2
    u32 battleForecastType:2; // 2
    u32 controller:1; // 1
    u32 rankDisplay:1; // unk

    u32 debugControlRed:2; // 2 (!)
    u32 debugControlGreen:2; // 2
    u32 unk43_4:5; // unk

    u8  unk44[0x48 - 0x44];
} BITPACKED;

struct PlaySt { // Chapter Data Struct
    /* 00 */ u32 time_saved;
    /* 04 */ u32 time_chapter_started;

    /* 08 */ u32 partyGoldAmount;
    /* 0C */ u8  gameSaveSlot;

    /* 0D */ u8  chapterVisionRange; // 0 means no fog
    /* 0E */ s8  chapterIndex;

    /* 0F */ u8  faction; // 0x00 = Player phase, 0x40 = NPC phase, 0x80 = Enemy phase (0xC0 = link arena 4th team?)

    /* 10 */ u16 chapterTurnNumber;

    /* 12 */ u8  xCursor, yCursor; // map cursor position

    /* 14 */ u8  chapterStateBits; // +0x04 = postgame, +0x10 = in prep screen, +0x40 = difficult mode, +0x80 = link arena?
    /* 15 */ u8  chapterWeatherId;
    /* 16 */ u16 chapterTotalSupportGain;

    /* 18 */ u8 playthroughIdentifier;
    /* 19 */ u8 unk19;
    /* 1A */ u8 lastUnitSortType;

    /* 1B */ u8  chapterModeIndex; // 1 for tutorial (ch0-8), 2 for Eirika route, 3 for Ephraim route

    // character identifiers indexed by weapon type.
    // has to do with allowing unusable weapons to be used
    /* 1C */ u8  unk1C[2];
    /* 1E */ u8 unk1E;
    /* 1F */ u8 unk1F;

    /* 20 */ char playerName[0x2B - 0x20]; // unused outside of link arena (was tactician name in FE7); Size unknown

    /* 2B */ u8 unk_2B_00 : 0x01;
             u8 unk_2B_01 : 0x07;

    u32 unk_2C_1 : 1;
    u32 unk_2C_01 : 3;
    u32 unk_2C_04 : 9;
    u32 unk_2C_0D : 10;
    u32 unk_2C_2 : 5;
    u32 unk_2C_3 : 4;

    struct PlaySt_30 unk_30;

    struct PlaySt_OptionBits config;

    u16 unk48;

    u8 unk4A_1 : 1;
    u8 save_menu_type : 3;
    u8 tutorial_exec_type : 4;
    u8 tutorial_counter;
} BITPACKED;

extern struct PlaySt gPlaySt; //! FE8U = (0x202BCF0)

extern void UnsetBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(struct Unit* unit);

extern int BreakBitOffset_Link;
extern int BreakInBattleBitOffset_Link;
extern int AstralBlessingSklOffset_Link;
extern int AstralBlessingSpdOffset_Link;
extern int AstralBlessingLckOffset_Link;
extern int LiquidCourageBitOffset_Link;
extern int StudiousBlowBitOffset_Link;
extern int HardworkingBitOffset_Link;
extern int FortuneStaffBitOffset_Link;
extern int MiraclemakerBitOffset_Link;
extern int NailedDownBitOffset_Link;
extern int DelegationBitOffset_Link;
extern int SwiftBitOffset_Link;
extern int PollenateBitOffset_Link;
extern int NoMoveBitOffset_Link;
extern int RushStaffBitOffset_Link;
extern int DoubleFalconBitOffset_Link;
extern int ProtectStaffBitOffset_Link;
extern int ConcentrateStaffBitOffset_Link;

extern void ClearAllChapterKillCounters();