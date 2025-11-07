.thumb

.macro blh to, reg=r3
  ldr \reg, =\to
  mov lr, \reg
  .short 0xf800
.endm

@r0=char data ptr

push	{r4-r5,r14}
mov		r4,r0
ldrb 		r1, [r4, #0x0B]
mov		r0, #0xC0
and		r1, r0
cmp 		r1, #0x0 
beq 		RetTrue 			@ if the unit is an ally, return true; else, don't

RetFalse:
mov		r0,#0
b		GoBack

RetTrue:
ldr 		r1, [r4, #0x4]
ldr 		r1, [r1, #0x28]			@ gets unit's class attributes
mov 		r0, #0x1 			@ mounted aid attribute
and		r1, r0
cmp		r1, #0x1
beq		RetFalse			@ if the unit has it, they cannot stealE

mov		r0,#1				@ passes in true if they do not have this and are a player

GoBack:
pop		{r4-r5}
pop		{r1}
cmp		r0,#0				@necessary due to laziness and space constraints
bx		r1

.align
.ltorg
SkillTester:
