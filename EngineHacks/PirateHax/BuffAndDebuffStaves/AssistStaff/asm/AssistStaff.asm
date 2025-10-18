.thumb

.include "../AssistStaffDefs.s"

.global AssistStaffUsability
.type AssistStaffUsability, %function

.global AssistStaffRangeSetup
.type AssistStaffRangeSetup, %function

.global TryAddUnitToAssistStaffTargetList
.type TryAddUnitToAssistStaffTargetList, %function

.global AssistStaffTargeting
.type AssistStaffTargeting, %function

.global MakeTargetListForAssistStaff
.type MakeTargetListForAssistStaff, %function

.global AssistStaffTargetChange
.type AssistStaffTargetChange, %function

.global AssistStaffEffect
.type AssistStaffEffect, %function


@-------------------------------------------
@AssistStaffUsability
@-------------------------------------------


		AssistStaffUsability:
		mov		r0, r4
		mov		r2, r5
		ldr		r1, =AssistStaffRangeSetup
		blh		IsGeneratedTargetListEmpty, r3
		pop		{r4-r5}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@-------------------------------------------
@AssistStaffRangeSetup
@-------------------------------------------


		AssistStaffRangeSetup:
		push	{r14}
		ldr		r1, =TryAddUnitToAssistStaffTargetList
		blh		Item_TURange, r3
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@TryAddUnitToAssistStaffTargetList
@-------------------------------------------


		TryAddUnitToAssistStaffTargetList:
		push	{r4-r5,r14}
		mov		r4, r0
		
		@Check if unit is being rescued
		ldr		r1, [r0,#0x0C]
		mov		r2, #0x20
		tst		r1, r2
		bne		TryAddUnitToAssistStaffTargetList_End
		
			ldr		r0, =gUnitSubject
			ldr		r0, [r0]
			ldrb	r0, [r0,#0x0B]
			ldrb	r1, [r4,#0x0B]
			blh		AreAllegiancesAllied, r2
			cmp		r0, #0
			beq		TryAddUnitToAssistStaffTargetList_End
			
				@To be added, unit needs to either be injured or not have the take aim buff
				mov		r0, r4
				blh		prGotoCurHPGetter, r1
				mov		r5, r0
				mov		r0, r4
				blh		prGotoMaxHPGetter, r1
				cmp		r5, r0
				bne		TryAddUnitToAssistStaffTargetList_Add
				
				mov		r0, r4
				blh		IsFortuneBitSet, r1
				cmp		r0, #0
				bne		TryAddUnitToAssistStaffTargetList_End
				
		TryAddUnitToAssistStaffTargetList_Add:
		mov		r0, r4
		mov		r1, #0x11
		ldsb	r1, [r4,r1]
		mov		r2, #0x0B
		ldsb	r2, [r4,r2]
		mov		r3, #0x10
		ldsb	r0, [r4,r3]
		mov		r3, #0
		blh		AddTarget, r4
				
		TryAddUnitToAssistStaffTargetList_End:
		pop		{r4-r5}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@AssistStaffTargeting
@-------------------------------------------


		AssistStaffTargeting:
		mov		r0, r5
		mov		r2, r4
		blh		MakeTargetListForAssistStaff, r3
		ldr		r0, =gMapMovement
		ldr		r0, [r0]
		mov		r1, #1
		neg		r1, r1
		blh		ClearMapWith, r2
		
		ldr		r0, =gMenu_AssistStaffTargetMenu
		blh		StartTargetSelection, r1
		mov		r4, r0
		
		ldr		r0, =AssistStaffTargetBottomTextLink
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
@MakeTargetListForAssistStaff
@-------------------------------------------


		MakeTargetListForAssistStaff:
		push	{r14}
		blh		AssistStaffRangeSetup, r1
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@AssistStaffTargetChange
@-------------------------------------------


		AssistStaffTargetChange:
		push	{r4,r14}
		mov		r4, r1
		
		mov		r0, #0
		ldsb	r0, [r4,r0]
		mov		r1, #1
		ldsb	r1, [r4,r1]
		blh		ChangeActiveUnitFacing, r2
		
		ldrb	r0, [r4,#2]
		blh		GetUnit, r1
		blh		FE8_Show_Heal_Amount, r1
		
		pop		{r4}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@AssistStaffEffect
@-------------------------------------------


		AssistStaffEffect:
		mov		r0, r6
		blh		ExecStandardHeal, r1
		
		@Set +30 hit
		ldr		r0, =gActionData
		ldrb	r0, [r0,#0x0D]
		blh		GetUnit, r1
		blh		SetFortuneBit, r1
		
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

