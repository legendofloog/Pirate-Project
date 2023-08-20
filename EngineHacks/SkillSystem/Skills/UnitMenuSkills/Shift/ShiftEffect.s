.thumb
.macro blh to, reg
    ldr \reg, =\to
    mov lr, \reg
    .short 0xF800
.endm

.equ GetUnit, 0x8019431
.equ gActionData, 0x203A958

push {lr}

@Get target struct
ldr  r0, =gActionData
ldrb r0, [r0, #0xC]     @r0 = gActionData.subject
blh  GetUnit, r1        @r0 = Subject unit

mov r3, #0x16
ldrb  r1, [r0,r3]
mov r3, #0x14
ldrb  r2, [r0,r3]
strb  r1, [r0,r3]
mov r3, #0x16
strb  r2, [r0,r3]

@mov r2, #0x0
@strb r2, [r0, #0x1d]     @r0 = gActionData.unitActionType

mov r0, #0x4
pop {r1}
bx r1
