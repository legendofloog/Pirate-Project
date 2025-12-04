@ Applies DR-Icon if DR-bit is set.

.thumb
.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm
.equ PlayerPhaseReturn, 0x80187CD 
.equ NonPlayerPhaseReturn, 0x801880D 
.equ gActionData, 0x203A958
.equ gChapterData, 0x202BCF0 
.equ GetUnit, 0x8019430
.equ IsItemStealable, 0x8017054
.equ VanillaIsItemStealableBytes, 0x1C01B500
.equ BattleMapState,				0x0202BCB0
.equ CallARM_PushToSecondaryOAM,	0x08002bb9
.equ OAMTable,						0x08590f44
.equ gMapUnit,						0x0202E4D8
.equ xMask1,						0x209
.equ yMask1,						0x100
.equ xMask2,						0x1FF
.equ yMask2,						0xFF
.equ IconID_Stealable, 				UsingDangerRadius+4
.equ IconID_DangerRadius, 			IconID_Stealable+4
.equ IconID_Droppable, 				IconID_DangerRadius+4
.equ UsingDangerRadius, StealableItemCache+4
push	{r2, r4, r14}

ldr r0, UsingDangerRadius 
cmp r0, #1 
bne Drop 

@ Check whether DR-bit is set.
mov		r0, #0x32
ldrb	r0, [r4, r0]	@ Replace with a different bit...
mov		r1,	#0x40		@ ...in unit struct, if in use.
tst		r0, r1
beq		Drop

@ Draw DR-Icon.
mov		r1, #0x10
ldsb	r1, [r4, r1]	@ X-Coordinate.
lsl		r1 ,r1 ,#0x4	@ Multiplied by 16 (tiles are 16 by 16).
ldr		r2, =BattleMapState
mov		r5, #0xc		@ dw, r5 is free.
ldsh	r0, [r2, r5]	@ gCurrentRealCameraPos, lower short (seems X related).
sub		r3 ,r1, r0		@ Subtract Xcamera pos from X-coordinate.
mov		r0, #0x11
ldsb	r0, [r4, r0]	@ Y-Coordinate.
lsl		r0 ,r0 ,#0x4	@ Multiplied by 16.
mov		r5, #0xe
ldsh	r1, [r2, r5]	@ gCurrentRealCameraPos, higher short (seems Y related).
sub		r2 ,r0, r1		@ Subtract Ycamera pos from Y-coordinate.
mov		r1 ,r3
add		r1, #0x10
mov		r0, #0x80
lsl		r0 ,r0 ,#0x1
cmp		r1 ,r0			@ Off screen, perhaps.
bhi		Drop
	mov		r0 ,r2
	add		r0, #0x10
	cmp		r0, #0xb0	@ Off screen, perhaps.
	bhi		Drop
		ldr		r5, =xMask1
		add		r0 ,r3, r5		@ X += #0x209 vanilla sets bit 9, despite...
		ldr		r1, =xMask2
		and		r0 ,r1			@ X &= #0x1FF ...this mask zeroing it.
		ldr		r3, =yMask1
		add		r1, r2, r3		@ Y += #0x10F vanilla sets bit 8, despite...
		ldr		r2, =yMask2
		and		r1 ,r2			@ Y &= #0xFF  ...this mask zeroing it.
		ldr		r2, =OAMTable
		ldr		r3, IconID_DangerRadius		@ Icon location and priority=2.
		ldr		r5, =CallARM_PushToSecondaryOAM
		bl		GOTO_R5


Drop: @ Check whether DroppableItem-bit is set.
mov		r0, #0xD
ldrb            r0, [r4, r0]
mov		r1,	#0x10
tst		r0, r1
beq		Steal

