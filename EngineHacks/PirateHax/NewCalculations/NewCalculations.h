bool CanBattleUnitGainExp(BattleUnit* actor, BattleUnit* target);
bool CanBattleUnitGainLevels(BattleUnit* battleUnit);
int GetLevelDifference(BattleUnit* actor, BattleUnit* target);
int GetNPCStatIncrease(int growth);

#define CA_NO_EXP CA_NEGATE_LETHALITY

#define NO_WEXP 0
#define D_WEXP 51
#define C_WEXP 76
#define B_WEXP 116
#define A_WEXP 176
#define S_WEXP 251

#define NO_RANK 0
#define D_RANK 2
#define C_RANK 3
#define B_RANK 4
#define A_RANK 5
#define S_RANK 6



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