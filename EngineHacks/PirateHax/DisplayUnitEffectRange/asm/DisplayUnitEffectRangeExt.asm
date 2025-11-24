@Requires ItemRangeFix 

.thumb

.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm

.equ gActiveUnit, 0x03004E50
.equ gActionData, 0x0203A958
.equ FillMovementMapForUnitAndMovement, 0x0801A3CC
.equ gMapMovement2, 0x0202E4F0
.equ ClearMapWith, 0x080197E4
.equ UnitHasMagicRank, 0x08018A58
.equ MapSetInMagicSealedRange, 0x0801B950
.equ gMapRange, 0x0202E4E4
.equ GetUnitUseFlags, 0x08018B28
.equ gGameState, 0x0202BCB0
.equ DisplayMoveRangeGraphics, 0x0801DA98

.global DisplayUnitEffectRangeExt
.type DisplayUnitEffectRangeExt, %function


		DisplayUnitEffectRangeExt:
		push	{r4-r6,r14}
		mov		r6, r0
		mov		r4, #1
		ldr		r5, =gActiveUnit
		ldr		r0, [r5]
		blh		prGotoMovGetter, r2
		mov		r1, r0
		ldr		r0, [r5]
		ldr		r2, =gActionData
		ldrb	r2, [r2,#0x10]
		sub		r1, r2
		lsl		r1, #0x18
		asr		r1, #0x18
		blh		FillMovementMapForUnitAndMovement, r3
		
		@Change from vanilla: regardless of Canto result, clear gMapMovement2 and gMapRange
		ldr		r0, =gMapMovement2
		ldr		r0, [r0]
		mov		r1, #0
		blh		ClearMapWith, r2
		
		ldr		r0, =gMapRange
		ldr		r0, [r0]
		mov		r1, #0
		blh		ClearMapWith, r2
		
		@If Cantoing and no Rerun-And-Hit, skip displaying item ranges
		ldr		r0, [r5]
		ldr		r1, [r0,#0x0C]
		mov		r2, #0x40
		tst		r1, r2
		beq		CheckMagicRank
		@bne		GoToDisplayMoveRangeGraphics @use this if Rerun-And-Hit isn't installed
		
			ldr		r1, =RerunAndHitIDLink
			ldrb	r1, [r1]
			blh		SkillTester, r3
			cmp		r0, #0
			beq		GoToDisplayMoveRangeGraphics
		
		CheckMagicRank:
		mov		r0, r6
		blh		UnitHasMagicRank, r1
		lsl		r0, #0x18
		cmp		r0, #0
		beq		NoMagicRange
		
			mov		r0, #1
			blh		MapSetInMagicSealedRange, r1
				
		NoMagicRange:
		ldr		r0, [r5]
		blh		GetUnitUseFlags, r1
		cmp		r0, #2
		beq		DisplayStaffRange
		
		cmp		r0, #1
		beq		DisplayWeaponRange
		
		cmp		r0, #3
		bne		GoToDisplayMoveRangeGraphics
		
			ldr		r0, =gGameState
			add		r0, #0x3E
			ldrb	r0, [r0]
			tst		r4, r0
			beq		DisplayWeaponRange
			
				DisplayStaffRange:
				ldr		r0, [r5]
				blh		All_Staffs_All_Squares, r1
				mov		r4, #5
				b		GoToDisplayMoveRangeGraphics
				
				DisplayWeaponRange:
				ldr		r0, [r5]
				blh		All_Weapons_All_Squares, r1
				mov		r4, #3
				
		GoToDisplayMoveRangeGraphics:
		mov		r0, r4
		blh		DisplayMoveRangeGraphics, r1
		pop		{r4-r6}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg

