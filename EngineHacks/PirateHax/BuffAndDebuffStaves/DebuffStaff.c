int AttackStaffMapSelect_Init(struct Proc*proc);
void StartUnitStaffOffenseInfoWindow(struct Proc* parent);
u8 AttackStaffMapSelect_SwitchIn(struct Proc* proc, struct TargetEntry* target);
void RefreshUnitStaffOffenseInfoWindow(struct Unit* unit, int hit);
int GetOffensiveStaffAccuracy(struct Unit* actor, struct Unit* target);
extern void DrawAccuracyText(struct TextHandle* text, int accuracy);
extern struct TargetSelectionDefinition CONST_DATA DebuffStaffSelectInfo;
void PrepareTargetSelectionForOffensiveStaff(struct Unit* unit, void(*func)(struct Unit*));

void PrepareTargetSelectionForOffensiveStaff(struct Unit* unit, void(*func)(struct Unit*))
{
    func(unit);

    BmMapFill(gMapMovement, -1);

    StartBottomHelpText(
        StartTargetSelection(&DebuffStaffSelectInfo),
        GetStringFromIndex(0x87B)); // TODO: msgid "Select a unit to use the staff on."
}

int AttackStaffMapSelect_Init(struct Proc*proc)
{
    StartUnitStaffOffenseInfoWindow(proc);
}

void StartUnitStaffOffenseInfoWindow(struct Proc* parent) {
    struct UnitInfoWindowProc* proc = NewUnitInfoWindow(parent);

    Text_InitDB(proc->lines, 8);
    return;
}

u8 AttackStaffMapSelect_SwitchIn(struct Proc* proc, struct TargetEntry* target)
{
    ChangeActiveUnitFacing(target->x, target->y);

    RefreshUnitStaffOffenseInfoWindow(
        GetUnit(target->unitIndex),
        GetOffensiveStaffAccuracy(gActiveUnit, GetUnit(target->unitIndex)));
}

void RefreshUnitStaffOffenseInfoWindow(struct Unit* unit, int hit) {
    int y = 0;
    int x = GetUnitInfoWindowX(unit, 10);

    struct UnitInfoWindowProc* proc = UnitInfoWindow_DrawBase(0, unit, x, y, 10, 1);

    DrawAccuracyText(proc->lines, hit);
    Text_Display(proc->lines, gBg0MapBuffer + TILEMAP_INDEX(x + 1, y + 3));

    return;
}