.thumb
.align


.global FortuneStaffEffectWrapper
.type FortuneEffectWrapper, %function

.global ProtectStaffEffectWrapper
.type ArriveCommandEffect, %function

.global ConcentrateStaffEffectWrapper
.type ConcentrateEffectWrapper, %function

FortuneStaffEffectWrapper:
mov r0,r6
bl ExecFortuneStaff
ldr r0,=0x802FF77
bx r0

.ltorg
.align

ProtectStaffEffectWrapper:
mov r0,r6
bl ExecProtectStaff
ldr r0,=0x802FF77
bx r0

.ltorg
.align

ConcentrateStaffEffectWrapper:
mov r0,r6
bl ExecConcentrateStaff
ldr r0,=0x802FF77
bx r0

.ltorg
.align