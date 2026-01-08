.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm
.equ CantoAttackID, SkillTester+4
.thumb

push	{r4, lr}
@check if dead
ldrb	r0, [r4,#0x13]
cmp	r0, #0x00
beq	End

@check if attacked, otherwise do nothing
ldrb  r0, [r6,#0x11]  @action taken this turn
cmp r0, #0x2
bne End

@check if flag 0x3 set; if so, cannot canto
ldr r0,=0x8083da8 @CheckEventId
mov r14,r0
mov r0,#3
.short 0xF800
cmp r0,#1
beq End

@check if already cantoing, and is not in a ballista
ldr	r0, [r4,#0x0C]	@status bitfield
mov	r1, #0x21
lsl	r1, #0x06	@has moved already and is in a ballista
and	r0, r1
cmp	r0, #0x00
bne	End

@check for skill
HasSkill:
mov	r0, r4
ldr	r1, CantoAttackID
ldr	r3, SkillTester
mov	lr, r3
.short	0xf800
cmp	r0,#0x00
beq	End

CanCanto:
@ if this is off, only refresh unit if player 
ldr r3, =CantoAI_Label
ldr r3, [r3] 

@check if enemy or not
ldrb r2, [r4,#0x0B]
lsr r2,r2,#6
cmp r3, #1 
beq Refresh 
cmp r2, #0 
bne End @ no canto ai and not a player

Refresh: 
@check if unit already activated a Canto effect
ldr r0, [r4,#0x0C]
mov r1, #0x40
tst r0, r1
bne End

mov r0, r4 @ unit 
add r0, #0x41 
ldrb r0, [r0] 
mov r1, #0x20 
tst r0, r1 
bne End @ do not canto if cannot move 

@give unit ability to move after combat
@ldr	r0, [r4,#0x0C]	@status bitfield
mov	r1, #0x02
mvn	r1, r1
and	r0, r1		@unset bit 0x2
mov	r1, #0x40	@set canto bit
orr	r0, r1
str	r0, [r4,#0x0C]

@canto amount = unit's move - how much they moved, so we change how much they moved to (unit's move - 2).
ldr	r0,=0x8019224	@mov getter
mov	lr, r0
mov	r0, r4
.short	0xF800
sub r0, #0x2 
strb r0, [r6,#0x10]
b End @ player who has been refreshed, so no need for ai 


@ debuff movement while finding the ideal place to move to after attacking 
ldr r0, =CurrentUnit 
ldr r0, [r0] 
bl GetUnitDebuffEntry 
mov r4, r0 @ entry 
ldr r1, =DebuffStatBitOffset_Mov
ldr r1, [r1] 
ldr r2, =DebuffStatNumberOfBits_Link
ldr r2, [r2] 
bl UnpackData_Signed 
mov r5, r0 @ value 
ldrb r0, [r6, #0x10] @ squares moved this turn 
mov r3, #0 
sub r3, r0 
mov r0, r4 @ entry 
ldr r1, =DebuffStatBitOffset_Mov
ldr r1, [r1] 
ldr r2, =DebuffStatNumberOfBits_Link
ldr r2, [r2] 
bl PackData_Signed 

bl CopyAIScript11_Move_Towards_Safety @ based on current unit, should return r0 XX r1 YY coords 
ldr r3, =MemorySlot 
strh r0, [r3, #0x0B*4]
strh r1, [r3, #0x0B*4+2]

mov r0, r4 @ entry 
ldr r1, =DebuffStatBitOffset_Mov
ldr r1, [r1] 
ldr r2, =DebuffStatNumberOfBits_Link
ldr r2, [r2] 
mov r3, r5 @ value 
bl PackData_Signed 

ldr r0, =MoveActiveTo_sBCoords
mov r1, #1 @ cutscene 
blh EventEngine 

End:
pop {r4}
pop	{r0}
bx	r0
.ltorg




.equ EventEngine, 0x800D07C
.equ MemorySlot,0x30004B8
.equ AiSetDecision, 0x8039C20
.equ CurrentUnit, 0x3004E50
.equ FindSafestTileAI, 0x803B808  
@ Based on 803CDD4 
@ 803B808 FindSafestTileAI from 803CDE7
CopyAIScript11_Move_Towards_Safety: 
push {r4, lr}
sub sp, #0x10 @ allocate space 
ldr r0, =CurrentUnit 
ldr r0, [r0] 
add r4, sp, #0x0C @ stack address to save something to  ? 
mov r1, r4 
blh FindSafestTileAI @0x803B809  

@ returns true or false 
@ if false, don't do AiSetDecision 
@ at a glance, it always returned true 
@ so I don't know what criteria it returns false for 
@ maybe if it failed in some way... ? 
lsl r0, #24 
lsr r0, #24 @ bool is a byte 
@add r0, sp, #0x0C @ stack address to save something to
mov r2, #0 
ldsh r0, [r4, r2] 
mov r2, #2
ldsh r1, [r4, r2] 
@ then it stores stuff into the sp and runs AiSetDecision 
add sp, #0x10 
pop {r4}
pop {r2} 
bx r2 

.ltorg 

SetAIToWaitAtCoords:
@ given r0 = XX and r1 = YY, and r2 = ActionType, wait at coords. 
push {r4, lr}
mov r4, r2 @ Action type 
sub sp, #0xC 
@ r0 = XX 
@ r1 = YY 

@ some of this is probably unnecessary 
@ it could be trimmed down 

@ sanity check that we aren't going to try and move to [0,0] if something is empty 
ldr r2, =0x203AA96 @ AI decision +0x92 (XX) 
ldrh r2, [r2, #0x0] @ XX,YY
ldr r3, =CurrentUnit 
ldr r3, [r3] 
cmp r2, #0 
bne ContinueSetAIToWaitAtCoords
mov r1, #0x10 
ldrh r0, [r3, r1] @ Current unit coords  
ldr r2, =0x203AA96 @ AI decision +0x92 (XX) 
strh r0, [r2, #0] @ 
ldr r2, =0x203A958 @ Action struct 
mov r1, #0x13
strh r0, [r2, r1] @ Action Struct @ 203A958

ldrb r0, [r3, #0x10] @ No decision made by move towards enemy 
ldrb r1, [r3, #0x11] @ so put in our current coords 
ContinueSetAIToWaitAtCoords:

@mov r2, #0 @ Action: noop @ when they move to a coord, it had 0 here (but runs the range event in some other way I guess) 
@mov r11, r11 
@ #0x00 - noop (does not trigger range events) 
@ #0x01 - attacks target? but since I have no target it just crashes the game 
@ #0x02 - moved two tiles to the left 
@ #0x03 - gold was stolen 
@ #0x04 - the village was destroyed 
@ #0x05 - wait, I guess? it ran the range event, which is good enough for me
@ #0x07 - targets something but no crash 
@ #0x08 - targets something but no crash 
@ #0x09 - wait ? 
@ #0xFF - targets & crashes 
@ is #5 is staffwait? @ 803a204 CpPerform_StaffWait
mov r2, r4 @ Usually #0x05 (Wait), but is given as parameter in r2 to this function now 
mov r3, #0 @ store into item slot / X / Y coord 
str r3, [sp, #0] @ Item slot 
str r3, [sp, #4] @ X Coord2 (0 is fine) 
str r3, [sp, #8] @ Y Coord2 (0 is fine)
mov r3, #0 @ Target 
blh AiSetDecision, r4 @0x8039C21 
add sp, #0xC 
@ breaks once per enemy with this AI, so perfect 
@ but doesn't actually 'wait' at the spot, so it doesn't trigger range events.. 
@ so we manually trigger them now 
@ but first, let's put the active unit's coord into sB and their unit id in s2 
pop {r4} 
pop {r1}
bx r1 

.ltorg 





.align
SkillTester:
@POIN SkillTester
@WORD CantoPlusID
