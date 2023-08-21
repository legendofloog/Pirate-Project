.thumb
.align

.equ FuseID,SkillTester+4

push {r4-r7,lr}
@goes in the battle loop.
@r0 is the attacker
@r1 is the defender
mov r4, r0
mov r5, r1

@check for skill
ldr r0, SkillTester
mov lr, r0
mov r0, r4
ldr r1, FuseID
.short 0xf800
cmp r0, #0
beq GoBack

@set defender AS to 0
mov r0, r5
add r0,#0x5e
mov r3,#0
strh r3,[r0]

@check range
ldr r0,=#0x203A4D4 @battle stats
ldrb r0,[r0,#2] @range
cmp r0,#1
bgt SetMiss

@check weapon type
mov     r0, #0x50      @Move to the attacking unit weapon type.
ldrb    r0, [r5, r0]   @Load the attacking unit weapon type.
cmp     r0, #0         @Sword?
beq     SetDamage
cmp     r0, #1         @Lance?
beq     SetDamage
cmp     r0, #2         @Axe?
beq     SetDamage
cmp     r0, #6         @Knife?
beq     SetDamage

SetMiss:
@set defender hit and atk to 0
mov r0, r5
add r0,#0x60
mov r3,#0
strh r3,[r0]

mov r0, r5
add r0,#0x5a
strh r3,[r0]
b GoBack

SetDamage:
@set defender hit to 100 and atk to 15
mov r0, r5
add r0,#0x60
mov r3,#100
strh r3,[r0]

mov r0, r5
add r0,#0x5a
mov r3,#15
strh r3,[r0]

GoBack:
pop {r4-r7}
pop {r0}
bx r0

.ltorg
.align

SkillTester:
@POIN SkillTester
@WORD FuseID
