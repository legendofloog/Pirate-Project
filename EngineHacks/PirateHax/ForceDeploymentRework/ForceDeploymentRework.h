#include "gbafe.h"


struct NewForceDeploymentEntry {
    /* 00 */ u8 unitId;
    /* 01 */ u8 route;
    /* 02 */ u8 chapter;
    /* 03 */ u8 xPos;
    /* 04 */ u8 yPos;
};

extern struct NewForceDeploymentEntry NewForceDeploymentList[];

bool IsCharacterForceDeployed_(u8 pid);

bool IsCharacterLocked(int unitId);

s8 IsCharacterForceDeployed(int char_id);

bool CanCharacterBePrepMoved(int unitId);

int CountForceDeployedUnits();

int GetChapterAllyUnitCount(void);
void InitUnitStack(void* buff);
extern u8 gPrepUnitPool[];
s8 IsUnitInCurrentRoster(struct Unit *unit);
void PushUnit(struct Unit* unit);
void LoadPlayerUnitsFromUnitStack();
s8 SomeLeftoverFunctionThatReturns0(struct Unit *unit);