@ Draw Droppable Item Icon.
mov		r1, #0x10
ldsb	r1, [r4, r1]	@ X-Coordinate.
lsl		r1 ,r1 ,#0x4	@ Multiplied by 16 (tiles are 16 by 16).
sub            r1, #0x08
ldr		r2, =BattleMapState
mov		r5, #0xc		@ dw, r5 is free.
ldsh	r0, [r2, r5]	@ gCurrentRealCameraPos, lower short (seems X related).
sub		r3 ,r1, r0		@ Subtract Xcamera pos from X-coordinate.
mov		r0, #0x11
ldsb	r0, [r4, r0]	@ Y-Coordinate.
lsl		r0 ,r0 ,#0x4	@ Multiplied by 16.
add             r0, #0x07
mov		r4, #0xe
ldsh	r1, [r2, r4]	@ gCurrentRealCameraPos, higher short (seems Y related).
sub		r2 ,r0, r1		@ Subtract Ycamera pos from Y-coordinate.
mov		r1 ,r3
add		r1, #0x10
mov		r0, #0x80
lsl		r0 ,r0 ,#0x1
cmp		r1 ,r0			@ Off screen, perhaps.
bhi		Return
	mov		r0 ,r2
	add		r0, #0x10
	cmp		r0, #0xb0	@ Off screen, perhaps.
	bhi		Return
		ldr		r5, =xMask1
		add		r0 ,r3, r5		@ X += #0x209 vanilla sets bit 9, despite...
		ldr		r1, =xMask2
		and		r0 ,r1			@ X &= #0x1FF ...this mask zeroing it.
		ldr		r3, =yMask1
		add		r1, r2, r3		@ Y += #0x10F vanilla sets bit 8, despite...
		ldr		r2, =yMask2
		and		r1 ,r2			@ Y &= #0xFF  ...this mask zeroing it.
		ldr		r2, =OAMTable
		ldr		r3, IconID_Droppable		@ Icon location and priority=2.
		ldr		r5, =CallARM_PushToSecondaryOAM
		bl		GOTO_R5
		b Return @ don't draw stealable item icon if they have a droppable item 
Steal:
mov r0, r4 @ unit 
bl DoesUnitHaveStealableItem 
cmp r0, #0 
beq Return @ no item to steal  

@ Draw Loot Icon.
mov		r1, #0x10
ldsb	r1, [r4, r1]	@ X-Coordinate.
lsl		r1 ,r1 ,#0x4	@ Multiplied by 16 (tiles are 16 by 16).
sub            r1, #0x08
ldr		r2, =BattleMapState
mov		r5, #0xc		@ dw, r5 is free.
ldsh	r0, [r2, r5]	@ gCurrentRealCameraPos, lower short (seems X related).
sub		r3 ,r1, r0		@ Subtract Xcamera pos from X-coordinate.
mov		r0, #0x11
ldsb	r0, [r4, r0]	@ Y-Coordinate.
lsl		r0 ,r0 ,#0x4	@ Multiplied by 16.
add             r0, #0x07
mov		r4, #0xe
ldsh	r1, [r2, r4]	@ gCurrentRealCameraPos, higher short (seems Y related).
sub		r2 ,r0, r1		@ Subtract Ycamera pos from Y-coordinate.
mov		r1 ,r3
add		r1, #0x10
mov		r0, #0x80
lsl		r0 ,r0 ,#0x1
cmp		r1 ,r0			@ Off screen, perhaps.
bhi		Return
	mov		r0 ,r2
	add		r0, #0x10
	cmp		r0, #0xb0	@ Off screen, perhaps.
	bhi		Return 
		ldr		r5, =xMask1
		add		r0 ,r3, r5		@ X += #0x209 vanilla sets bit 9, despite...
		ldr		r1, =xMask2
		and		r0 ,r1			@ X &= #0x1FF ...this mask zeroing it.
		ldr		r3, =yMask1
		add		r1, r2, r3		@ Y += #0x10F vanilla sets bit 8, despite...
		ldr		r2, =yMask2
		and		r1 ,r2			@ Y &= #0xFF  ...this mask zeroing it.
		ldr		r2, =OAMTable
		ldr		r3, IconID_Stealable		@ Icon location and priority=2.
		ldr		r5, =CallARM_PushToSecondaryOAM
		bl		GOTO_R5

