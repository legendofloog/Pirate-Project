.thumb
.equ PanacheID, SkillTester+4

.equ gBattleData, 0x203A4D4

push {r4-r7, lr}
mov r4, r0 @atkr
mov r5, r1 @dfdr

@has ArcaneBlade
ldr r0, SkillTester
mov lr, r0
mov r0, r4 @Attacker data
ldr r1, PanacheID
.short 0xf800
cmp r0, #0
beq End

ldr     r5,=0x203a4ec @attacker
cmp     r4,r5
bne     End @skip if unit isn't the attacker

@make sure we're in combat (or combat prep)
ldrb	r3, =gBattleData
ldrb	r3, [r3]
cmp		r3, #4
beq		End

@check range
ldr r0,=#0x203A4D4 @battle stats
ldrb r0,[r0,#2] @range
cmp r0,#1
bne End

@store magic in r6
mov		r1,#0x3A
ldrb	r6,[r4,r1]

@increase AS by magic
mov r1, #0x5E
ldrh r0, [r4, r1]
add r0, r6
strh r0, [r4,r1]

@increase crit by 17
mov r1, #0x66
ldrh r0, [r4, r1]
add r0, #17
strh r0, [r4,r1]

End:
pop {r4-r7, r15}
.align
.ltorg
SkillTester:
@Poin SkillTester
@WORD PanacheID
