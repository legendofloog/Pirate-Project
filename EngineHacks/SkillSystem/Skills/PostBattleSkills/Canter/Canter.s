.thumb
.align
.equ CanterID, SkillTester+4

push	{lr}

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

@check that unit has Canter
mov	r0, r4
ldr	r1, CanterID
ldr	r3, SkillTester
mov	lr, r3
.short	0xf800
cmp	r0, #0x00
beq	End

@check if unit already activated a Canto effect
ldr r0, [r4,#0x0C]
mov r1, #0x40
tst r0, r1
bne End

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

End:
pop	{r0}
bx	r0
.ltorg
.align
SkillTester:
@POIN SkillTester
@WORD CanterID
