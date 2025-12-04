.thumb

.include "../MucuDefs.s"

.global MucuTryAddUnitToTargetList
.type MucuTryAddUnitToTargetList, %function

.global MucuCannotIfCanto
.type MucuCannotIfCanto, %function

.global MucuCannotIfBallista
.type MucuCannotIfBallista, %function

.global MucuRequireInventory
.type MucuRequireInventory, %function

.global MucuRequireTargetAttack
.type MucuRequireTargetAttack, %function

.global MucuRequireTraveler
.type MucuRequireTraveler, %function

.global MucuCannotIfTraveler
.type MucuCannotIfTraveler, %function

.global MucuLimitOneTakeGive
.type MucuLimitOneTakeGive, %function

.global MucuRequireTargetGive
.type MucuRequireTargetGive, %function

.global MucuRequireTargetTake
.type MucuRequireTargetTake, %function

.global MucuTargetAllegiancesEqual
.type MucuTargetAllegiancesEqual, %function

.global MucuTargetAllegiancesAllied
.type MucuTargetAllegiancesAllied, %function

.global MucuTargetCannotRescuing
.type MucuTargetCannotRescuing, %function

.global MucuTargetRequireRescuing
.type MucuTargetRequireRescuing, %function

.global MucuTargetCannotBerserkSleep
.type MucuTargetCannotBerserkSleep, %function

.global MucuTargetCannotSupply
.type MucuTargetCannotSupply, %function

.global MucuTargetGiveConAid
.type MucuTargetGiveConAid, %function

.global MucuTargetTakeConAid
.type MucuTargetTakeConAid, %function

.global MucuRequireUsableStaff
.type MucuRequireUsableStaff, %function

.global MucuCannotIfMagicBlocked
.type MucuCannotIfMagicBlocked, %function

.global MucuTargetRequireEnemyFaction
.type MucuTargetRequireEnemyFaction, %function

.global MucuTargetRequireSpeedGE
.type MucuTargetRequireSpeedGE, %function

.global MucuTargetRequireStealableItem
.type MucuTargetRequireStealableItem, %function


@--------------------------------------------
@MucuTryAddUnitToTargetList
@--------------------------------------------


		MucuTryAddUnitToTargetList:
		push	{r4-r6,r14}
		mov		r4, r0
		ldr		r5, =gUnitSubject
		mov		r6, r1
		
		MucuTryAddUnitToTargetList_Loop:
		ldr		r2, [r6]
		cmp		r2, #0
		beq		MucuTryAddUnitToTargetList_Add
		
			ldr		r0, [r5]
			mov		r1, r4
			mov		lr, r2
			.short	0xF800
			cmp		r0, #0
			beq		MucuTryAddUnitToTargetList_End
			
				add		r6, #4
				b		MucuTryAddUnitToTargetList_Loop
		
		MucuTryAddUnitToTargetList_Add:
		ldr		r0, =AddTarget
		mov		lr, r0
		mov		r0, #0x10
		ldsb	r0, [r4,r0]
		mov		r1, #0x11
		ldsb	r1, [r4,r1]
		mov		r2, #0x0B
		ldsb	r2, [r4,r2]
		mov		r3, #0
		.short	0xF800
		
		MucuTryAddUnitToTargetList_End:
		pop		{r4-r6}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@--------------------------------------------
