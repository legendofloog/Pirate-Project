.thumb

.include "../RushStaffDefs.s"

.global RushStaffUsability
.type RushStaffUsability, %function

.global RushStaffRangeSetup
.type RushStaffRangeSetup, %function

.global TryAddUnitToRushStaffTargetList
.type TryAddUnitToRushStaffTargetList, %function

.global RushStaffTargeting
.type RushStaffTargeting, %function

.global MakeTargetListForRushStaff
.type MakeTargetListForRushStaff, %function

.global RushStaffTargetChange
.type RushStaffTargetChange, %function

.global RushStaffSetupTargetWindow
.type RushStaffSetupTargetWindow, %function

.global UnitInfoWindow_RushStaffMov
.type UnitInfoWindow_RushStaffMov, %function

.global RushStaffEffect
.type RushStaffEffect, %function

.global RushStaffClearBit
.type RushStaffClearBit, %function

.global RushStaffExtraMov
.type RushStaffExtraMov, %function

.global RushStaffCanto
.type RushStaffCanto, %function


@-------------------------------------------
@RushStaffUsability
@-------------------------------------------


		RushStaffUsability:
		mov		r0, r4
		mov		r2, r5
		ldr		r1, =RushStaffRangeSetup
		blh		IsGeneratedTargetListEmpty, r3
		pop		{r4-r5}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@-------------------------------------------
