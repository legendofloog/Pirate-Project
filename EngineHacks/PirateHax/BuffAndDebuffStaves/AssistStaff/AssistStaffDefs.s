
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

.equ ExecStandardHeal, 0x0802EB98
.equ ItemEffectAfterAnimProc, 0x095A6E80
.equ ProcStartBlocking, 0x08002CE0
