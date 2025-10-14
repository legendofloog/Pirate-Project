#include "gbafe.h"

extern u8 AstralBlessingIDLink;
extern bool SkillTester(Unit* unit, u8 skillID);
extern bool IsUnitOnField(Unit* unit);
extern long long AuraSkillCheck(Unit* unit, int skill, int param, int maxRange);

extern u32 AstralBlessingSklOffset_Link;
extern u32 AstralBlessingSpdOffset_Link;
extern u32 AstralBlessingLckOffset_Link;

extern void SetBit(u32* address, u8 bitOffset);
extern void UnsetBit(u32* address, u8 bitOffset);
extern bool CheckBit(u32* address, u8 bitOffset);
extern u32* GetUnitDebuffEntry(Unit* unit);

bool AuraSkillCheckWrapper(Unit* unit, int skill, int param, int maxRange);
void AstralBlessing();
void ApplyAstralBlessing(Unit* unit);

extern u8 AstralBlessing_Spd;
extern u8 AstralBlessing_Skl;
extern u8 AstralBlessing_Luk;

extern u8 AstralBlessing_Range;

long long AstralBlessingSpdBoost(u8 stat, Unit* unit);
long long AstralBlessingSklBoost(u8 stat, Unit* unit);
long long AstralBlessingLukBoost(u8 stat, Unit* unit);

extern void StartBuffFx(Unit* unit, u8 rallyBits, u8 range); //not the real return type but works for here

struct FxProc {
	PROC_HEADER;
	u32 unit;
	u32 effectBits;
	u32 range;
} typedef FxProc;

extern FxProc BuffFxProc;
