.thumb
.align

.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm


.global MapAnim_BeginRoundSpecificAnims
.type MapAnim_BeginRoundSpecificAnims, %function

MapAnim_BeginRoundSpecificAnims: @ 0x0808161C
	push {r4, r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r1, gMapAnimData @ gCurrentMapAnimState
	add r0, r1, #0
	add r0, #0x58
	ldrb r0, [r0]	@ loads u8 at 0x58 in MapAnimState struct, the acting unit id
	mov r10, r0	@ moves it into r10?
	add r0, r1, #0
	add r0, #0x5a
	ldrh r2, [r0]	@ loads in r2, u16 at 0x5A in same struct, hit attributes
	mov r0, #0x80
	and r0, r2	@ sees if hit attributes and with 0x80 (is this a devil hit?)
	add r7, r1, #0	@ moves current map anim state into r7
	cmp r0, #0
	beq _0808164C	@ if it does not AND, skip the following part where it loads the acting unit id into r5
	mov r5, r10
	b _08081652
_0808164C:
	add r0, r7, #0
	add r0, #0x59	
	ldrb r5, [r0] @ loads target ID into r5
_08081652:
	add r6, r7, #0		@ moves current map anim state into r6
	mov r0, r10
	lsl r4, r0, #2
	add r0, r4, r0
	lsl r0, r0, #2
	add r1, r6, #4
	add r0, r0, r1
	ldr r0, [r0]
	add r0, #0x4a
	ldrh r0, [r0]		@ gets weapon before (0x4A) of unit?
	blh GetSpellAssocReturnBool
	lsl r0, r0, #0x18
	str r4, [sp]
	cmp r0, #0
	bne _08081692
	add r0, r6, #0
	add r0, #0x5a
	ldrh r1, [r0]
	mov r0, #2
	and r0, r1
	cmp r0, #0
	bne _08081682
	b _080818C4		@ goes to end of func
_08081682:
	lsl r0, r5, #2
	add r0, r0, r5
	lsl r0, r0, #2
	add r0, r0, r6
	ldr r0, [r0]		@ somehow getting the Unit pointer of the target and drawing miss anim on them
	blh MapAnim_BeginMISSAnim
	b _080818C4		@ goes to end of func
_08081692:
	add r1, r6, #0
	add r1, #0x5f
	mov r0, #1
	strb r0, [r1]
	ldr r0, [r6]
	add r0, #0x30
	ldrb r1, [r0]
	mov r0, #0xf
	and r0, r1
	cmp r0, #0xa
	bne _080816BA
	add r0, r6, #0
	add r0, #0x5d
	mov r1, #0
	ldrsb r1, [r0, r1]
	neg r1, r1
	mov r0, r10
	blh 0x080818D8
	b _080816C8
_080816BA:
	add r0, r7, #0
	add r0, #0x5d
	mov r1, #0
	ldrsb r1, [r0, r1]
	add r0, r5, #0
	blh 0x080818D8
_080816C8:
	ldr r6, gMapAnimData  @ gCurrentMapAnimState
	add r4, r6, #0
	add r4, #0x5a
	ldrh r1, [r4]
	mov r0, #0x80
	lsl r0, r0, #1
	and r0, r1
	cmp r0, #0
	beq _080816EA
	add r0, r6, #0
	add r0, #0x5d
	mov r1, #0
	ldrsb r1, [r0, r1]
	neg r1, r1
	mov r0, r10
	blh 0x080818D8
_080816EA:
	add r0, r6, #0
	add r0, #0x5d
	mov r2, #0
	ldrsb r2, [r0, r2]
	cmp r2, #0
	bge _080816F8
	b _080818C4
_080816F8:
	ldrh r1, [r4]
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _08081730
	lsl r4, r5, #2
	add r4, r4, r5
	lsl r4, r4, #2
	add r4, r4, r6
	ldr r0, [r4]
	mov r1, #0x10
	ldrsb r1, [r0, r1]
	lsl r1, r1, #4
	ldr r0, gGameState  @ gGameState
	mov r2, #0xc
	ldrsh r0, [r0, r2]
	sub r1, r1, r0
	mov r0, #0xc8
	blh PlaySpacialSoundMaybe
	ldr r0, [r4]
	blh MapAnim_BeginMISSAnim
	b _080818C4
_08081730:
	cmp r2, #0
	bne _08081770
	mov r0, #0x80
	lsl r0, r0, #6
	and r0, r1
	cmp r0, #0
	beq _08081740
	b _080818C4
_08081740:
	ldr r0, _08081768  @ 0x000002CE
	lsl r4, r5, #2
	add r4, r4, r5
	lsl r4, r4, #2
	add r4, r4, r6
	ldr r1, [r4]
	ldrb r1, [r1, #0x10]
	lsl r1, r1, #0x18
	asr r1, r1, #0x18
	lsl r1, r1, #4
	ldr r2, gGameState  @ gGameState
	mov r3, #0xc
	ldrsh r2, [r2, r3]
	sub r1, r1, r2
	blh PlaySpacialSoundMaybe
	ldr r0, [r4]
	blh MapAnim_BeginNODAMAGEAnim
	b _080818C4
	.align 2, 0
_08081768: .4byte 0x000002CE
_08081770:
	mov r3, #0
	lsl r2, r5, #2
	add r0, r2, r5
	lsl r0, r0, #2
	add r1, r6, #4
	add r0, r0, r1
	ldr r0, [r0]
	add r0, #0x55
	ldrb r0, [r0]
	add r4, r2, #0
	cmp r0, #0x1b
	beq _0808178C
	cmp r0, #0x33
	bne _0808178E
_0808178C:
	mov r3, #1
_0808178E:
	cmp r3, #0
	beq _080817CA
	ldr r2, gMapAnimData  @ gCurrentMapAnimState
	add r0, r2, #0
	add r0, #0x5c
	ldrb r1, [r0]
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _080817B8
	mov r6, #0xaf
	add r0, r4, r5
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #1
	blh MapAnim_BeginWallBreakAnim
	b NoBreak
_080817B8:
	mov r6, #0xb0
	add r0, r4, r5
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0
	blh MapAnim_BeginWallBreakAnim
	b NoBreak
_080817CA:
	ldr r2, gMapAnimData  @ gCurrentMapAnimState
	ldr r0, [r2]
	add r0, #0x30
	ldrb r1, [r0]
	mov r0, #0xf
	and r0, r1
	cmp r0, #0xa
	bne _080817E8
	ldr r6, _080817E4  @ 0x000003C9
	b NoBreak
	.align 2, 0
_080817E4: .4byte 0x000003C9
_080817E8:
	add r0, r2, #0
	add r0, #0x5c
	ldrb r1, [r0]
	mov r0, #2
	and r0, r1
	mov r6, #0xd2	@ if the battle is not a kill, play song id 0xd2 (hit)
	cmp r0, #0
	beq _080817FA
	mov r6, #0xd5   @ if the battle is a kill, play song id 0xd5 (kill)
_080817FA:		@ NEW CODE BEGINS HERE
	ldr r7, gMapAnimData @ gCurrentMapAnimState
	add r0, r7, #0
	add r0, #0x5a
	ldrh r1, [r0]
	mov r0, #1
	lsl r0, #12		
	and r0, r1	
	cmp r0, #0	@ checks if it ands with 1 << 17, an unused hit attribute now used for Break
	bne BreakTime
	b NoBreak
BreakTime:
	cmp r6, #0xd2	@ is this a normal break via hit, or kill?
	beq BreakHit	@ if it isn't a kill, branch to hit stuff
	mov r6, #0x26
	lsl r6, #4		
	add r6, #0xE	@ shifts to 0x26E, where I put the break kill sound
	b BreakDisplay

BreakHit:
	mov r6, #0x26
	lsl r6, #4	@ shifts to 0x260
	add r6, #0xF	@ makes r6 0x26F, where I put the break hit sound

BreakDisplay:
	add r0, r4, r5
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r0, [r0]		@ gets the unit needed to call our Break function
	blh MapAnim_BeginBREAKAnim	@ calls the break anim and continues

NoBreak:		@ Back to vanilla code
	ldr r7, gMapAnimData  @ gCurrentMapAnimState
	add r0, r7, #0
	add r0, #0x5a
	ldrh r1, [r0]
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _08081880
	add r4, r4, r5
	lsl r4, r4, #2
	add r0, r4, r7
	mov r9, r0
	ldr r0, [r0]
	mov r1, #0x10
	ldrsb r1, [r0, r1]
	lsl r1, r1, #4
	ldr r2, gGameState  @ gGameState
	mov r8, r2
	mov r3, #0xc
	ldrsh r0, [r2, r3]
	sub r1, r1, r0
	add r0, r6, #0
	blh PlaySpacialSoundMaybe
	add r6, r7, #0
	add r6, #8
	add r4, r4, r6
	ldr r5, [r4]
	ldr r4, [sp]
	add r4, r10
	lsl r4, r4, #2
	add r0, r7, #4
	add r0, r4, r0
	ldr r0, [r0]
	add r0, #0x4a
	ldrh r0, [r0]
	blh GetSpellAssocFlashColor
	add r1, r0, #0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r0, r5, #0
	blh MU_StartCritFlash
	blh NewBG0Shaker
	mov r1, r9
	ldr r0, [r1]
	mov r1, #0x10
	ldrsb r1, [r0, r1]
	lsl r1, r1, #4
	mov r2, r8
	mov r3, #0xc
	ldrsh r0, [r2, r3]
	sub r1, r1, r0
	mov r0, #0xd8
	blh PlaySpacialSoundMaybe
	add r4, r4, r6
	ldr r0, [r4]
	blh MU_StartFastMoveAnim
	b _080818C4
_08081880:
	add r4, r4, r5
	lsl r4, r4, #2
	add r0, r4, r7
	ldr r0, [r0]
	mov r1, #0x10
	ldrsb r1, [r0, r1]
	lsl r1, r1, #4
	ldr r0, gGameState  @ gGameState
	mov r2, #0xc
	ldrsh r0, [r0, r2]
	sub r1, r1, r0
	add r0, r6, #0
	blh PlaySpacialSoundMaybe
	add r0, r7, #0
	add r0, #8
	add r4, r4, r0
	ldr r4, [r4]
	ldr r0, [sp]
	add r0, r10
	lsl r0, r0, #2
	add r1, r7, #4
	add r0, r0, r1
	ldr r0, [r0]
	add r0, #0x4a
	ldrh r0, [r0]
	blh GetSpellAssocFlashColor
	add r1, r0, #0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r0, r4, #0
	blh MU_StartHitFlash
_080818C4:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov r10, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0

.align
.ltorg
gGameState:
.long 0x0202BCB0
gMapAnimData:
.long 0x0203E1F0
@