@Suppressive Fire: Allies ignore foe's avoid when foe is in unit's attacking range.
.equ SuppressiveFireID, SkillTester+4
.equ AuraSkillCheck, SkillTester+8
.equ GetUnitsInRange, SkillTester+12
.thumb
push {r4-r7,lr}
@goes in the battle loop.
@r0 is the attacker
@r1 is the defender
mov r4, r0
mov r5, r1

@not at stat screen
ldr r1, [r5,#4] @check defender's class
cmp r1, #0 @if 0, this is stat screen
beq End

@okay this is unintuitive but since lua herself can't get the skill effect we jump to the end if any combatant has the skill
ldr r0, SkillTester
mov lr, r0
mov r0, r4 @attacker
ldr r1, SuppressiveFireID
.short 0xf800
cmp r0, #0
bne End @usually beq but we want to skip if unit has skill

@if there's a more eloquent way to do this it's beyond me
ldr r0, SkillTester
mov lr, r0
mov r0, r5 @usually r4 for attacker but we want to check defender
ldr r1, SuppressiveFireID
.short 0xf800
cmp r0, #0
bne End @usually beq but we want to skip if unit has skill

@now we need to get lua's attack range to pass to the aura skill check
mov r0, #0x1 @disgusting hardcoded unit id, should be replaced later
ldr r3, =0x0801829D @returns unit struct in r0
mov lr, r3
.short 0xF800

mov r1, #0x1e @first item in unit's inventory
ldrb r0, [r0,r1]
ldr r3, =0x08017684 @max range getter
mov lr, r3
.short 0xF800

cmp r0, #8
blt CheckAura		

	mov r0, #8	@ if the range is higher than 8, then set max to 8
	
CheckAura:
push {r0} @preserve the range cause we need it for later

@now we check enemies in skill holder's attack range
ldr r0, AuraSkillCheck
mov lr, r0
mov r0, r4
ldr r1, SuppressiveFireID
mov r2, #3 @are enemies
pop {r3} @put range back here for max aura radius
.short 0xf800
cmp r0, #0
beq End

@set attacker's avoid to half of what it was
mov r0, #0x62
ldrh r1, [r4,r0]
lsr r1, #0x1	@ divides avoid by 2
strh r1, [r4,r0]

End:
pop {r4-r7}
pop {r0}
bx r0
.align
.ltorg
SkillTester:
@ POIN SkillTester
@ WORD SuppressiveFireID
@ POIN AuraSkillCheck
@ POIN GetUnitsInRange
