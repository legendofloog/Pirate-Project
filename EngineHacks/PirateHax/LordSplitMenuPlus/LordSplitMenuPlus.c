#include "gbafe.h"

void CallChoiceMenuASMC(struct Proc* proc)
{
    ClearBg0Bg1();
    SetDispEnable(1, 1, 1, 1, 1);
    SetTextFont(0);
    InitSystemTextFont();
    LoadUiFrameGraphics();
	
	struct MenuDef* menu = (struct MenuDef*) gEventSlots[0x1];
	
    StartMenu(menu, proc);
}

u8 FlagUsabilityMenuOption(struct MenuItemDef* def, int number) {
	if(def->overrideId == 0) {
		return MENU_ENABLED;
	}
	
	if(CheckFlag(def->overrideId)) {
		return MENU_ENABLED;
	}
	
	return MENU_DISABLED;
}

u8 ChoiceMenu_OnSelect(struct MenuProc* menu, struct MenuItemProc* menu_item) {
	if(menu_item->availability == MENU_ENABLED) {
		gEventSlots[0xC] = menu_item->itemNumber;
		return MENU_ACT_CLEAR | MENU_ACT_SND6A | MENU_ACT_END | MENU_ACT_SKIPCURSOR;
	}
	return MENU_ACT_SND6B;
}
