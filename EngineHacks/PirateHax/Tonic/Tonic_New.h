#define NO_WEXP 0
#define D_WEXP 1
#define C_WEXP 26
#define B_WEXP 76
#define A_WEXP 151
#define S_WEXP 251

void NewPopup2_PlanA(Proc* parent, int IconIndex, char *str);

extern u32 ConcentrateStaffBitOffset_Link;
extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(Unit* unit);
extern void Item_TURange(struct Unit* unit, void* func, int item);

#define STAT_HP         0x00
#define STAT_STRENGTH   0x01
#define STAT_SKILL      0x02
#define STAT_SPEED      0x03
#define STAT_DEFENSE    0x04
#define STAT_RESISTANCE 0x05
#define STAT_LUCK       0x06
#define STAT_MOV        0x07
#define STAT_CON        0x08
#define STAT_MAGIC      0x09

#define HP_TONIC        0x01
#define STR_TONIC       0x02
#define DEX_TONIC       0x03
#define SPD_TONIC       0x04
#define DEF_TONIC       0x05
#define RES_TONIC       0x06
#define EVA_TONIC       0x07
#define MAG_TONIC       0x08
#define RAINBOW_TONIC   0x09

extern u32 HPTonicOffset_Link;
extern u32 StrTonicOffset_Link;
extern u32 MagTonicOffset_Link;
extern u32 DexTonicOffset_Link;
extern u32 SpdTonicOffset_Link;
extern u32 DefTonicOffset_Link;
extern u32 ResTonicOffset_Link;
extern u32 EvaTonicOffset_Link;

extern u8 TonicStatBonusTable[];
extern u8 HangoverIDLink;
extern u8 PotentBrewIDLink;
extern u16 TonicUseBoxTextPointer;
int GetTonicBitFromStat(int statIndex);

extern bool(*gSkillTester)(Unit* unit, int skillID);

extern u8 TonicChapterExclusionTable[];

int GetUsedTonicBit(struct Unit* unit, int item);
void DoTonicEffect(struct Unit* unit, int itemSlot);
int CheckNumberOfTonics(struct Unit* unit);

extern const u32 TonicPopup[];