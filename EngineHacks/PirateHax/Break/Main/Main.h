bool DidUnitBreak();
void BreakPostBattle();
void ClearActiveFactionBreakStatus();

typedef struct BreakEntry BreakEntry;

struct BreakEntry
{
	u8 breakerWType;
	u8 brokenWType;
};

extern struct BreakEntry BreakTargetTable[];

extern u8 BreakExemptCharacterList[];
extern u8 BreakExemptClassList[];

//need these for break display

bool CheckEventId(int eventId);
void UnsetEventId(int eventId);
void SetEventId(int eventId);

//skillsys stuff
extern u8 DazzleIDLink;
extern u8 ArcaneArtificeIDLink;
extern u8 PulverizeIDLink;
extern u8 HandCannonIDLink;

extern bool(*gSkillTester)(Unit* unit, int skillID);


//#define SKILLSTATE_BREAK (1 << 2)
//#define SKILLSTATE_BROKEN_IN_BATTLE (1 << 3)
#define BATTLE_HIT_BREAK BATTLE_HIT_ATTR_12 // (1 << 17)

extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(struct Unit* unit);

extern int BreakBitOffset_Link;
extern int BreakInBattleBitOffset_Link;