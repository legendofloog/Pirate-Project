bool CanBattleUnitGainExp(BattleUnit* actor, BattleUnit* target);
bool CanBattleUnitGainLevels(BattleUnit* battleUnit);
int GetLevelDifference(BattleUnit* actor, BattleUnit* target);
int GetNPCStatIncrease(int growth);

#define BITPACKED __attribute__((aligned(4), packed))

#define CA_NO_EXP CA_NEGATE_LETHALITY

#define NO_WEXP 0
#define D_WEXP 1
#define C_WEXP 26
#define B_WEXP 76
#define A_WEXP 151
#define S_WEXP 251

#define NO_RANK 0
#define D_RANK 2
#define C_RANK 3
#define B_RANK 4
#define A_RANK 5
#define S_RANK 6


void SetUnitLeaderCharId(struct Unit* unit, int charId);

struct ChapterEventGroup
{
    /* 00 */ void *turnBasedEvents;
    /* 04 */ void *characterBasedEvents; // must be 32-Aligned?
    /* 08 */ void *locationBasedEvents;
    /* 0C */ void *miscBasedEvents;

    // select unit -> select destination -> move unit to destination
    /* 10 */ void *specialEventsWhenUnitSelected;
    /* 14 */ void *specialEventsWhenDestSelected;
    /* 18 */ void *specialEventsAfterUnitMoved;

    /* 1C */ void *tutorialEvents;

    /* 20 */ void *traps;
    /* 24 */ void *extraTrapsInHard;

    /* 28 */ void *playerUnitsInNormal;
    /* 2C */ void *playerUnitsInHard;

    /* 30 */ void *playerUnitsChoice1InEncounter;
    /* 34 */ void *playerUnitsChoice2InEncounter;
    /* 38 */ void *playerUnitsChoice3InEncounter;

    /* 3C */ void *enemyUnitsChoice1InEncounter;
    /* 40 */ void *enemyUnitsChoice2InEncounter;
    /* 44 */ void *enemyUnitsChoice3InEncounter;

    /* 48 */ void *beginningSceneEvents;
    /* 4C */ void *endingSceneEvents;
};

const struct ChapterEventGroup* GetChapterEventDataPointer(unsigned chIndex);

struct CharacterMagicData {
	/* 00 */ s8 baseMag;
	/* 01 */ s8 growthMag;
};

struct ClassMagicData {
	/* 00 */ s8 baseMag;
	/* 01 */ s8 growthMag;
	/* 02 */ s8 maxMag;
	/* 03 */ u8 promotionMag; 
};

extern const struct ClassMagicData MagClassTable[];
extern const struct CharacterMagicData MagCharTable[];

enum
{
    CHAPTER_FLAG_0          = (1 << 0),
    CHAPTER_FLAG_1          = (1 << 1),
    CHAPTER_FLAG_POSTGAME   = (1 << 2),
    CHAPTER_FLAG_3          = (1 << 3),
    CHAPTER_FLAG_PREPSCREEN = (1 << 4),
    CHAPTER_FLAG_5          = (1 << 5),
    CHAPTER_FLAG_DIFFICULT  = (1 << 6),
    CHAPTER_FLAG_7          = (1 << 7)
};

s8 ActionSteal(Proc* proc);
void BattleApplyStealAction(struct Proc* proc, int item);
void BattleApplyStealActionExpGains(int item);
void ApplyStealFatigueValue(int item);
int GetStealExpValue(int item);
void UpdateActorFromBattle(void);
void BeginMapAnimForSteal(void);
void BattleInitItemEffect(struct Unit* actor, int itemSlot);
void InitBattleUnit(struct BattleUnit* bUnit, struct Unit* unit);
void CheckBattleUnitLevelUp(struct BattleUnit* bu);

static const struct ProcInstruction sProcScr_BattleAnimSimpleLock[] = {
    PROC_SLEEP(1),
    PROC_CALL_ROUTINE(UpdateActorFromBattle),
    PROC_END
};

int GenUnitDefinitionFinalPosition(const struct UnitDefinition* uDef, s8* xOut, s8* yOut, s8 findNearest);
void SetUnitAiFromDefinition(struct Unit* unit, const struct UnitDefinition* uDef);

bool UnitAddStolenItem(struct Unit* unit, int item);

int ReturnNumberOfHubChaptersVisited();

int GetBattleMapType(void);

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

s8 IsUnitEnemyWithActiveUnit(struct Unit* unit);

s8 AreUnitsAllied(int left, int right);

bool LuaIsInHub(Unit* unit);

u8 IsDifficultMode(void);

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

void AutolevelUnit(struct Unit* unit, int levelCount);
void AutolevelClass(struct Unit* unit, int levelCount);
void FloorStats(struct Unit* unit);

int GetOffensiveStaffAccuracy(struct Unit* actor, struct Unit* target);
extern s8 prMagGetter(Unit* unit);

extern u8 WildAxeIDLink;
extern u8 SnapshotIDLink;

enum
{
    // Menu availability identifiers

    MENU_ENABLED  = 1,
    MENU_DISABLED = 2,
    MENU_NOTSHOWN = 3,
};

void MakeTargetListForSteal(struct Unit* unit);
int AddItemToConvoy(int item);