Return:
@ Overwritten stuff.
pop		{r2, r4}
ldr		r0, [r4, #0x4]	@ Class data.
ldr		r1, [r2, #0x28] @ Character ability 1-4.
ldr		r0, [r0, #0x28]	@ Class ability 1-4.
orr		r1 ,r0
mov		r0, #0x80		@ Boss Icon.
lsl		r0 ,r0 ,#0x8
and		r1 ,r0

pop		{r0}
bx		r0

GOTO_R5:
bx		r5



DoesUnitHaveStealableItem: 
ldrb r1, [r0, #0x0B] @ deployment byte 
lsr r2, r1, #7 @ enemies only 
cmp r2, #0 
beq False_DoesUnitHaveStealableItem
ldr r3, StealableItemCache 
mov r2, #0x3F 
and r1, r2 
lsr r2, r1, #3 @ bytes 
add r3, r2 
mov r2, #7 
and r1, r2 @ bit offset 
ldrb r2, [r3] 
mov r0, #1 
lsl r0, r1 
tst r0, r2 
bne True_DoesUnitHaveStealableItem 

False_DoesUnitHaveStealableItem: 
mov r0, #0 
b Exit_DoesUnitHaveStealableItem 
True_DoesUnitHaveStealableItem: 
mov r0, #1 
Exit_DoesUnitHaveStealableItem: 
bx lr 
.ltorg 

.global Hook_SetUpCacheForStealableItems
.type Hook_SetUpCacheForStealableItems, %function 
Hook_SetUpCacheForStealableItems: 
push {lr} 
bl SetupCacheForStealableItems

ldr r0, =gChapterData 
ldrb r0, [r0, #0xF] 
cmp r0, #0 
bne GotoNonPlayer 
pop {r3} 
mov r4, #1 
ldr r3, =PlayerPhaseReturn
bx r3 
GotoNonPlayer: 
pop {r3} 
ldr r1, =gChapterData 
ldr r3, =NonPlayerPhaseReturn
bx r3 
.ltorg 

.global Hook_RefreshCacheAfterStealing
.type Hook_RefreshCacheAfterStealing, %function 
Hook_RefreshCacheAfterStealing:
push {lr} 
ldr r0, =gActionData 
ldrb r0, [r0, #0x11] @ action 
cmp r0, #6 
bne Vanilla
bl SetupCacheForStealableItems
Vanilla: 
ldr r2, [r4] 
ldr r0, [r2] 
ldr r1, [r2, #4] 
ldr r0, [r0, #0x28] 
ldr r1, [r1, #0x28] 
orr r0, r1 
pop {r3} 
bx r3
.ltorg 


@ called each player phase 
.global SetupCacheForStealableItems
.type SetupCacheForStealableItems, %function 
SetupCacheForStealableItems:
push {r4-r7, lr} 

@ clear cache 
mov r0, #0 
ldr r3, StealableItemCache 
str r0, [r3] 
str r0, [r3, #4] 

mov r7, #1 @ true vanilla 
ldr r3, =IsItemStealable 
ldr r0, [r3] @ what bytes are here? 
ldr r1, =VanillaIsItemStealableBytes 
cmp r0, r1 
beq YesVanilla
mov r7, #0 
YesVanilla: 

mov r6, #0x7F 
LoopUnits: 
add r6, #1 
cmp r6, #0xC0 
bge BreakCache 
mov r0, r6 
blh GetUnit 
mov r4, r0 @ unit 
bl IsUnitOnField 
cmp r0, #0 
beq LoopUnits 
mov r5, #0x1C 
LoopItems: 
add r5, #2 
cmp r5, #0x28 
bge LoopUnits
ldrh r0, [r4, r5] 
cmp r0, #0 
beq LoopUnits 
@r0=unit data ptr of unit being stolen from, r1=slot number
cmp r7, #1 
beq VanillaParams
mov r0, r4 
mov r1, r5 
mov r2, #0x1E 
sub r1, r2 
lsr r1, #1 @ slot number 
VanillaParams: 
blh NewIsItemStealable_Pre
cmp r0, #1 
beq SetUnitStealableItem 
b LoopItems 
SetUnitStealableItem: 
ldrb r1, [r4, #0x0B] @ deployment byte 
ldr r3, StealableItemCache 
mov r2, #0x3F 
and r1, r2 
lsr r2, r1, #3 @ bytes 
add r3, r2 
mov r2, #7 
and r1, r2 @ bit offset 
ldrb r2, [r3] 
mov r0, #1 
lsl r0, r1 
orr r2, r0 
strb r2, [r3] 

b LoopUnits 

BreakCache: 
pop {r4-r7} 
pop {r0} 
bx r0 
.ltorg 

IsUnitOnField: 
cmp r0, #0 
beq RetFalse 
ldr r1, [r0] 
cmp r1, #0 
beq RetFalse 
ldrb r1, [r1, #4] @ unit id 
cmp r1, #0 
beq RetFalse
ldr r1, [r0, #0x0C] 
ldr r2, =0x1000C @ escaped, undeployed, dead 
tst r1, r2 
bne RetFalse
RetTrue: 
mov r0, #1 
b Exit_IsUnitOnField 
RetFalse: 
mov r0, #0 
Exit_IsUnitOnField: 
bx lr 
.ltorg 
.align 
StealableItemCache: 
@ WORD StealableItemCache 

