.thumb

	WriteAndVerifySramFast = 0x080D184C+1
	ReadSramFastAddr       = 0x030067A0   @ pointer to the actual ReadSramFast function

	gActiveUnit = 0x3004E50
	GetUnit = 0x8019430
	gActionData = 0x203A958
	RemoveUnitBlankItems = 0x8017984+1
	Memset = 0x80D1C6C+1
	gChapterData = 0x202BCF0
	
	.equ MaxTonicIndex, 10

	.global SUD_SaveTonic
	.type   SUD_SaveTonic, function

	.global SUD_LoadTonic
	.type   SUD_LoadTonic, function

	.global TonicEffectLadder
	.type 	TonicEffectLadder, function

	.global TonicUsabilityLadder
	.type 	TonicUsabilityLadder, function

	.global TonicPrepUsabilityLadder
	.type 	TonicPrepUsabilityLadder, function

	.global TonicPrepEffectLadder
	.type 	TonicPrepEffectLadder, function

	.global DelayedTonicDeletion
	.type 	DelayedTonicDeletion, function

	.global ClearTonicDebuffsHook
	.type ClearTonicDebuffHook, function

	.macro blh to, reg = r3
    ldr \reg, =\to
    mov lr, \reg
    .short 0xF800
	.endm
.align

.global SUD_SaveTonic		@ in the rework, these save and load tonic functiosn will not matter: eventually will be removed - Loog
.type SUD_SaveTonic, %function 
SUD_SaveTonic:
	@ arguments:
	@ - r0 = target address (SRAM)
	@ - r1 = target size

	ldr r3, =WriteAndVerifySramFast

	mov r2, r1   @ WriteAndVerifySramFast arg r2 = size
	mov r1, r0   @ WriteAndVerifySramFast arg r1 = target address

	ldr r0, =TonicTablePointer
	ldr r0, [r0] @ WriteAndVerifySramFast arg r0 = source address
	ldr r0, [r0]

	bx  r3

	.pool

	.align

.global SUD_LoadTonic
.type SUD_LoadTonic, %function 
SUD_LoadTonic:
	@ arguments:
	@ - r0 = source address (SRAM)
	@ - r1 = source size

	ldr r3, =ReadSramFastAddr
	ldr r3, [r3] @ r3 = ReadSramFast

	mov r2, r1   @ ReadSramFast arg r2 = size
	@ implied    @ ReadSramFast arg r0 = source address

	ldr r1, =TonicTablePointer
	ldr r1, [r1] @ ReadSramFast arg r1 = target address
	ldr r1, [r1]

	bx  r3

	.pool

	.align


TonicEffectLadder:

	mov r0, r6	@ this should be the proc

	bl TonicEffectFunc

	ldr r0,=0x802FF77		@ proc blocking I think
	bx r0

	.ltorg
	.align

TonicPrepEffectLadder:

	mov r0, r4	@ the unit pointer
	mov r1, r7	@ the item slot

	bl TonicPrepEffectFunc

	ldr r0, =TonicUseBoxTextPointer
	ldr r0, [r0]
	ldr r0, [r0]
	
	pop {r4-r7}
	pop {r1}
	bx r1

	.ltorg
	.align

TonicUsabilityLadder:

	mov r0, r4	@ the unit pointer
	mov r1, r5

	bl TonicUsabilityFunc

	pop {r4,r5}
	pop {r1}
	bx r1

	.ltorg
	.align

TonicPrepUsabilityLadder:

	mov r0,r5
	mov r1,r4		@ pass in as unit, item

	bl TonicPrepUsabilityFunc

	pop {r4,r5}
	pop {r1}
	bx r1


	.ltorg
	.align

DelayedTonicDeletion:		@ i don't care to rewrite this piece since it seems to be working just fine and is unrelated to tonic changes to debuff - Loog
	@no params, no return val
	push {r4,r14}
	ldr r4,=gActionData
	
	@is the action type use item?
	ldrb r0,[r4,#0x11]
	cmp r0,#0x1A
	bne DelayedTonicDeletion_End
	
	@is the item used a tonic?
	ldrb r0, [r4, #0x12] @ item slot used
	lsl r0,#1 @ *2
	add r0,#0x1E @items offset
	ldr r1,=gActiveUnit
	ldr r1,[r1]
	add r0,r1
	ldrh r0,[r0]
	mov r1,#0xFF
	and r0,r1
	ldr r1,=TonicItemID
	ldrb r1,[r1]
	cmp r0,r1
	bne DelayedTonicDeletion_End
	
	@ set item to 0 and clear it
	ldr r0, =gActiveUnit
	ldr r0, [r0]
	ldrb r1, [r4, #0x12] @ item slot used
	lsl r1, #1 @ x2
	add r1, #0x1E @ items
	mov r2, #0
	strh r2, [r0, r1]

	blh RemoveUnitBlankItems
	
	DelayedTonicDeletion_End:
	pop {r4}
	pop {r0}
	bx r0
	
	.pool
	
	.align

ClearTonicDebuffsHook:

	@ Hook to A4354

	blh ClearTonicDebuffs

	@ vanilla code
	blh 0x80A42BC+1
	blh 0x80A429C+1

	ldr r3, =0x80A435C+1
	bx r3

	.pool

	.align