@RushStaffRangeSetup
@-------------------------------------------


		RushStaffRangeSetup:
		push	{r14}
		ldr		r1, =TryAddUnitToRushStaffTargetList
		blh		Item_TURange, r3
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@TryAddUnitToRushStaffTargetList
@-------------------------------------------


		TryAddUnitToRushStaffTargetList:
		push	{r4-r5,r14}
		mov		r4, r0
		
		@Check if unit is being rescued
		ldr		r1, [r0,#0x0C]
		mov		r2, #0x20
		tst		r1, r2
		bne		TryAddUnitToRushStaffTargetList_End
		
			ldr		r0, =gUnitSubject
			ldr		r0, [r0]
			ldrb	r0, [r0,#0x0B]
			ldrb	r1, [r4,#0x0B]
			blh		AreAllegiancesAllied, r2
			cmp		r0, #0
			beq		TryAddUnitToRushStaffTargetList_End
			
				@To be added, unit needs to not have the rush staff buff
				mov		r0, r4
				blh		IsRushStaffBitSet, r1
				cmp		r0, #0
				bne		TryAddUnitToRushStaffTargetList_End
				
		TryAddUnitToRushStaffTargetList_Add:
		mov		r0, r4
		mov		r1, #0x11
		ldsb	r1, [r4,r1]
		mov		r2, #0x0B
		ldsb	r2, [r4,r2]
		mov		r3, #0x10
		ldsb	r0, [r4,r3]
		mov		r3, #0
		blh		AddTarget, r4
				
		TryAddUnitToRushStaffTargetList_End:
		pop		{r4-r5}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@RushStaffTargeting
@-------------------------------------------


		RushStaffTargeting:
		mov		r0, r5
		mov		r2, r4
		blh		MakeTargetListForRushStaff, r3
		ldr		r0, =gMapMovement
		ldr		r0, [r0]
		mov		r1, #1
		neg		r1, r1
		blh		ClearMapWith, r2
		
		ldr		r0, =gMenu_RushStaffTargetMenu
		blh		StartTargetSelection, r1
		mov		r4, r0
		
		ldr		r0, =RushStaffTargetBottomTextLink
		ldrh	r0, [r0]
		blh		String_GetFromIndex, r1
		mov		r1, r0
		mov		r0, r4
		blh		StartBottomHelpText, r2
		
		pop		{r4-r5}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@MakeTargetListForRushStaff
@-------------------------------------------


		MakeTargetListForRushStaff:
		push	{r14}
		blh		RushStaffRangeSetup, r1
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@RushStaffTargetChange
@-------------------------------------------


		RushStaffTargetChange:
		push	{r4,r14}
		mov		r4, r1
		
		mov		r0, #0
		ldsb	r0, [r4,r0]
		mov		r1, #1
		ldsb	r1, [r4,r1]
		blh		ChangeActiveUnitFacing, r2
		
		ldrb	r0, [r4,#2]
		blh		GetUnit, r1
		blh		RushStaffSetupTargetWindow, r1
		
		pop		{r4}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@RushStaffSetupTargetWindow
@-------------------------------------------


		RushStaffSetupTargetWindow:
		push	{r4-r6,r14}
		add		sp, #-0x0C
		str		r0, [sp,#8] @target unit
		
		mov		r1, #0x0A
		blh		GetUnitInfoWindowX, r2
		mov		r4, r0
		
		mov		r0, #0x0A
		str		r0, [sp]
		mov		r0, #1
		str		r0, [sp,#4]
		
		mov		r0, #0
		ldr		r1, [sp,#8]
		mov		r2, r4
		mov		r3, #0
		blh		UnitInfoWindow_DrawBase, r5
		mov		r5, r0
		add		r5, #0x38
		
		@Display change in Mov
		mov		r0, r5
		ldr		r1, [sp,#8]
		blh		UnitInfoWindow_RushStaffMov, r2
		mov		r1, r4
		add		r1, #0x61
		lsl		r1, #1
		ldr		r6, =gBg0MapBuffer
		add		r1, r6

		mov		r0, r5
		blh		Text_Display, r2
		
		add		sp, #0x0C
		pop		{r4-r6}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@UnitInfoWindow_RushStaffMov
@-------------------------------------------


		UnitInfoWindow_RushStaffMov:
		push	{r4-r7,r14}
		mov		r5, r0
		mov		r6, r1 @unit
		
		@"Mov"
		blh		Text_Clear, r1
		ldr		r0, =RushStaffTargetWindowTextLink
		ldrh	r0, [r0]
		blh		String_GetFromIndex, r1
		mov		r3, r0
		mov		r0, r5
		mov		r1, #0 @spacing
		mov		r2, #3 @font color
		blh		Text_InsertString, r4
		
		@Current Mov
		mov		r0, r6
		blh		prGotoMovGetter, r1
		mov		r3, r0
		mov		r7, r0
		mov		r0, r5
		mov		r1, #0x1A @spacing
		mov		r2, #2 @font color
		cmp		r3, #15
		blt		UnitInfoWindow_RushStaffMov_DisplayOld
		
			mov		r2, #4
			
		UnitInfoWindow_RushStaffMov_DisplayOld:
		blh		Text_InsertNumberOr2Dashes, r4
		
		@"->"
		ldr		r0, =RushStaffTargetWindowTextLink
		ldrh	r0, [r0,#2]
		blh		String_GetFromIndex, r1
		mov		r3, r0
		mov		r0, r5
		mov		r1, #0x24 @spacing
		mov		r2, #3 @font color
		blh		Text_InsertString, r4
		
		@Mov after Rush Staff
		mov		r2, #2 @font color
		mov		r3, r7
		cmp		r3, #0
		beq		UnitInfoWindow_RushStaffMov_DisplayNew
		
			ldr		r0, =RushStaffMovAmountLink
			ldrb	r0, [r0]
			add		r3, r0
			cmp		r3, #15
			blt		UnitInfoWindow_RushStaffMov_DisplayNew
			
				mov		r3, #15
				mov		r2, #4
				
		UnitInfoWindow_RushStaffMov_DisplayNew:
		mov		r0, r5
		mov		r1, #0x38 @spacing
		blh		Text_InsertNumberOr2Dashes, r4
		
		pop		{r4-r7}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@RushStaffEffect
@-------------------------------------------


		RushStaffEffect:
		ldr		r4, =gActionData
		ldrb	r0, [r4,#0x0C]
		blh		GetUnit, r1
		mov		r5, r0
		ldrb	r1, [r4,#0x12]
		blh		SetupSubjectBattleUnitForStaff, r2
		
		ldrb	r0, [r4,#0x0D]
		blh		GetUnit, r1
		mov		r7, r0
		blh		SetupTargetBattleUnitForStaff, r1
		
		mov		r0, r7
		blh		GetUnitDebuffEntry, r1
		ldr		r1, =RushStaffBitOffset_Link
		ldr		r1, [r1]
		blh		SetBit, r2
		
		mov		r0, r6
		blh		FinishUpItemBattle, r1
		blh		BeginBattleAnimations, r0
		
		@ldr		r0, =ItemEffectAfterAnimProc
		@mov		r1, r6
		@blh		ProcStartBlocking, r2
		
		pop		{r4}
		mov		r8, r4
		pop		{r4-r7}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@IsRushStaffBitSet
@-------------------------------------------


		IsRushStaffBitSet:
		push	{r14}
		blh		GetUnitDebuffEntry, r1
		ldr		r1, =RushStaffBitOffset_Link
		ldr		r1, [r1]
		blh		CheckBit, r2
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@-------------------------------------------
@RushStaffClearBit
@-------------------------------------------


		RushStaffClearBit:
		push	{r4-r6,r14}
		
		ldr		r0, =gChapterData
		ldrb	r0, [r0,#0x0F]
		add		r5, r0, #1 @unit to check
		mov		r6, r0
		add		r6, #0x40 @stop once all units of this faction have been checked
		
		RushStaffClearBit_Loop:
		ldr		r1, =gUnitLookup
		mov		r0, #0xFF
		and		r0, r5
		lsl		r0, #2
		add		r0, r1
		ldr		r4, [r0]
		cmp		r4, #0
		
			ldr		r0, [r4]
			cmp		r0, #0
			beq		RushStaffClearBit_Next
			
				@don't clear if rescued
				ldr		r1, [r4,#0x0C]
				mov		r2, #0x20
				tst		r1, r2
				bne		RushStaffClearBit_Next
				
					mov		r0, r4
					blh		GetUnitDebuffEntry, r1
					ldr		r1, =RushStaffBitOffset_Link
					ldr		r1, [r1]
					blh		UnsetBit, r2
					
					RushStaffClearBit_Next:
					add		r5, #1
					cmp		r5, r6
					blt		RushStaffClearBit_Loop
					
		pop		{r4-r6}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@RushStaffExtraMov
@-------------------------------------------


		RushStaffExtraMov:
		push	{r4-r5,r14}
		mov		r5, r0 @stat
		mov		r4, r1 @unit
		
		mov		r0, r4
		blh		IsRushStaffBitSet, r1
		cmp		r0, #0
		beq		RushStaffExtraMov_End
		
			ldr		r0, =RushStaffMovAmountLink
			ldrb	r0, [r0]
			add		r5, r0
			
		RushStaffExtraMov_End:
		mov		r0, r5
		pop		{r4-r5}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@-------------------------------------------
@RushStaffCanto
@-------------------------------------------


		RushStaffCanto:
		push	{lr}

		@check if dead
		ldrb	r0, [r4,#0x13]
		cmp	r0, #0x00
		beq	RushStaffCanto_End

		@check if waited
		ldrb  r0, [r6,#0x11]  @action taken this turn
		cmp r0, #0x1
		beq RushStaffCanto_End
		
		@ check for no move, if so don't do it
		ldr	r0,=0x8019224	@mov getter
		mov	lr, r0
		mov	r0, r4
		.short	0xF800
		cmp r0, #0x2
		blt RushStaffCanto_End
		
		mov		r0, r4
		blh		IsRushStaffBitSet, r1
		cmp		r0, #0
		beq		RushStaffCanto_End
		
		@check if unit already activated a Canto effect
		ldr r0, [r4,#0x0C]
		mov r1, #0x40
		tst r0, r1
		bne RushStaffCanto_End
		
		mov	r1, #0x02
		mvn	r1, r1
		and	r0, r1		@unset bit 0x2
		mov	r1, #0x40	@set canto bit
		orr	r0, r1
		str	r0, [r4,#0x0C]
		
		RushStaffCanto_End:
		pop {r0}
		bx r0
		
		.align
		.ltorg

