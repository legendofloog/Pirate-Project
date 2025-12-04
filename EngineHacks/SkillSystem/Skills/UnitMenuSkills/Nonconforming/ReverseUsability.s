.equ CheckEventId,0x8083da8

.equ NonconformingID, SkillTester+4
.thumb
.org 0
push {r4-r7,lr}
ldr r0,=0x3004e50
ldr r4,[r0] @save active unit in r4
ldr r1,[r4,#0xc]
mov r0, #0x40 @has not moved...
and r0,r1
cmp r0,#0
bne False

@check if active unit has the skill
mov r0, r4 @test
ldr r1, NonconformingID
ldr r2, SkillTester
mov lr, r2
.short 0xf800 @test if unit has the skill
cmp r0, #0
beq False

ldr r0,=ReverseFlagLink	@ this is the Reverse Usability, so we want this flag to NOT be set to do it
ldrh r0,[r0]
ldr r1,=CheckEventId
mov r14,r1
.short 0xF800
cmp r0,#1
beq False

True:
mov r0,#1
b End

False:
mov r0,#3
End:
pop {r4-r7}
pop {r1}
bx r1

.align
.ltorg
SkillTester:
@POIN SkillTester
@WORD ShiftID
