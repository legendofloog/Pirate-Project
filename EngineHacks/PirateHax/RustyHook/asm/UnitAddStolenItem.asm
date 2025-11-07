.thumb

.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm

.equ gActionData, 0x0203A958
.equ GetUnit, 0x08019430
.equ GetItemWType, 0x08017548
.equ GetItemWeight, 0x0801760C
.equ GetUnitSpeed, 0x08019211

.global UnitAddStolenItem
.type UnitAddStolenItem, %function


		UnitAddStolenItem:
		push	{r4-r7,r14}
		mov		r4, r0 @unit
		mov		r5, r1 @item
		mov		r7, #0xFF
		
		@Skip changing durability on durability based items
		ldr		r3, =DurabilityItemList
		and		r1, r7
		
		LoopDurabilityBasedItems:
		ldrb	r2, [r3]
		cmp		r2, #0
		beq		IsRustyHookEquipped
		
			cmp		r1, r2
			beq		End
			
				add		r3, #1
				b		LoopDurabilityBasedItems
		
		IsRustyHookEquipped:
		blh		GetUnitEquippedItem, r1
		and		r0, r7
		ldr		r1, =RustyHookIDLink
		ldrb	r1, [r1]
		cmp		r0, r1
		bne		End
		
		@Determine if unit needed rusty hook to steal
		
		@Speed check				@ Need to have this use the speed getter instead, so making an edit - Loog
		ldr		r0, =gActionData
		ldrb	r0, [r0,#0x0D] 
		blh		GetUnit, r1	@ target unit is in r0
		mov r1, r4			@ the actor unit is in r1
				
		push {r4-r7}		@ need to clear this for a bit
		
		mov r4, r0
		mov r5, r1
		
		blh		GetUnitSpeed, r3
		
		mov r4, r0	@ r0 should now be the target's speed, so storing it in r4
		mov r0, r5	@ putting the actor here now
		blh		GetUnitSpeed, r3
		
		mov r1, r4 @ r0 now has actor's speed, r1 has the target's speed
		pop {r4-r7} @ restore these registers to what they had before

		

		cmp		r0, r1 @ if actor's speed is greater or equal, then didn't need rustyhook (right?): so put blt
		blt		RustyHookLowerDurability
		
		@Wt check
		mov		r0, r5
		blh		GetItemWType, r1
		cmp		r0, #9
		beq		End
		
			mov		r0, r4
			blh		prConGetter, r1
			mov		r6, r0
			mov		r0, r5
			blh		GetItemWeight, r1
			cmp		r6, r0
			bge		End
			
		RustyHookLowerDurability:
		lsr		r0, r5, #8 @get only durability
		
		@25%
		lsr		r0, #2
		
		@33%
		@mov		r1, #3
		@swi		#6
		
		@minimum of 1 use
		cmp		r0, #0
		bgt		StoreNewDurability
		
			mov		r0, #1
		
		StoreNewDurability:
		lsl		r0, #8
		and		r5, r7
		orr		r5, r0
		
		End:
		mov		r0, r4
		mov		r1, r5
		blh		UnitAddStolenItemHelper, r2
		pop		{r4-r7}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg

