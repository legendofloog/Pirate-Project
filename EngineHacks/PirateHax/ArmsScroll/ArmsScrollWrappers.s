.thumb
.align

.global ArmsScrollUsabilityWrapper
.type ArmsScrollUsabilityWrapper, %function

.global ArmsScrollPrepUsabilityWrapper
.type ArmsScrollPrepUsabilityWrapper, %function

.global ArmsScrollEffectWrapper
.type ArmsScrollEffectWrapper, %function

.global ArmsScrollPrepEffectWrapper
.type ArmsScrollPrepEffectWrapper, %function

ArmsScrollUsabilityWrapper: @ r4 - unit, r5 - item
mov r0,r4
mov r1,r5
bl ArmsScrollUsability
pop {r4,r5}
pop {r1}
bx r1

.ltorg
.align

ArmsScrollPrepUsabilityWrapper:	@ r4 - item, r5 - unit, why is it flipped??
mov r0,r5
mov r1,r4		@ pass in as unit, item
bl ArmsScrollPrepUsability
pop {r4,r5}
pop {r1}
bx r1

.ltorg
.align

ArmsScrollEffectWrapper: @ r4 - action struct, r6 - proc pointer
mov r0,r6
bl ArmsScrollEffect
ldr r0,=0x802FF77		@ proc blocking I think
bx r0

.ltorg
.align

ArmsScrollPrepEffectWrapper: @ r4 - unit pointer, r6 - item used, r7 - item slot
mov r0,r4
mov r1,r7
bl DoArmsScrollEffect

ldr r0, =ArmsScrollPopupTextWordLink
ldr r0, [r0]

pop {r4-r7}
pop {r1}
bx r1
	
.ltorg
.align




