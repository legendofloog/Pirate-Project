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

	.equ HangoverID, SkillTester+4

	.global SUD_SaveTonic
	.type   SUD_SaveTonic, function

	.global SUD_LoadTonic
	.type   SUD_LoadTonic, function

	.global GetTonicByte
	.type 	GetTonicByte, function

	.global SetTonicByte
	.type 	SetTonicByte, function

	.global CheckTonicBit
	.type 	CheckTonicBit, function

	.global CheckIfAnyTonic
	.type 	CheckIfAnyTonic, function

	.global SetTonicBit
	.type 	SetTonicBit, function

	.global GetTonicBitFromDurability
	.type 	GetTonicBitFromDurability, function

	.global GetTonicBitFromStat
	.type 	GetTonicBitFromStat, function

	.global AddTonicBonus
	.type 	AddTonicBonus, function

	.global TonicEffectFunc
	.type 	TonicEffectFunc, function

	.global TonicUsabilityFunc
	.type 	TonicUsabilityFunc, function

	.global TonicEffectLadder
	.type 	TonicEffectLadder, function

	.global TonicUsabilityLadder
	.type 	TonicUsabilityLadder, function

	.global ClearTonicTable
	.type ClearTonicTable, function

	.global ClearTonicTableHook
	.type ClearTonicTableHook, function

	.global TonicPrepScreenUsabilityFunc
	.type 	TonicPrepScreenUsabilityFunc, function

	.global TonicPrepScreenEffectFunc
	.type 	TonicPrepScreenEffectFunc, function

	.global DelayedTonicDeletion
	.type DelayedTonicDeletion, %function

	.macro blh to, reg = r3
    ldr \reg, =\to
    mov lr, \reg
    .short 0xF800
	.endm
.global SUD_SaveTonic
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