@MucuCannotIfCanto
@--------------------------------------------

		MucuCannotIfCanto:
		ldr		r1, [r0,#0x0C]
		mov		r2, #0x40
		tst		r1, r2
		beq		MucuCannotIfCanto_CanUse
		
			mov		r0, #0
			b		MucuCannotIfCanto_End
		
		MucuCannotIfCanto_CanUse:
		mov		r0, #1
		
		MucuCannotIfCanto_End:
		bx		r14
		
		.align
		.ltorg


@--------------------------------------------
@MucuCannotIfBallista
@--------------------------------------------

		MucuCannotIfBallista:
		ldr		r1, [r0,#0x0C]
		mov		r2, #0x80
		lsl		r2, #4
		tst		r1, r2
		beq		MucuCannotIfBallista_CanUse
		
			mov		r0, #0
			b		MucuCannotIfBallista_End
		
		MucuCannotIfBallista_CanUse:
		mov		r0, #1
		
		MucuCannotIfBallista_End:
		bx		r14
		
		.align
		.ltorg


@--------------------------------------------
@MucuRequireInventory
@--------------------------------------------

		MucuRequireInventory:
		ldrh	r0, [r0,#0x1E]
		cmp		r0, #0
		beq		MucuRequireInventory_End
		
			mov		r0, #1
			
		MucuRequireInventory_End:
		bx		r14
		
		.align
		.ltorg


@--------------------------------------------
@MucuRequireTargetAttack
@--------------------------------------------

		MucuRequireTargetAttack:
		push	{r4-r6,r14}
		mov		r6, #0
		mov		r5, r0
		ldrh	r4, [r5,#0x1E]
		
		MucuRequireTargetAttack_LoopStart:
		mov		r0, r4
		blh		GetItemAttributes, r1
		mov		r1, #1
		tst		r0, r1
		beq		MucuRequireTargetAttack_NextItem
		
			mov		r0, r5
			mov		r1, r4
			blh		CanUnitUseWeapon, r2
			lsl		r0, #0x18
			cmp		r0, #0
			beq		MucuRequireTargetAttack_NextItem
			
				mov		r0, r5
				mov		r1, r4
				blh		MakeTargetListForWeapon, r2
				blh		GetTargetListSize, r0
				cmp		r0, #0
				bne		MucuRequireTargetAttack_CanUse
				
					MucuRequireTargetAttack_NextItem:
					add		r6, #1
					cmp		r6, #4
					bgt		MucuRequireTargetAttack_CannotUse
					
						mov		r0, r6
						lsl		r0, #1
						add		r0, #0x1E
						ldrh	r4, [r5,r0]
						b		MucuRequireTargetAttack_LoopStart
			
		MucuRequireTargetAttack_CanUse:
		mov		r0, #1
		b		MucuRequireTargetAttack_End
		
		MucuRequireTargetAttack_CannotUse:
		mov		r0, #0
		
		MucuRequireTargetAttack_End:
		pop		{r4-r6}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@--------------------------------------------
@MucuRequireTraveler
@--------------------------------------------


		MucuRequireTraveler:
		ldr		r0, [r0,#0x0C]
		mov		r1, #0x10
		and		r0, r1
		cmp		r0, #0
		beq		MucuRequireTraveler_End
		
			mov		r0, #1
		
		MucuRequireTraveler_End:
		bx		r14
		
		.align
		.ltorg


@--------------------------------------------
@MucuCannotIfTraveler
@--------------------------------------------


		MucuCannotIfTraveler:
		ldr		r0, [r0,#0x0C]
		mov		r1, #0x10
		tst		r0, r1
		beq		MucuCannotIfTraveler_CanUse
		
			mov		r0, #0
			b		MucuCannotIfTraveler_End
			
		MucuCannotIfTraveler_CanUse:
		mov		r0, #1
		
		MucuCannotIfTraveler_End:
		bx		r14
		
		.align
		.ltorg


@--------------------------------------------
@MucuLimitOneTakeGive
@--------------------------------------------


		MucuLimitOneTakeGive:
		ldr		r0, =gGameState
		add		r0, #0x3D
		ldrb	r1, [r0]
		mov		r0, #1
		tst		r0, r1
		beq		MucuLimitOneTakeGive_CanUse
		
			mov		r0, #0
			b		MucuLimitOneTakeGive_End

		MucuLimitOneTakeGive_CanUse:
		mov		r0, #1
		
		MucuLimitOneTakeGive_End:
		bx		r14
		
		.align
		.ltorg


@--------------------------------------------
@MucuRequireTargetGive
@--------------------------------------------


		MucuRequireTargetGive:
		push	{r14}
		blh		MakeGiveTargetList, r1
		blh		GetTargetListSize, r0
		cmp		r0, #0
		beq		MucuRequireTargetGive_End
		
			mov		r0, #1
		
		MucuRequireTargetGive_End:
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@--------------------------------------------
@MucuRequireTargetTake
@--------------------------------------------


		MucuRequireTargetTake:
		push	{r14}
		blh		MakeTakeTargetList, r1
		blh		GetTargetListSize, r0
		cmp		r0, #0
		beq		MucuRequireTargetTake_End
		
			mov		r0, #1
		
		MucuRequireTargetTake_End:
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@--------------------------------------------
@MucuTargetAllegiancesEqual
@--------------------------------------------


		MucuTargetAllegiancesEqual:
		push	{r14}
		ldrb	r0, [r0,#0x0B]
		ldrb	r1, [r1,#0x0B]
		blh		AreAllegiancesEqual, r2
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@--------------------------------------------
@MucuTargetAllegiancesAllied
@--------------------------------------------


		MucuTargetAllegiancesAllied:
		push	{r14}
		ldrb	r0, [r0,#0x0B]
		ldrb	r1, [r1,#0x0B]
		blh		AreAllegiancesAllied, r2
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@--------------------------------------------
@MucuTargetCannotRescuing
@--------------------------------------------


		MucuTargetCannotRescuing:
		mov		r0, #0
		ldr		r1, [r1,#0x0C]
		mov		r2, #0x10
		tst		r1, r2
		bne		MucuTargetCannotRescuing_End
		
			mov		r0, #1
		
		MucuTargetCannotRescuing_End:
		bx		r14
		
		.align
		.ltorg

		
@--------------------------------------------
@MucuTargetRequireRescuing
@--------------------------------------------


		MucuTargetRequireRescuing:
		mov		r0, #0
		ldr		r1, [r1,#0x0C]
		mov		r2, #0x10
		tst		r1, r2
		beq		MucuTargetRequireRescuing_End
		
			mov		r0, #1
		
		MucuTargetRequireRescuing_End:
		bx		r14
		
		.align
		.ltorg


@--------------------------------------------
@MucuTargetCannotBerserkSleep
@--------------------------------------------


		MucuTargetCannotBerserkSleep:
		mov		r1, #0x30
		ldrb	r1, [r0,r1]
		lsl		r1, #28
		lsr		r1, #28
		mov		r0, #0
		cmp		r1, #4 @status id
		beq		MucuTargetCannotBerserkSleep_End
		
			cmp		r1, #2
			beq		MucuTargetCannotBerserkSleep_End
		
				mov		r0, #1
		
		MucuTargetCannotBerserkSleep_End:
		bx		r14
		
		.align
		.ltorg


@--------------------------------------------
@MucuTargetCannotSupply
@--------------------------------------------


		MucuTargetCannotSupply:
		ldr		r0, [r1]
		ldr		r0, [r0,#0x28]
		ldr		r1, [r1,#4]
		ldr		r1, [r1,#0x28]
		orr		r0, r1
		mov		r1, #0x80
		lsl		r1, #2
		tst		r0, r1
		bne		MucuTargetCannotSupply_CannotUse
		
			mov		r0, #1
			b		MucuTargetCannotSupply_End
		
		MucuTargetCannotSupply_CannotUse:
		mov		r0, #0
		
		MucuTargetCannotSupply_End:
		bx		r14
		
		.align
		.ltorg


@--------------------------------------------
@MucuTargetGiveConAid
@--------------------------------------------


		MucuTargetGiveConAid:
		push	{r4,r14}
		mov		r4, r1 @target
		ldrb	r0, [r0,#0x1B] @traveler with acting unit
		blh		GetUnit, r1
		mov		r1, r0
		mov		r0, r4
		blh		CanUnitRescue, r2
		pop		{r4}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@--------------------------------------------
@MucuTargetTakeConAid
@--------------------------------------------


		MucuTargetTakeConAid:
		push	{r4,r14}
		mov		r4, r0 @acting unit
		ldrb	r0, [r1,#0x1B] @traveler with target
		blh		GetUnit, r1
		mov		r1, r0
		mov		r0, r4
		blh		CanUnitRescue, r2
		pop		{r4}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@--------------------------------------------
@MucuRequireUsableStaff
@--------------------------------------------


		MucuRequireUsableStaff:
		push	{r4-r6,r14}
		mov		r4, r0
		mov		r6, #0
		
		MucuRequireUsableStaff_Loop:
		lsl		r0, r6, #1
		add		r0, #0x1E
		ldrh	r5, [r4,r0]
		cmp		r5, #0
		beq		MucuRequireUsableStaff_CannotUse
		
			mov		r0, r5
			blh		GetItemWType, r1
			cmp		r0, #4
			bne		MucuRequireUsableStaff_NextItem
			
				mov		r0, r4
				mov		r1, r5
				blh		CanUnitUseItem, r2
				cmp		r0, #0
				beq		MucuRequireUsableStaff_NextItem
				
					mov		r0, #1
					b		MucuRequireUsableStaff_End
				
					MucuRequireUsableStaff_NextItem:
					add		r6, #1
					cmp		r6, #4
					ble		MucuRequireUsableStaff_Loop
		
		MucuRequireUsableStaff_CannotUse:
		mov		r0, #0
		
		MucuRequireUsableStaff_End:
		pop		{r4-r6}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@--------------------------------------------
@MucuCannotIfMagicBlocked
@--------------------------------------------


		MucuCannotIfMagicBlocked:
		push	{r14}
		blh		CanUnitNotUseMagic, r1
		cmp		r0, #0
		bne		MucuCannotIfMagicBlocked_CannotUse
		
			mov		r0, #1
			b		MucuCannotIfMagicBlocked_End
			
		MucuCannotIfMagicBlocked_CannotUse:
		mov		r0, #2
		
		MucuCannotIfMagicBlocked_End:
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@--------------------------------------------
@MucuTargetRequireEnemyFaction
@--------------------------------------------


		MucuTargetRequireEnemyFaction:
		mov		r2, #0
		ldrb	r0, [r1,#0x0B]
		mov		r1, #0xC0
		and		r0, r1
		cmp		r0, #0x80
		bne		MucuTargetRequireEnemyFaction_End
		
			mov		r2, #1
			
		MucuTargetRequireEnemyFaction_End:
		mov		r0, r2
		bx		r14
		
		.align
		.ltorg


@--------------------------------------------
@MucuTargetRequireSpeedGE
@--------------------------------------------


		MucuTargetRequireSpeedGE:
		mov		r3, #1
		mov		r2, #0x16
		ldsb	r0, [r0,r2]
		ldsb	r1, [r1,r2]
		cmp		r0, r1
		bge		MucuTargetRequireSpeedGE_End
		
			mov		r3, #0
			
		MucuTargetRequireSpeedGE_End:
		mov		r0, r3
		bx		r14
		
		.align
		.ltorg


@--------------------------------------------
@MucuTargetRequireStealableItem
@--------------------------------------------


		MucuTargetRequireStealableItem:
		push	{r4-r6,r14}
		mov		r4, r1
		add		r4, #0x1E
		mov		r5, r1
		mov		r6, #0
		
		MucuTargetRequireStealableItem_Loop:
		ldrh	r0, [r4]
		cmp		r0, #0
		beq		MucuTargetRequireStealableItem_End
		
			mov		r0, r5 @StealPlus change
			mov		r1, r6 @StealPlus change
			blh		NewIsItemStealable, r2
			cmp		r0, #0
			beq		MucuTargetRequireStealableItem_NextItem
			
				mov		r0, #1
				b		MucuTargetRequireStealableItem_End
				
				MucuTargetRequireStealableItem_NextItem:
				add		r4, #2
				add		r6, #1
				cmp		r6, #4
				ble		MucuTargetRequireStealableItem_Loop
				
					mov		r0, #0
					
		MucuTargetRequireStealableItem_End:
		pop		{r4-r6}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg

