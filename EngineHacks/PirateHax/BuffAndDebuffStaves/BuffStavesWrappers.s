.thumb
.align

.global BarrierStaffTargetingWrapper
.type BarrierStaffTargetingWrapper, %function

.global FortuneUsabilityWrapper
.type FortuneUsabilityWrapper, %function

.global FortuneStaffTargetingWrapper
.type FortuneStaffTargetingWrapper, %function

.global FortuneStaffEffectWrapper
.type FortuneStaffEffectWrapper, %function

.global ProtectUsabilityWrapper
.type ProtectUsabilityWrapper, %function

.global ProtectStaffTargetingWrapper
.type ProtectStaffTargetingWrapper, %function

.global ProtectStaffEffectWrapper
.type ProtectStaffEffectWrapper, %function

.global ConcentrateUsabilityWrapper
.type ConcentrateUsabilityWrapper, %function

.global ConcentrateStaffTargetingWrapper
.type ConcentrateStaffTargetingWrapper, %function

.global ConcentrateStaffEffectWrapper
.type ConcentrateStaffEffectWrapper, %function

FortuneUsabilityWrapper:
mov r0,r4
mov r1,r5
bl FortuneUsability
pop {r4,r5}
pop {r1}
bx r1

.ltorg
.align

FortuneStaffTargetingWrapper:
mov r0, r5
mov r2, r4
bl FortuneStaffTargeting
pop {r4-r5}
pop {r0}
bx r0

.ltorg
.align

FortuneStaffEffectWrapper:
mov r0,r6
bl ExecFortuneStaff
ldr r0,=0x802FF77
bx r0

.ltorg
.align

ProtectUsabilityWrapper:
mov r0,r4
mov r1,r5
bl ProtectUsability
pop {r4,r5}
pop {r1}
bx r1

.ltorg
.align

ProtectStaffTargetingWrapper:
mov r0, r5
mov r2, r4
bl ProtectStaffTargeting
pop {r4-r5}
pop {r0}
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

ConcentrateUsabilityWrapper:
mov r0,r4
mov r1,r5
bl ConcentrateUsability
pop {r4,r5}
pop {r1}
bx r1

.ltorg
.align

ConcentrateStaffTargetingWrapper:
mov r0, r5
mov r2, r4
bl ConcentrateStaffTargeting
pop {r4-r5}
pop {r0}
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

BarrierStaffTargetingWrapper:
mov r0, r5
mov r2, r4
bl BarrierStaffTargeting
pop {r4-r5}
pop {r0}
bx r0

.ltorg
.align