GetTonicByte:
	@ r0 - unit address
	@ ret r0 - tonic byte

	push {lr}

	ldr r1, =TonicTablePointer
	ldr r1, [r1]
	ldr r1, [r1]

	ldrb r0, [r0, #0xB]

	add r0, r1
	ldrb r0, [r0]

	pop {r1}
	bx r1

	.pool

	.align

SetTonicByte:
	@ r0 - unit address
	@ r1 - tonic byte

	push {lr}
	push {r4}

	mov r4, r1

	ldr r1, =TonicTablePointer
	ldr r1, [r1]
	ldr r1, [r1]

	ldrb r0, [r0, #0xB]

	add r0, r1
	strb r4, [r0]

	pop {r4}
	pop {r0}
	bx r0

	.pool

	.align

CheckIfAnyTonic:
	@ r0 - unit address
	
	push {lr}
	push {r4-r6}

	mov r4, r0	@ store unit here
	bl GetTonicByte @ this gets their tonic byte and stores it in r0
	mov r5, r0	@ stores their tonic byte here
	mov r6, #1	@ (HP tonic bit starts at 1); looping variable
	
	LoopPoint:
	mov r0, r5	@ moves tonic byte back into r0 then ands it with specific bit we're looking for
	and r0, r6
	cmp r0, r5
	beq checkTonicBitRetTrueTwo @ jumps straight to true if it checks out

	mov r2, #1
	lsl r2, #MaxTonicIndex
	cmp r6, r2
	beq checkTonicBitRetFalseTwo
	
	lsl r6, #1	@ if we haven't hit the max, then we can keep looping
	b LoopPoint

	checkTonicBitRetFalseTwo:
	mov r0, #0
	b checkTonicBitEndTwo


	checkTonicBitRetTrueTwo:
	mov r0, #1

	checkTonicBitEndTwo:
	pop {r4-r6}
	pop {r1}
	bx r1

CheckTonicBit:
	@ r0 - unit address
	@ r1 - bit to check
	@ ret r0 - true if set

	push {lr}
	push {r4,r5}

	mov r4, r0
	mov r5, r1

	bl GetTonicByte

	mov r1, r5
	and r0, r1
	cmp r0, #0
	beq checkTonicBitRetFalse

	mov r0, #1
	b checkTonicBitEnd

	checkTonicBitRetFalse:
	mov r0, #0

	checkTonicBitEnd:
	pop {r4,r5}
	pop {r1}
	bx r1

	.pool

	.align

SetTonicBit:
	@ r0 - unit address
	@ r1 - bit to set

	push {lr}
	push {r4,r5}

	mov r4, r0
	mov r5, r1

	bl GetTonicByte

	mov r1, r5
	orr r1, r0

	mov r0, r4

	bl SetTonicByte

	pop {r4,r5}
	pop {r0}
	bx r0

	.pool

	.align

GetTonicBitFromDurability:
	@ r0 - durability
	@ ret r0 - tonic bit

	push {lr}

	cmp r0, #1
	ble getTonicBitFromDurabilityEnd

	sub r0, #1
	mov r1, r0
	mov r0, #1
	lsl r0, r1

	getTonicBitFromDurabilityEnd:
	
	pop {r1}
	bx r1

	.pool

	.align

GetTonicBitFromStat:
	@ r0 - stat
	@ ret r0 - tonic bit

	push {lr}

	cmp r0, #9 @ mag
	bne getTonicBitFromStat
	mov r0, #7

	getTonicBitFromStat:
	mov r1, r0
	mov r0, #1
	lsl r0, r1
	
	pop {r1}
	bx r1

	.pool

	.align

AddTonicBonus:
	@ r0 - unit pointer
	@ r1 - stat
	@ ret r0 - stat bonus

	push {lr}
	push {r4-r6}

	mov r4, r0
	mov r5, r1

	mov r0, r1
	bl GetTonicBitFromStat

	mov r1, r0
	mov r0, r4
	bl CheckTonicBit
	cmp r0, #0
	beq addTonicBonusEnd

	cmp r5, #0x0
	beq addTonicHPBonus

	cmp r5, #0x5	@ res
	beq addTonicHPBonus
	
	cmp r5, #0x4 	@ def
	beq addBadStatBonus	

	cmp r5, #0x6	@ luck
	beq addBadStatBonus

	ldr r0, =TonicStatBonusPointer
	ldr r0, [r0]
	ldr r0, [r0]
	b addTonicBonusEnd

	addBadStatBonus:
	ldr r0, =TonicBadStatBonusPointer
	ldr r0, [r0]
	ldr r0, [r0]
	b addTonicBonusEnd

	addTonicHPBonus:
	ldr r0, =TonicHPBonusPointer
	ldr r0, [r0]
	ldr r0, [r0]

	addTonicBonusEnd:
	@has Hangover
	mov r6, r0	@ puts the tonic bonus in r0 in r6 for safekeeping
	ldr r0, SkillTester
	mov lr, r0
	mov r0, r4 @Attacker data
	ldr r1, HangoverID
	.short 0xf800
	cmp r0, #0
	beq End

	mov r2, #3
	mul r6, r2	@ multiplies the bonus by 3
	
	@ if hangover is true for the unit, triple their stat
	End:
	mov r0, r6 	@ moves r0 back into r6
	pop {r4-r6}
	pop {r1}
	bx r1

	.pool

	.align

TonicEffectFunc:
	@ r4 - action data

	push {lr}

	ldr r0, =gActiveUnit
	ldr r0, [r0]

	ldrb r1, [r4, #0x12] @ item slot used
	lsl r1, #1 @ x2
	add r1, #1 @ durability
	add r1, #0x1E @ items
	ldrb r0, [r0, r1]

	cmp r0, #9
	beq rainbowTonicEffect

	bl GetTonicBitFromDurability
	mov r1, r0

	ldr r0, =gActiveUnit
	ldr r0, [r0]
	bl SetTonicBit
	b tonicEffectEnd

	rainbowTonicEffect:
	ldr r0, =gActiveUnit
	ldr r0, [r0]
	mov r1, #0xFF
	bl SetTonicByte

	tonicEffectEnd:

	@ set item to 0 and clear it - relocated to be done later to allow for some other checks to work
	@ldr r0, =gActiveUnit
	@ldr r0, [r0]
	@ldrb r1, [r4, #0x12] @ item slot used
	@lsl r1, #1 @ x2
	@add r1, #0x1E @ items
	@mov r2, #0
	@strh r2, [r0, r1]

	blh RemoveUnitBlankItems
	
	mov r0,#0x1A
	strb r0,[r4,#0x11] @set action to use item

	pop {r0}
	bx r0

	.pool

	.align
	
	
DelayedTonicDeletion:
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


TonicUsabilityFunc:
	@ r4 - active unit
	@ ret r0 - true if can be used

	push {lr}

	mov r0, r4
	ldr r2, =gActionData

	ldrb r1, [r2, #0x12] @ item slot used
	lsl r1, #1 @ x2
	add r1, #1 @ durability
	add r1, #0x1E @ items
	ldrb r0, [r0, r1]

	cmp r0, #9
	beq tonicUsabilityRainbowTonic

	bl GetTonicBitFromDurability
	mov r1, r0

	mov r0, r4
	bl CheckTonicBit
	cmp r0, #0
	bne tonicUsabilityRetFalse
	mov r0, #1
	b tonicUsabilityEnd

	tonicUsabilityRainbowTonic:
	mov r0, r4
	bl GetTonicByte
	cmp r0, #0xFF
	beq tonicUsabilityRetFalse
	mov r0, #1
	b tonicUsabilityEnd

	tonicUsabilityRetFalse:
	mov r0, #0

	tonicUsabilityEnd:

	pop {r1}
	bx r1

	.pool

	.align

TonicEffectLadder:

	mov r0, r6

	bl TonicEffectFunc

	ldr r3, =0x802FF76 + 1
	bx r3

	.pool

	.align

TonicUsabilityLadder:

	mov r0, r4

	bl TonicUsabilityFunc

	ldr r3, =0x8028BFE + 1
	bx r3

	.pool

	.align

ClearTonicTable:

	push {lr}

	ldr r0, =gChapterData
	ldrb r0, [r0, #0xE] @ chapter id

	ldr r1, =TonicChapterExclusionTablePointer
	ldr r1, [r1]

	clearTonicTableLoop:
	ldrb r2, [r1]
	cmp r0, r2
	beq clearTonicTableEnd
	add r1, #1
	cmp r2, #0xFF
	bne clearTonicTableLoop

	ldr r0, =TonicTablePointer
	ldr r0, [r0]
	ldr r0, [r0]

	mov r1, #0

	ldr r2, =TonicTableSizePointer
	ldr r2, [r2]
	ldr r2, [r2]

	blh Memset

	clearTonicTableEnd:

	pop {r0}
	bx r0

	.pool

	.align

ClearTonicTableHook:

	@ Hook to A4354

	bl ClearTonicTable

	@ vanilla code
	blh 0x80A42BC+1
	blh 0x80A429C+1

	ldr r3, =0x80A435C+1
	bx r3

	.pool

	.align

TonicPrepScreenUsabilityFunc:
	@ r4 - item and uses
	@ r5 - unit pointer
	@ ret r0 - true if can be used

	lsr r4, #8 @ get top byte
	cmp r4, #9
	beq tonicPrepScreenUsabilityRainbowTonic

	mov r0, r4
	bl GetTonicBitFromDurability
	mov r1, r0

	mov r0, r5
	bl CheckTonicBit
	cmp r0, #0
	bne tonicPrepScreenUsabilityRetFalse
	mov r0, #1
	b tonicPrepScreenUsabilityEnd

	tonicPrepScreenUsabilityRainbowTonic:
	mov r0, r5
	bl GetTonicByte
	cmp r0, #0xFF
	beq tonicPrepScreenUsabilityRetFalse
	mov r0, #1
	b tonicPrepScreenUsabilityEnd

	tonicPrepScreenUsabilityRetFalse:
	mov r0, #0

	tonicPrepScreenUsabilityEnd:

	@ we didnt push but this is necessary apparently?
	pop {r4-r5}
	pop {r1}
	bx r1

	.pool

	.align

TonicPrepScreenEffectFunc:
	@ r4 - unit pointer
	@ r6 - item and uses
	@ r7 - item slot
	@ ret r0 - text to display

	lsr r6, #8 @ get top byte
	cmp r6, #9
	beq tonicPrepScreenEffectRainbowTonic

	mov r0, r6
	bl GetTonicBitFromDurability
	mov r1, r0

	mov r0, r4
	bl SetTonicBit
	b tonicPrepScreenEffectEnd

	tonicPrepScreenEffectRainbowTonic:
	mov r0, r4
	mov r1, #0xFF
	bl SetTonicByte

	tonicPrepScreenEffectEnd:

	@ set item to 0 and clear it
	mov r0, r4
	mov r1, r7
	lsl r1, #1 @ x2
	add r1, #0x1E @ items
	mov r2, #0
	strh r2, [r0, r1]

	blh RemoveUnitBlankItems
	
	mov r3, #1
	push {r3}

	@ check here if hangover, then triple the bonus
	ldr r0, SkillTester
	mov lr, r0
	mov r0, r4 @Attacker data
	ldr r1, HangoverID
	.short 0xf800
	pop {r3}
	cmp r0, #0
	beq SkipSetting

	mov r3, #3

	SkipSetting:
	@ restore hp to max
	mov r0, r4
	ldrb r1, [r0, #0x13] @ current hp
	ldr r2, =TonicHPBonusPointer
	ldr r2, [r2]
	ldr r2, [r2]
	mul r2, r3	@ hangover multiplier
	add r1, r2
	strb r1, [r0, #0x13]

	ldr r0, =TonicUseBoxTextPointer
	ldr r0, [r0]
	ldr r0, [r0]

	@ we didnt push but this is necessary apparently?
	pop {r4-r7}
	pop {r1}
	bx r1

	.pool

	.align

SkillTester:
@Poin SkillTester
@WORD ArcaneBladeID
