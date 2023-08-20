.thumb
.equ EqualizerID, SkillTester+4
.equ AuraSkillCheck, SkillTester+8

push {r4-r7, lr}
mov r4, r0 @attacker
mov r5, r1 @defender

@first we check if attacker has skill
ldr r0, SkillTester
mov lr, r0
mov r0, r4 @attacker
ldr r1, EqualizerID
.short 0xf800
cmp r0, #0
bne LevelCheck @usually beq End but we want to jump straight to the level conditional if unit has skill

@if attacker doesn't have skill, check if they're within 2 tiles of a unit that does
ldr r0, AuraSkillCheck
mov lr, r0
mov r0, r4
ldr r1, EqualizerID
mov r2, #0 @aura targets are of same allegiance
mov r3, #2 @max aura range
.short 0xf800
cmp r0, #0
beq End @now we beq End

@this is where the fun begins
LevelCheck:
mov r0, #0x08
ldrb r1, [r4,r0] @load attacker's level into r1
ldrb r2, [r5,r0] @load defender's level into r2
cmp r1, r2
bgt End @if attacker's level > defender's level, don't do anything

@add difference to attacker's damage
mov r0, #0x5A
sub r2, r1 @how much higher defender's level > attacker's level
ldrh r1, [r4,r0]
add r1, r2
strh r1, [r4,r0] @add that difference to damage

End:
pop {r4-r7, r15}
.align
.ltorg
SkillTester:
@POIN SkillTester
@WORD EqualizerID
@POIN NewAuraSkillCheck
