#include "BuffStaves.h"

void DrawUnitResChangeText(struct TextHandle* text, struct Unit* unit, int bonus) {
	Text_Clear(text);

	Text_InsertString(text, 0, 3, GetStringFromIndex(0x4f0)); // Def[.][X]
	Text_InsertString(text, 16, 3, GetStringFromIndex(0x20D)); // :
    Text_InsertString(text, 44, 3, GetStringFromIndex(HealArrowIDLink)); // ->

    Text_InsertNumberOr2Dashes(text, 64, 2, (GetUnitResistance(unit)) + bonus);
    Text_InsertNumberOr2Dashes(text, 32, 2, (GetUnitResistance(unit)));
}

#include "FortuneStaff.c"

#include "ProtectStaff.c"

#include "ConcentrateStaff.c"

#include "BarrierStaff.c"

#include "DebuffStaff.c"