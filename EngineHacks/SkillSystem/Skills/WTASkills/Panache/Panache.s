.thumb
.equ PanacheID, SkillTester+4

.equ gBattleData, 0x203A4D4

push {r4-r7, lr}
mov r4, r0 @attacker
mov r5, r1 @defender

@check skill on attacker
ldr r0, SkillTester
mov lr, r0
mov r0, r4 @attacker data
ldr r1, PanacheID
.short 0xf800
cmp r0, #0x0
beq End

@make sure we're in combat (or combat prep)
ldr	r3, =gBattleData
ldrb	r3, [r3]
cmp		r3, #4
beq		End

@check range
ldr r0,=#0x203A4D4 @battle stats
ldrh r0,[r0,#2] @range
cmp r0,#1
bne End

@add WTA for attacker
mov r0,r4
add r0,#0x53
mov r1,#20
strb r1,[r0]
add r0,#1
mov r1,#2
strb r1,[r0]

@add WTD for defender
mov r0,r5
add r0,#0x53
mov r1,#20
neg r1,r1
strb r1,[r0]
add r0,#1
mov r1,#2
neg r1,r1
strb r1,[r0]

End:
pop {r4-r7, r15}
.align
.ltorg
SkillTester:
@Poin SkillTester
@WORD NonconformingID
