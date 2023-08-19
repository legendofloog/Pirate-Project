.thumb
.equ CatchingUpID, SkillTester+4

push {r4-r7, lr}
mov r4, r0 @attacker
mov r5, r1 @defender

ldr r0,=0x203a4ec
cmp r0,r4
beq End

@not at stat screen
ldr r1, [r5,#4] @check defender's class
cmp r1, #0 @if 0, this is stat screen
beq End

@first we check that the attacker has Catching Up
ldr r0, SkillTester
mov lr, r0
mov r0, r4 @attacker data
ldr r1, CatchingUpID
.short 0xf800
cmp r0, #0
beq SwapAttackerDefender

@after that, we check that we're being doubled
mov r0, #0x5E
ldrh r1, [r4,r0] @load attacker's AS into r1
ldrh r2, [r5,r0] @load defender's AS into r2
add r1, #0x4
cmp r1, r2 @if we're being doubled, attacker's AS+4 <= defender's AS
bgt SwapAttackerDefender @if we're not being doubled, no boost for us

@add amount being doubled by as extra damage
mov r0, #0x5A
ldrh r3, [r4,r0] @atk
sub r2, r1 @amount doubled by = defender's AS - (attacker's AS+4)
add r3, r2
strh r3, [r4,r0]

@now we do the same thing but swap attacker and defender
SwapAttackerDefender:
ldr r0, SkillTester
mov lr, r0
mov r0, r5 @defender data
ldr r1, CatchingUpID
.short 0xf800
cmp r0, #0
beq End

@after that, we check that we're being doubled
mov r0, #0x5E
ldrh r1, [r5,r0] @load defender's AS into r1
ldrh r2, [r4,r0] @load attacker's AS into r2
add r1, #0x5
cmp r1, r2 @if we're being doubled, defender's AS+5 <= attacker's AS
bgt End @if we're not being doubled, no boost for us

@add amount above being doubled by as extra damage
mov r0, #0x5A
ldrh r3, [r5,r0] @atk
sub r2, r1 @amount above doubled by = attacker's AS - (defender's AS+5)
add r3, r2
strh r3, [r5,r0]

End:
pop {r4-r7, r15}
.align
.ltorg
SkillTester:
@Poin SkillTester
@WORD CatchingUpID
