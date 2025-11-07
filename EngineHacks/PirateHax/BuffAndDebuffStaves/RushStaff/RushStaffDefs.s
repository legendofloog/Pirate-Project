
.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm

.equ gActionData, 0x0203A958
.equ GetUnit, 0x08019430

.equ IsGeneratedTargetListEmpty, 0x08029068
.equ gUnitSubject, 0x02033F3C
.equ AreAllegiancesAllied, 0x08024D8C
.equ AddTarget, 0x0804F8BC

.equ gMapMovement, 0x0202E4E0
.equ ClearMapWith, 0x080197E4
.equ StartTargetSelection, 0x0804FA3C
.equ String_GetFromIndex, 0x0800A240
.equ StartBottomHelpText, 0x08035708
.equ ChangeActiveUnitFacing, 0x0801F50C

.equ GetUnitInfoWindowX, 0x080349D4
.equ UnitInfoWindow_DrawBase, 0x0803483C
.equ gBg0MapBuffer, 0x02022CA8
.equ Text_Display, 0x08003E70
.equ Text_Clear, 0x08003DC8
.equ Text_InsertString, 0x08004480 
.equ Text_InsertNumberOr2Dashes, 0x080044A4 

.equ SetupSubjectBattleUnitForStaff, 0x0802CB24
.equ SetupTargetBattleUnitForStaff, 0x0802CBC8
.equ FinishUpItemBattle, 0x0802CC54
.equ BeginBattleAnimations, 0x0802CA14
.equ ItemEffectAfterAnimProc, 0x095A6E80
.equ ProcStartBlocking, 0x08002CE0

.equ gChapterData, 0x0202BCF0
.equ gUnitLookup, 0x0859A5D0
