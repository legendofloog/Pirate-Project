#include "gbafe.h"

extern bool SkillTester(Unit* unit, u8 skillID);
extern u8 PotentBrewID_Link;
extern u8 AddTonicBonus(Unit* unit, u8 statIndex);

// Stat Indexes in standard stat arrays
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

u8 PotentBrewBonus(u8 stat, Unit* unit, u8 statIndex) {
	if (SkillTester(unit, PotentBrewID_Link)) {
		if (AddTonicBonus(unit, statIndex) != 0) {
			stat += 2;
		}
	}
	return stat;
}

u8 PotentBrew_HP(u8 stat, Unit* unit) {
	return PotentBrewBonus(stat, unit, STAT_HP);
}

u8 PotentBrew_Strength(u8 stat, Unit* unit) {
	return PotentBrewBonus(stat, unit, STAT_STRENGTH);
}

u8 PotentBrew_Skill(u8 stat, Unit* unit) {
	return PotentBrewBonus(stat, unit, STAT_SKILL);
}

u8 PotentBrew_Speed(u8 stat, Unit* unit) {
	return PotentBrewBonus(stat, unit, STAT_SPEED);
}

u8 PotentBrew_Defense(u8 stat, Unit* unit) {
	return PotentBrewBonus(stat, unit, STAT_DEFENSE);
}

u8 PotentBrew_Resistance(u8 stat, Unit* unit) {
	return PotentBrewBonus(stat, unit, STAT_RESISTANCE);
}

u8 PotentBrew_Luck(u8 stat, Unit* unit) {
	return PotentBrewBonus(stat, unit, STAT_LUCK);
}

u8 PotentBrew_Mov(u8 stat, Unit* unit) {
	return PotentBrewBonus(stat, unit, STAT_MOV);
}

u8 PotentBrew_Con(u8 stat, Unit* unit) {
	return PotentBrewBonus(stat, unit, STAT_CON);
}

u8 PotentBrew_Magic(u8 stat, Unit* unit) {
	return PotentBrewBonus(stat, unit, STAT_MAGIC);
}

