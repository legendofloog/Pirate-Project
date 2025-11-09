.thumb

.include "../EarlyLeaveDefs.s"

.global EarlyLeaveTurnCheckFunc
.type EarlyLeaveTurnCheckFunc, %function

.global CallEarlyLeaveEvent
.type CallEarlyLeaveEvent, %function

.global EarlyLeaveUnitLeaves
.type EarlyLeaveUnitLeaves, %function

.global StartEarlyLeaveMenu
.type StartEarlyLeaveMenu, %function

.global InitEarlyLeaveMenuMoreInfo
.type InitEarlyLeaveMenuMoreInfo, %function

.global SetupEarlyLeaveTotalReplacement
.type SetupEarlyLeaveTotalReplacement, %function

.global UpdateEarlyLeaveMenuMoreInfo
.type UpdateEarlyLeaveMenuMoreInfo, %function

.global UpdateEarlyLeaveMenuCommandDefinition
.type UpdateEarlyLeaveMenuCommandDefinition, %function

.global StartEarlyLeaveWindow
.type StartEarlyLeaveWindow, %function

.global UpdateEarlyLeaveWindow
.type UpdateEarlyLeaveWindow, %function

.global EarlyLeaveUpdateFaceDisplay
.type EarlyLeaveUpdateFaceDisplay, %function

.global EarlyLeaveMenuDirectionInputs
.type EarlyLeaveMenuDirectionInputs, %function

.global EarlyLeaveMenuSelectInputs
.type EarlyLeaveMenuSelectInputs, %function

.global ReuseEarlyLeaveCursorLocation
.type ReuseEarlyLeaveCursorLocation, %function

.global EarlyLeaveEffect_UnitSelect
.type EarlyLeaveEffect_UnitSelect, %function

.global EarlyLeaveEffect_UnitConfirm
.type EarlyLeaveEffect_UnitConfirm, %function

.global EarlyLeaveEffect_UnitCancel
.type EarlyLeaveEffect_UnitCancel, %function

.global EarlyLeaveSetUndeploy
.type EarlyLeaveSetUndeploy, %function

.global EarlyLeaveViewStatScreen
.type EarlyLeaveViewStatScreen, %function

.global EarlyLeaveStatScreenToMenu
.type EarlyLeaveStatScreenToMenu, %function

.global EditEarlyLeavePageNumberDisplay
.type EditEarlyLeavePageNumberDisplay, %function

.global EditEarlyLeaveDeployQuestion
.type EditEarlyLeaveDeployQuestion, %function

.global EarlyLeaveC3Condition
.type EarlyLeaveC3Condition, %function

.global IsUnitOnEarlyLeaveReplacementList
.type IsUnitOnEarlyLeaveReplacementList, %function


@-------------------------------------------
@EarlyLeaveTurnCheckFunc
@-------------------------------------------


		EarlyLeaveTurnCheckFunc:
		
		@Does not activate until a specific turn
		ldr 	r0, =gChapterData
		ldrh	r0, [r0,#0x10]
		cmp		r0, #5
		bne		EarlyLeaveTurnCheckFunc_False
		
			mov		r0, #1
			b		EarlyLeaveTurnCheckFunc_End
			
			EarlyLeaveTurnCheckFunc_False:
			mov		r0, #0
			
		EarlyLeaveTurnCheckFunc_End:
		bx		r14
		
		.align
		.ltorg


@-------------------------------------------
@CallEarlyLeaveEvent
@-------------------------------------------


		CallEarlyLeaveEvent:
		push	{r14}
		ldr		r0, =EarlyLeaveEvent
		mov		r1, #1
		blh		CallMapEventEngine, r2
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@EarlyLeaveUnitLeaves
@-------------------------------------------


		EarlyLeaveUnitLeaves:
		push	{r4-r5,r14}
		
		ldr		r0, =EarlyLeaveCharIDLink
		ldrb	r0, [r0]
		blh		GetUnitByCharId, r1
		mov		r4, r0
		
		ldr		r5, =gMapUnit
		ldr		r5, [r5]
		ldrb	r0, [r4,#0x11]
		lsl		r0, #2
		ldr		r5, [r5,r0]
		ldrb	r1, [r4,#0x10]
		mov		r0, #0
		strb	r0, [r5,r1]
		
		@Check if unit is traveling with another unit
		ldrb	r0, [r4,#0x1B]
		cmp		r0, #0
		beq		EarlyLeaveUnitLeaves_UnitLeaves
		
			@Find unit and clear traveler byte on both units
			blh		GetUnit, r1
			mov		r1, #0
			strb	r1, [r4,#0x1B]
			strb	r1, [r0,#0x1B]
			
			@Check if character is on list for different consequences
			ldr		r1, [r0]
			ldrb	r1, [r1,#0x04]
			ldr		r3, =EarlyLeaveTraitorCharIDList
			
			EarlyLeaveUnitLeaves_TraitorLoop:
			ldrb	r2, [r3]
			cmp		r2, #0
			beq		TravelerDefault
			
				cmp		r2, r1
				beq		TravelerTraitor
				
					add		r3, #1
					b		EarlyLeaveUnitLeaves_TraitorLoop
			
			TravelerDefault:
			
			@Store unit id in gMapUnit
			ldrb	r1, [r0,#0x0B]
			ldrb	r2, [r4,#0x10]
			strb	r1, [r5,r2]
			
			@Drop unit
			mov		r1, #0
			strb	r1, [r0,#0x0C]
			b		EarlyLeaveUnitLeaves_UnitLeaves
			
			TravelerTraitor:
			
			@Unit disappears and counts as progress towards C-3 objective
			blh		ClearUnit, r1
			ldr		r1, =gEventCounter
			ldrb	r0, [r1]
			add		r0, #1
			strb	r0, [r1]
			
		EarlyLeaveUnitLeaves_UnitLeaves:
		
		@Set REMU
		mov		r1, #4
		lsl		r1, #8
		add		r1, #1
		lsl		r1, #16
		add		r1, #1
		ldr		r0, [r4,#0x0C]
		orr		r0, r1
		str		r0, [r4,#0x0C]
		
		@Check if convoy has space for unit's inventory
		ldr		r0, =ConvoySizePointer
		ldrb	r0, [r0]
		ldr		r3, =ConvoyPointer
		ldr		r3, [r3]
		sub		r3, #2
		add		r0, #2
		
		EarlyLeaveUnitLeaves_SupplySlotLoop:
		cmp		r0, #0
		beq		EarlyLeaveUnitLeaves_End @if no more space, then skip the rest of routine
		
			sub		r0, #1
			add		r3, #2
			ldrh	r1, [r3]
			cmp		r1, #0
			bne		EarlyLeaveUnitLeaves_SupplySlotLoop
		
		add		r4, #0x1E
		mov		r2, #0
		
		EarlyLeaveUnitLeaves_InvLoop:
		ldrh	r1, [r4,r2]
		cmp		r1, #0
		beq		EarlyLeaveUnitLeaves_End
			
			@story item in convoy
			strh	r1, [r3]
			
			@remove from unit's inventory
			mov		r1, #0
			strh	r1, [r4,r2]
			
			@if supply still has room, we can continue checking unit's inventory
			add		r3, #2
			sub		r0, #1
			cmp		r0, #0
			beq		EarlyLeaveUnitLeaves_End
				
				@check if we've already gone through 5 items
				add		r2, #2
				cmp		r2, #10
				blt		EarlyLeaveUnitLeaves_InvLoop
		
		EarlyLeaveUnitLeaves_End:
		blh		SMS_UpdateFromGameData, r0
		
		pop		{r4-r5}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@StartEarlyLeaveMenu
@-------------------------------------------

		StartEarlyLeaveMenu:
		push	{r4-r5,r14}
		mov		r4, r0
		
		blh		InitEarlyLeaveMenuMoreInfo, r0
		
		@If player has no units that can be deployed as a replacement, do not start the actual menu
		ldr		r0, =EarlyLeaveMenuMoreInfo
		ldr		r0, [r0]
		ldrb	r0, [r0,#0x10]
		
		cmp		r0, #0
		beq		StartEarlyLeaveMenu_End
		
			@Without this, menu may not correctly appear the unless player goes to stat screen
			blh		PlayerPhase_ReloadGameGfx, r0
			
			blh		DrawSemiTransparentMenuBg, r0
			
			@Start on first page
			mov		r0, #0
			blh		UpdateEarlyLeaveMenuMoreInfo, r1
			blh		UpdateEarlyLeaveMenuCommandDefinition, r0

			ldr		r0, =EarlyLeaveMenuDefinition
			mov		r1, r4
			blh		StartMenuChild, r2
			mov		r5, r0
			
			@Start on second command from top
			add		r0, #0x60
			mov		r1, #1
			strb	r1, [r0,#1]
			strb	r1, [r0,#2]
			
			ldr		r0, =EarlyLeaveWindowProc
			mov		r1, r5
			blh		ProcStart, r2
			
			blh		EarlyLeaveUpdateFaceDisplay, r0
			
		StartEarlyLeaveMenu_End:
		pop		{r4-r5}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@InitEarlyLeaveMenuMoreInfo
@-------------------------------------------


		InitEarlyLeaveMenuMoreInfo:
		push	{r4,r14}
		ldr		r4, =EarlyLeaveMenuMoreInfo
		ldr		r4, [r4]
		
		mov		r0, #0
		str		r0, [r4]
		str		r0, [r4,#0x04]
		str		r0, [r4,#0x08]
		str		r0, [r4,#0x0C]
		str		r0, [r4,#0x10]
		str		r0, [r4,#0x14]
		
		@Total replacements
		blh		SetupEarlyLeaveTotalReplacement, r0
		strb	r0, [r4,#0x10]
		
		@Total number of pages
		mov		r1, #UnitsPerPage @total number of units per page (limit of 9 displayed commands; 1 command taken by page header)
		swi		#6
		add		r0, #1
		strb	r0, [r4,#0x11]
		
		pop		{r4}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@SetupEarlyLeaveTotalReplacement
@-------------------------------------------


		SetupEarlyLeaveTotalReplacement:
		push	{r4-r6,r14}
		ldr		r4, =EarlyLeaveMenuReplacementList
		ldr		r4, [r4]
		ldr		r0, =gChapterData
		mov		r1, #0x0E
		ldsb	r0, [r0,r1]
		ldr		r2, =EarlyLeaveCustomChapterConditionList
		
		SetupEarlyLeaveTotalReplacement_ConditionLoop:
		ldr		r1, [r2]
		cmp		r1, #0xFF
		bgt		SetupEarlyLeaveTotalReplacement_Default
		
			cmp		r1, r0
			beq		SetupEarlyLeaveTotalReplacement_Custom
			
				add		r2, #8
				b		SetupEarlyLeaveTotalReplacement_ConditionLoop
				
		SetupEarlyLeaveTotalReplacement_Custom:
		mov		r0, r4
		ldr		r1, [r2,#4]
		mov		lr, r1
		.short	0xF800
		b		SetupEarlyLeaveTotalReplacement_End
		
		SetupEarlyLeaveTotalReplacement_Default:
		mov		r0, #0
		ldr		r5, =gUnitArrayBlue
		mov		r6, #TotalPlayerUnits
		
		SetupEarlyLeaveTotalReplacement_Loop:
		
		@Units that are undeployed, not dead, not REMU'd, and not barred from being deployed are valid
		
		@Skip deployed units
		ldr		r1, [r5,#0x0C]
		mov		r2, #0x08
		tst		r1, r2
		beq		SetupEarlyLeaveTotalReplacement_Next
		
			@Skip units that are either dead, REMU'd, or barred from deployment
			mov		r3, #0x06
			lsl		r3, #24
			mov		r2, #0x01
			lsl		r2, #16
			orr		r2, r3
			add		r2, #0x04
			tst		r1, r2
			bne		SetupEarlyLeaveTotalReplacement_Next
				
				add		r0, #1
				ldrb	r1, [r5,#0x0B]
				strb	r1, [r4]
				add		r4, #1
				
				SetupEarlyLeaveTotalReplacement_Next:
				add		r5, #0x48
				sub		r6, #1
				cmp		r6, #0
				bgt		SetupEarlyLeaveTotalReplacement_Loop
				
					mov		r1, #0xFF
					strb	r1, [r4]
					add		r4, #1
					mov		r1, #0
					strb	r1, [r4]
		
		SetupEarlyLeaveTotalReplacement_End:
		pop		{r4-r6}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@-------------------------------------------
@UpdateEarlyLeaveMenuMoreInfo
@-------------------------------------------


		UpdateEarlyLeaveMenuMoreInfo:
		push	{r4}
		
		ldr		r1, =EarlyLeaveMenuMoreInfo
		ldr		r1, [r1]
		strb	r0, [r1,#0x12]
		mov		r3, #UnitsPerPage
		mul		r0, r3
		
		@Find spot in list
		ldr		r4, =EarlyLeaveMenuReplacementList
		ldr		r4, [r4]
		add		r4, r0
		mov		r2, #0
		
		UpdateEarlyLeaveMenuMoreInfo_Loop:
		ldrb	r0, [r4,r2]
		strb	r0, [r1,r2]
		add		r2, #1
		cmp		r2, r3
		bge		UpdateEarlyLeaveMenuMoreInfo_NoMoreUnitsInit
		
			cmp		r0, #0
			beq		UpdateEarlyLeaveMenuMoreInfo_NoMoreUnitsInit
			
				cmp		r0, #0xFF
				bne		UpdateEarlyLeaveMenuMoreInfo_Loop
			
		UpdateEarlyLeaveMenuMoreInfo_NoMoreUnitsInit:
		mov		r0, #0
		
		UpdateEarlyLeaveMenuMoreInfo_NoMoreUnitsLoop:
		strb	r0, [r1,r2]
		add		r2, #1
		cmp		r2, r3
		ble		UpdateEarlyLeaveMenuMoreInfo_NoMoreUnitsLoop
		
		pop		{r4}
		bx		r14
		
		.align
		.ltorg


@-------------------------------------------
@UpdateEarlyLeaveMenuCommandDefinition
@-------------------------------------------


		UpdateEarlyLeaveMenuCommandDefinition:
		push	{r4-r7,r14}
		ldr		r4, =EarlyLeaveMenuCommandDefinition
		ldr		r4, [r4]
		ldr		r5, =EarlyLeaveMenuMoreInfo
		ldr		r5, [r5]
		ldr		r6, =EarlyLeaveMenuTextPointers
		mov		r7, #0
		
		@"Pg X / Y"
		
		@Text id of command
		ldrh	r0, [r6]
		str		r0, [r4,#0x04]
		
		@Effect
		str		r7, [r4,#0x14]
		
		b		UpdateEarlyLeaveMenuCommandDefinition_StoreUsability
		
		UpdateEarlyLeaveMenuCommandDefinition_Loop:
		ldrb	r0, [r5]
		cmp		r0, #0
		beq		UpdateEarlyLeaveMenuCommandDefinition_End
		
			cmp		r0, #0xFF
			beq		UpdateEarlyLeaveMenuCommandDefinition_NoReplacement
			add		r5, #1
			
			@Text id of command
			blh		GetUnit, r1
			ldr		r0, [r0]
			ldrh	r0, [r0]
			str		r0, [r4,#0x04]
			
			@Effect
			ldr		r0, =EarlyLeaveEffect_UnitSelect
			str		r0, [r4,#0x14]
			UpdateEarlyLeaveMenuCommandDefinition_StoreUsability:
			ldr		r0, =MenuCommandAlwaysUsable
			str		r0, [r4,#0x0C]
			
			str		r7, [r4]
			str		r7, [r4,#0x08]
			str		r7, [r4,#0x10]
			str		r7, [r4,#0x18]
			str		r7, [r4,#0x1C]
			str		r7, [r4,#0x20]
			
			add		r4, #0x24
			b		UpdateEarlyLeaveMenuCommandDefinition_Loop
		
		UpdateEarlyLeaveMenuCommandDefinition_NoReplacement:
		
		@"No Replacement"
		@Text id of command
		ldrh	r0, [r6,#2]
		str		r0, [r4,#0x04]
		
		@Effect
		ldr		r0, =EarlyLeaveEffect_UnitSelect
		str		r0, [r4,#0x14]
		
		@Usability
		ldr		r0, =MenuCommandAlwaysUsable
		str		r0, [r4,#0x0C]
		
		str		r7, [r4]
		str		r7, [r4,#0x08]
		str		r7, [r4,#0x10]
		str		r7, [r4,#0x18]
		str		r7, [r4,#0x1C]
		str		r7, [r4,#0x20]
		
		add		r4, #0x24
		
		UpdateEarlyLeaveMenuCommandDefinition_End:
		str		r7, [r4]
		str		r7, [r4,#0x04]
		str		r7, [r4,#0x08]
		str		r7, [r4,#0x0C]
		str		r7, [r4,#0x10]
		str		r7, [r4,#0x14]
		str		r7, [r4,#0x18]
		str		r7, [r4,#0x1C]
		str		r7, [r4,#0x20]
		
		pop		{r4-r7}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@StartEarlyLeaveWindow
@-------------------------------------------

@mostly copied from ForceMenuItemPanel (0801E684)


		StartEarlyLeaveWindow:
		push	{r4-r5,r14}
		mov		r4, r0
		ldr		r5, =EarlyLeaveWindowParameters
		
		@x-coord
		ldrb	r0, [r5]
		mov		r1, #0x30
		strb	r0, [r4,r1]
		
		@y-coords
		ldrb	r0, [r5,#1]
		mov		r1, #0x31
		strb	r0, [r4,r1]
		
		@height
		ldrb	r0, [r5,#2]
		mov		r1, #0x32
		strb	r0, [r4,r1]
		
		mov		r0, r4
		add		r0, #0x34
		mov		r1, #0x0C
		blh		Text_InitDB, r2
		
		mov		r0, r4
		add		r0, #0x3C
		mov		r1, #0x0C
		blh		Text_InitDB, r2
		
		mov		r0, r4
		add		r0, #0x44
		mov		r1, #0x0C
		blh		Text_InitDB, r2
		
		pop		{r4-r5}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@UpdateEarlyLeaveWindow
@-------------------------------------------

@mostly copied from UpdateMenuItemPanel (0801E748)


		UpdateEarlyLeaveWindow:
		push	{r4-r7,r14}
		mov		r7, r10
		mov		r6, r9
		mov		r5, r8
		push	{r5-r7}
		add		sp, #-0x0C
		ldr		r0, =EarlyLeaveWindowProc
		blh		ProcFind, r1
		mov		r7, r0
		mov		r0, #0
		blh		GetBgMapBuffer, r1
		mov		r4, r7
		add		r4, #0x30
		ldrb	r1, [r4]
		lsl		r1, #1
		add		r0, r1
		mov		r1, #0x31
		add		r1, r7
		mov		r8, r1
		ldrb	r1, [r1]
		lsl		r1, #6
		add		r0, r1
		str		r0, [sp,#4]
		mov		r6, r7
		add		r6, #0x34
		ldr		r2, [r7,#0x2C]
		mov		r9, r2
		
		mov		r0, r7
		add		r0, #0x32
		ldrb	r0, [r0]
		str		r0, [sp,#8]
		mov		r0, r6
		blh		TextClear, r1
		mov		r0, r6
		add		r0, #0x08
		blh		TextClear, r1
		mov		r0, r6
		add		r0, #0x10
		blh		TextClear, r1
		
		ldr		r3, =MakeUIWindowTileMap_BG0BG1
		mov		lr, r3
		ldrb	r0, [r4]
		mov		r2, r8
		ldrb	r1, [r2]
		mov		r2, #0
		str		r2, [sp]
		mov		r2, #0x0E
		mov		r3, #8
		.short	0xF800
		
		ldr		r0, =EarlyLeaveWindowTextPointers
		ldrh	r0, [r0]
		blh		String_GetFromIndex, r1
		mov		r3, r0
		mov		r0, r6
		mov		r1, #0x04 @spacing within window
		mov		r2, #0
		blh		Text_InsertString, r4
		
		ldr		r0, =EarlyLeaveWindowTextPointers
		ldrh	r0, [r0,#2]
		blh		String_GetFromIndex, r1
		mov		r3, r0
		mov		r0, r6
		add		r0, #0x08
		mov		r1, #0x04 @spacing within window
		mov		r2, #0
		blh		Text_InsertString, r4
		
		ldr		r0, =EarlyLeaveWindowTextPointers
		ldrh	r0, [r0,#4]
		blh		String_GetFromIndex, r1
		mov		r3, r0
		mov		r0, r6
		add		r0, #0x10
		mov		r1, #0x04 @spacing within window
		mov		r2, #0
		blh		Text_InsertString, r4
		
		mov		r0, r7
		add		r0, #0x34
		mov		r6, r7
		add		r6, #0x31
		ldrb	r1, [r6]
		add		r1, #1
		lsl		r1, #5
		add		r1, #1
		mov		r5, r7
		add		r5, #0x30
		ldrb	r2, [r5]
		add		r1, r2
		lsl		r1, #1
		ldr		r4, =gBg0MapBuffer
		add		r1, r4
		blh		Text_Display, r2
		
		mov		r0, r7
		add		r0, #0x3C
		ldrb	r1, [r6]
		add		r1, #3
		lsl		r1, #5
		add		r1, #1
		ldrb	r2, [r5]
		add		r1, r2
		lsl		r1, #1
		add		r1, r4
		blh		Text_Display, r2
		
		mov		r0, r7
		add		r0, #0x44
		ldrb	r1, [r6]
		add		r1, #5
		lsl		r1, #5
		add		r1, #1
		ldrb	r2, [r5]
		add		r1, r2
		lsl		r1, #1
		add		r1, r4
		blh		Text_Display, r2
		
		mov		r0, #1
		blh		EnableBgSyncByMask, r1
		
		add		sp, #0x0C
		pop		{r3-r5}
		mov		r8, r3
		mov		r9, r4
		mov		r10, r5
		pop		{r4-r7}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@EarlyLeaveUpdateFaceDisplay
@-------------------------------------------

@mostly copied from ItemCommandEffect (080232E8)


		EarlyLeaveUpdateFaceDisplay:
		push	{r4-r5,r14}
		add		sp, #-0x04
		mov		r0, #0
		blh		EndFaceById, r1
		
		@Determine which menu is open to determine method of getting portrait to display 
		ldr		r5, =EarlyLeaveMenuMoreInfo
		ldr		r5, [r5]
		ldrb	r0, [r5,#0x14]
		cmp		r0, #0
		bne		EarlyLeaveUpdateFaceDisplay_NoReplacementCheck
		
			@Unit selection
			ldr		r0, =gProc_Menu
			blh		ProcFind, r1
			cmp		r0, #0
			beq		EarlyLeaveUpdateFaceDisplay_End
			
				add		r0, #0x60
				ldrb	r0, [r0,#1]
				sub		r0, #1
				ldrb	r0, [r5,r0]
				EarlyLeaveUpdateFaceDisplay_NoReplacementCheck:
				cmp		r0, #0xFF
				beq		EarlyLeaveUpdateFaceDisplay_End
			
		EarlyLeaveUpdateFaceDisplay_GetFace:
		blh		GetUnit, r1
		blh		GetUnitPortraitId, r1
		mov		r1, r0
		mov		r0, #2
		str		r0, [sp]
		mov		r0, #0
		ldr		r4, =EarlyLeavePortraitParameters
		ldrb	r2, [r4]
		ldrb	r3, [r4,#1]
		blh		StartFace, r4
		mov		r0, #0
		mov		r1, #5
		blh		SetFaceBlinkControlById, r2
			
		EarlyLeaveUpdateFaceDisplay_End:
		add		sp, #0x04
		pop		{r4-r5}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@EarlyLeaveMenuDirectionInputs
@-------------------------------------------


		EarlyLeaveMenuDirectionInputs:
		push	{r4-r6,r14}
		mov		r4, r0
		mov		r5, r1
		mov		r6, r2
		
		ldrh	r3, [r4,#6]
		
		CheckButton_Up:
		mov		r0, #0x40
		tst		r0, r3
		beq		CheckButton_Down
		
			ldrb	r0, [r6,#1]
			cmp		r0, #0
			bne		NewCursorLocation_Up
			
				ldrh	r2, [r4,#8]
				cmp		r3, r2
				bne		EarlyLeaveMenuDirectionInputs_End
				
					ldrb	r0, [r6]
					strb	r0, [r6,#1]
					
					NewCursorLocation_Up:
					ldrb	r0, [r6,#1]
					sub		r0, #1
					
					@don't let cursor reach top command
					cmp		r0, #0
					bne		StoreCursor_Up
					
						ldrb	r0, [r6]
						sub		r0, #1
						
						StoreCursor_Up:
						strb	r0, [r6,#1]
						
		CheckButton_Down:
		mov		r0, #0x80
		tst		r0, r3
		beq		DidCursorMove
		
			ldrb	r0, [r6]
			sub		r0, #1
			ldrb	r1, [r6,#1]
			cmp		r1, r0
			bne		NewCursorLocation_Down
			
				ldrh	r2, [r4,#8]
				cmp		r3, r2
				bne		EarlyLeaveMenuDirectionInputs_End
				
					mov		r0, #0xFF
					strb	r0, [r6,#1]
					
					NewCursorLocation_Down:
					ldrb	r0, [r6,#1]
					add		r0, #1
					
					@don't let cursor reach top command
					cmp		r0, #0xFF
					ble		StoreCursor_Down
					
						mov		r0, #1
						
						StoreCursor_Down:
						strb	r0, [r6,#1]
						
		DidCursorMove:
		ldrb	r1, [r6,#2]
		ldrb	r0, [r6,#1]
		cmp		r1, r0
		beq		DidCursorMove2
		
			ldrb	r1, [r6,#2]
			mov		r0, r5
			mov		r2, #0
			blh		Menu_DrawHoverThing, r3
			ldrb	r1, [r6,#1]
			mov		r0, r5
			mov		r2, #1
			blh		Menu_DrawHoverThing, r3
			
			ldr		r0, =gChapterData
			add		r0, #0x41
			ldrb	r0, [r0]
			lsl		r0, #0x1E
			cmp		r0, #0
			blt		DidCursorMove2
			
				mov		r0, #0x66
				blh		m4aSongNumStart, r1
					
		DidCursorMove2:
		ldrb	r0, [r6,#1]
		ldrb	r1, [r6,#2]
		cmp		r0, r1
		beq		EarlyLeaveMenuDirectionInputs_End
		
			blh		EarlyLeaveUpdateFaceDisplay, r0
			
			ldrb	r0, [r6,#2]
			lsl		r0, #2
			add		r0, #0x34
			ldr		r1, [r5,r0]
			ldr		r0, [r1,#0x30]
			ldr		r2, [r0,#0x20]
			cmp		r2, #0
			beq		Label_0804F350
			
				mov		r0, r5
				mov		lr, r2
				.short	0xF800
				
				Label_0804F350:
				ldrb	r0, [r6,#1]
				lsl		r0, #2
				add		r0, #0x34
				ldr		r1, [r5,r0]
				ldr		r0, [r1,#0x30]
				ldr		r2, [r0,#0x1C]
				cmp		r2, #0
				beq		EarlyLeaveMenuDirectionInputs_End
				
					mov		r0, r5
					mov		lr, r2
					.short	0xF800
		
		EarlyLeaveMenuDirectionInputs_End:
		pop		{r4-r6}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@EarlyLeaveMenuSelectInputs
@-------------------------------------------


		EarlyLeaveMenuSelectInputs:
		push	{r4-r7,r14}
		mov		r4, r1
		mov		r5, r2
		mov		r6, r3
		
		ldrh	r1, [r0,#8]
		
		@CheckButton_Right
		mov		r0, #0x10
		tst		r0, r1
		beq		CheckButton_Left
		
			ldr		r2, =EarlyLeaveMenuMoreInfo
			ldr		r2, [r2]
			ldrb	r1, [r2,#0x11]
			cmp		r1, #1
			ble		EarlyLeaveMenuSelectInputs_OnePage
			
				ldrb	r0, [r2,#0x12]
				add		r0, #1
				cmp		r0, r1
				blt		EarlyLeaveMenuSelectInputs_UpdateMoreInfo
				
					mov		r0, #0
					b		EarlyLeaveMenuSelectInputs_UpdateMoreInfo
		
		CheckButton_Left:
		mov		r0, #0x20
		tst		r0, r1
		beq		CheckButton_A
		
			ldr		r2, =EarlyLeaveMenuMoreInfo
			ldr		r2, [r2]
			ldrb	r1, [r2,#0x11]
			cmp		r1, #1
			ble		EarlyLeaveMenuSelectInputs_OnePage
			
				ldrb	r0, [r2,#0x12]
				sub		r0, #1
				cmp		r0, #0
				bge		EarlyLeaveMenuSelectInputs_UpdateMoreInfo
				
					ldrb	r0, [r2,#0x11]
					sub		r0, #1
			
		EarlyLeaveMenuSelectInputs_UpdateMoreInfo:
		blh		UpdateEarlyLeaveMenuMoreInfo, r1
		blh		UpdateEarlyLeaveMenuCommandDefinition, r0
		
		ldr		r0, =EarlyLeaveWindowProc
		blh		ProcFind, r1
		blh		EndProc, r1
		
		blh		ResetGraphicsForMenuRedraw, r0
		blh		DrawSemiTransparentMenuBg, r0
		
		ldr		r0, =EarlyLeaveMenuDefinition
		ldr		r1, [r4,#0x14]
		blh		StartMenuChild, r2
		mov		r6, r0
		mov		r1, r4
		blh		ReuseEarlyLeaveCursorLocation, r2
		
		ldr		r0, =EarlyLeaveWindowProc
		mov		r1, r6
		blh		ProcStart, r2
		
		blh		EarlyLeaveUpdateFaceDisplay, r0
		
		ldr		r0, =gChapterData
		add		r0, #0x41
		ldrb	r0, [r0]
		lsl		r0, #0x1E
		cmp		r0, #0
		blt		RestartMenuOnNewPage
		
			mov		r0, #0x66
			blh		m4aSongNumStart, r1
			
			RestartMenuOnNewPage:
			mov		r0, #0x94
			b		EarlyLeaveMenuSelectInputs_End2
		
		EarlyLeaveMenuSelectInputs_OnePage:
		ldr		r0, =gChapterData
		add		r0, #0x41
		ldrb	r0, [r0]
		lsl		r0, #0x1E
		cmp		r0, #0
		blt		EarlyLeaveMenuSelectInputs_DoNothing
		
			mov		r0, #0x6C
			blh		m4aSongNumStart, r1
			
			EarlyLeaveMenuSelectInputs_DoNothing:
			mov		r0, #0
			b		EarlyLeaveMenuSelectInputs_End2
		
		CheckButton_A:
		ldr		r7, [r5,#0x30]
		mov		r0, #1
		tst		r0, r1
		beq		CheckButton_B
		
			mov		r0, r4
			mov		r1, r5
			blh		0x0804F7E8, r2
			lsl		r0, #0x18
			lsr		r6, r0, #0x18
			cmp		r6, #0xFF
			bne		EarlyLeaveMenuSelectInputs_End
			
				ldr		r2, [r7,#0x14]
				b		CheckForValidRoutine
		
		CheckButton_B:
		mov		r0, #2
		tst		r0, r1
		beq		CheckButton_R
		
			ldr		r0, [r4,#0x30]
			ldr		r2, [r0,#0x18]
			
			CheckForValidRoutine:
			cmp		r2, #0
			beq		EarlyLeaveMenuSelectInputs_End
			
				mov		r0, r4
				mov		r1, r5
				mov		lr, r2
				.short	0xF800
				lsl		r0, #0x18
				lsr		r6, r0, #0x18
				b		EarlyLeaveMenuSelectInputs_End
				
		CheckButton_R:
		mov		r0, #0x80
		lsl		r0, #1
		tst		r0, r1
		beq		EarlyLeaveMenuSelectInputs_End
		
			ldr		r0, [r4,#0x30]
			ldr		r1, [r0,#0x1C]
			cmp		r1, #0
			beq		EarlyLeaveMenuSelectInputs_End
			
				mov		r0, r4
				mov		lr, r1
				.short	0xF800
		
		EarlyLeaveMenuSelectInputs_End:
		mov		r0, r6
		
		EarlyLeaveMenuSelectInputs_End2:
		pop		{r4-r7}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@-------------------------------------------
@ReuseEarlyLeaveCursorLocation
@-------------------------------------------


		ReuseEarlyLeaveCursorLocation:
		add		r0, #0x60 @new menu
		add		r1, #0x60 @old menu
		
		@if new menu is smaller than old menu, force cursor to still be in boundaries
		ldrb	r2, [r1,#1]
		ldrb	r3, [r0]
		cmp		r2, r3
		blt		ReuseEarlyLeaveCursorLocation_End
		
			sub		r2, r3, #1
		
		ReuseEarlyLeaveCursorLocation_End:
		strb	r2, [r0,#1]
		strb	r2, [r0,#2]
		bx		r14
		
		.align
		.ltorg

		
@-------------------------------------------
@EarlyLeaveEffect_UnitSelect
@-------------------------------------------


		EarlyLeaveEffect_UnitSelect:
		push	{r4-r6,r14}
		mov		r4, r0
		mov		r5, r1
		
		@Get cursor position and store it
		add		r0, #0x60
		ldrb	r0, [r0,#1]
		ldr		r6, =EarlyLeaveMenuMoreInfo
		ldr		r6, [r6]
		strb	r0, [r6,#0x13]
		
		@Based on cursor position, get selected unit's unit id and store that
		sub		r0, #1
		ldrb	r0, [r6,r0]
		strb	r0, [r6,#0x14]
		
		ldr		r0, =EarlyLeaveWindowProc
		blh		ProcFind, r1
		blh		EndProc, r1
		
		blh		ResetGraphicsForMenuRedraw, r0
		blh		DrawSemiTransparentMenuBg, r0
		
		ldr		r0, =EarlyLeaveUnitMenuDefinition
		ldr		r1, [r4,#0x14]
		blh		StartMenuChild, r2
		mov		r1, r0
		
		@Start on second command from top
		add		r0, #0x60
		mov		r2, #1
		strb	r2, [r0,#1]
		strb	r2, [r0,#2]
		
		ldr		r0, =EarlyLeaveWindowProc
		blh		ProcStart, r2
		
		@Sound effect
		ldr		r0, =gChapterData
		add		r0, #0x41
		ldrb	r0, [r0]
		lsl		r0, #0x1E
		cmp		r0, #0
		blt		EarlyLeaveEffect_UnitSelect_End
		
			mov		r0, #0x66
			blh		m4aSongNumStart, r1
			
		EarlyLeaveEffect_UnitSelect_End:
		mov		r0, #0x94
		pop		{r4-r6}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@-------------------------------------------
@EarlyLeaveEffect_UnitConfirm			@ adding a special case with regards to biko: reduce his move by 2 if he doesn't get his full rest.
@-------------------------------------------


		EarlyLeaveEffect_UnitConfirm:
		push	{r4-r7,r14}
		add		sp, #-0x08
		ldr		r0, =EarlyLeaveCharIDLink
		ldrb		r0, [r0]
		mov		r4, r0 @ storing this here so I can do a Biko check
		ldr		r1, =BikoCharIdLink
		ldrb		r1, [r1]
		cmp 		r0, r1
		bne		EarlyLeaveEffect_UnitConfirm_Continue

			@ apply a well-rested decrement twice? should be all I need to do
			blh		WellRested_DecrementBonus, r1
			blh		WellRested_DecrementBonus, r1
			

		EarlyLeaveEffect_UnitConfirm_Continue:
		mov 		r0, r4 @ putting it back where it belongs
		blh		GetUnitByCharId, r1
		mov		r5, r0
		
		ldr		r1, =EarlyLeaveMenuMoreInfo
		ldr		r1, [r1]
		ldrb	r0, [r1,#0x14]
		cmp		r0, #0xFF
		beq		EarlyLeaveEffect_UnitConfirm_End
		
			blh		GetUnit, r1
			mov		r6, r0
			
			@Give replacement unit coords (if early leave unit dropped a traveler, give a different set of coords)
			ldr		r7, =gMapUnit
			ldr		r7, [r7]
			ldrb	r1, [r5,#0x11]
			lsl		r1, #2
			ldr		r2, [r7,r1]
			ldrb	r3, [r5,#0x10]
			ldrb	r1, [r2,r3]
			
			cmp		r1, #0
			beq		EarlyLeaveDirectCoords
			
				@r0 = staff unit
				@r1 = unit being rescued
				
				mov		r0, r5
				mov		r1, r6
				
				mov		r2, r13
				add		r3, sp, #4
				blh		GetRescueStaffeePosition, r4
				ldr		r0, [sp]
				strb	r0, [r6,#0x10]
				ldr		r0, [sp,#4]
				strb	r0, [r6,#0x11]
				b		EarlyLeaveEffect_UnitConfirm_Deploy
				
				EarlyLeaveDirectCoords:
				ldrh	r1, [r5,#0x10]
				strh	r1, [r6,#0x10]
			
		EarlyLeaveEffect_UnitConfirm_Deploy:
		mov		r0, #0
		strb	r0, [r6,#0x0C]
		
		@Add unit to gMapUnit
		ldrb	r0, [r6,#0x11]
		lsl		r0, #2
		ldr		r1, [r7,r0]
		ldrb	r2, [r6,#0x10]
		ldrb	r0, [r6,#0x0B]
		strb	r0, [r1,r2]
		
		EarlyLeaveEffect_UnitConfirm_End:
		mov		r0, #0x37
		add		sp, #0x08
		pop		{r4-r7}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@-------------------------------------------
@EarlyLeaveEffect_UnitCancel
@-------------------------------------------


		EarlyLeaveEffect_UnitCancel:
		push	{r4-r5,r14}
		mov		r4, r0
		
		ldr		r0, =EarlyLeaveWindowProc
		blh		ProcFind, r1
		blh		EndProc, r1
		
		blh		ResetGraphicsForMenuRedraw, r0
		blh		DrawSemiTransparentMenuBg, r0
		
		ldr		r0, =EarlyLeaveMenuDefinition
		ldr		r1, [r4,#0x14]
		blh		StartMenuChild, r2
		mov		r5, r0
		ldr		r0, =EarlyLeaveWindowProc
		mov		r1, r5
		blh		ProcStart, r2
		
		@Use cursor position from previous menu (unit selection), not the current menu (confirm/cancel)
		add		r5, #0x60
		ldr		r1, =EarlyLeaveMenuMoreInfo
		ldr		r1, [r1]
		ldrb	r2, [r1,#0x13]
		strb	r2, [r5,#1]
		strb	r2, [r5,#2]
		
		@Reset stored cursor location/selected unit in EarlyLeaveMenuMoreInfo
		mov		r0, #0
		strb	r0, [r1,#0x13]
		strb	r0, [r1,#0x14]
		
		@Sound effect
		ldr		r0, =gChapterData
		add		r0, #0x41
		ldrb	r0, [r0]
		lsl		r0, #0x1E
		cmp		r0, #0
		blt		EarlyLeaveEffect_UnitCancel_End
		
			mov		r0, #0x66
			blh		m4aSongNumStart, r1
			
		EarlyLeaveEffect_UnitCancel_End:
		mov		r0, #0x94
		pop		{r4-r5}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@-------------------------------------------
@EarlyLeaveSetUndeploy
@-------------------------------------------


		EarlyLeaveSetUndeploy:
		push	{r14}
		
		ldr		r0, =EarlyLeaveCharIDLink
		ldrb	r0, [r0]
		blh		GetUnitByCharId, r1
		
		mov		r1, #4
		lsl		r1, #8
		add		r1, #1
		lsl		r1, #16
		add		r1, #8
		ldr		r2, [r0,#0x0C]
		eor		r2, r1
		str		r2, [r0,#0x0C]
		
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@EarlyLeaveViewStatScreen
@-------------------------------------------


		EarlyLeaveViewStatScreen:
		push	{r4-r6,r14}
		mov		r4, r0
		
		ldr		r1, =EarlyLeaveMenuMoreInfo
		ldr		r1, [r1]
		ldrb	r6, [r1,#0x14]
		cmp		r6, #0
		bne		EarlyLeaveViewStatScreen_CheckNoReplacement
		
			@Based on cursor position, get selected unit's unit id
			add		r0, #0x60
			ldrb	r0, [r0,#1]
			sub		r0, #1
			ldrb	r6, [r1,r0]
			
			EarlyLeaveViewStatScreen_CheckNoReplacement:
			
			@If "No Replacement" is selected, play sound effect instead
			cmp		r6, #0xFF
			beq		EarlyLeaveViewStatScreen_No
			
				ldr		r0, =EarlyLeaveStatScreenTransitionProc
				mov		r1, r4
				blh		ProcStartBlocking, r2
				mov		r5, r0
				
				mov		r0, #0x5D
				blh		SetStatScreenNextUnitLogic, r1
				
				mov		r0, r6
				blh		GetUnit, r1
				mov		r1, r5
				blh		StartStatScreen, r2
				b		EarlyLeaveViewStatScreen_End
				
				EarlyLeaveViewStatScreen_No:
				ldr		r0, =gChapterData
				add		r0, #0x41
				ldrb	r0, [r0]
				lsl		r0, #0x1E
				cmp		r0, #0
				blt		EarlyLeaveViewStatScreen_End
				
					mov		r0, #0x6C
					blh		m4aSongNumStart, r1
		
		EarlyLeaveViewStatScreen_End:
		pop		{r4-r6}
		pop		{r0}
		bx		r0


@-------------------------------------------
@EarlyLeaveStatScreenToMenu
@-------------------------------------------


		EarlyLeaveStatScreenToMenu:
		push	{r4-r7,r14}
		mov		r4, r0
		ldr		r5, =EarlyLeaveMenuMoreInfo
		ldr		r5, [r5]
		ldr		r6, =StatScreenStruct
		ldr		r6, [r6,#0x0C]
		ldrb	r6, [r6,#0x0B]
		
		ldr		r0, =EarlyLeaveWindowProc
		blh		ProcFind, r1
		blh		EndProc, r1
		
		blh		ResetGraphicsForMenuRedraw, r0
		blh		DrawSemiTransparentMenuBg, r0
		
		@Refresh EarlyLeaveMenuMoreInfo
		mov		r7, #0
		
		EarlyLeaveStatScreenToMenu_RefreshLoop:
		ldrb	r0, [r5,r7]
		cmp		r0, r6
		beq		EarlyLeaveStatScreenToMenu_UnitFound
		
			cmp		r0, #0
			beq		EarlyLeaveStatScreenToMenu_NextPage
			
				cmp		r0, #0xFF
				beq		EarlyLeaveStatScreenToMenu_FirstPage
				
					add		r7, #1
					b		EarlyLeaveStatScreenToMenu_RefreshLoop
					
			EarlyLeaveStatScreenToMenu_NextPage:
			ldrb	r0, [r5,#0x12]
			add		r0, #1
			b		EarlyLeaveStatScreenToMenu_RefreshLoopCont
			
			EarlyLeaveStatScreenToMenu_FirstPage:
			mov		r0, #0
			EarlyLeaveStatScreenToMenu_RefreshLoopCont:
			blh		UpdateEarlyLeaveMenuMoreInfo, r1
			mov		r7, #0
			b		EarlyLeaveStatScreenToMenu_RefreshLoop
			
		EarlyLeaveStatScreenToMenu_UnitFound:
		blh		UpdateEarlyLeaveMenuCommandDefinition, r0
		add		r7, #1
		
		@Find correct menu to be in
		ldrb	r0, [r5,#0x14]
		cmp		r0, #0
		bne		EarlyLeaveStatScreenToMenu_UnitMenu
		
			@Unit selection
			ldr		r0, =EarlyLeaveMenuDefinition
			b		EarlyLeaveStatScreenToMenu_End
			
			EarlyLeaveStatScreenToMenu_UnitMenu:
			strb	r7, [r5,#0x13]
			strb	r6, [r5,#0x14]
			ldr		r0, =EarlyLeaveUnitMenuDefinition
			mov		r7, #1
		
		EarlyLeaveStatScreenToMenu_End:
		ldr		r1, [r4,#0x14]
		ldr		r1, [r1,#0x14]
		blh		StartMenuChild, r2
		mov		r1, r0
		add		r0, #0x60
		strb	r7, [r0,#1]
		strb	r7, [r0,#2]
		ldr		r0, =EarlyLeaveWindowProc
		blh		ProcStart, r2
		
		ldr		r0, [r4,#0x14]
		blh		EndProc, r1
		
		blh		EarlyLeaveUpdateFaceDisplay, r0
		
		pop		{r4-r7}
		pop		{r0}
		bx		r0
		
		.align
		.ltorg


@-------------------------------------------
@EditEarlyLeavePageNumberDisplay
@-------------------------------------------


		EditEarlyLeavePageNumberDisplay:
		push	{r4,r14}
		blh		String_GetFromIndex, r1
		mov		r4, r0
		
		@get max number of pages
		ldr		r1, =EarlyLeaveMenuMoreInfo
		ldr		r1, [r1]
		ldrb	r2, [r1,#0x11]
		
		@get current page number
		ldrb	r1, [r1,#0x12]
		
		add		r1, #1 
		
		mov		r0, #0
		mov		r3, #5
		
		TensDigitLoop_CurPg:
		cmp		r1, #10
		blt		CheckTensDigit_CurPg
		
			sub		r1, #10
			add		r0, #1
			b		TensDigitLoop_CurPg
			
		CheckTensDigit_CurPg:
		cmp		r0, #0
		ble		StoreOnesDigit_CurPg
		
			add		r0, #0x30
			strb	r0, [r4,r3]
		
		StoreOnesDigit_CurPg:
		add		r1, #0x30
		cmp		r2, #10
		blt		Cont_StoreOnesDigit_CurPg
		
			add		r3, #1
			
			Cont_StoreOnesDigit_CurPg:
			strb	r1, [r4,r3]
		
		@" of "
		add		r3, #1
		mov		r0, #0x20
		strb	r0, [r4,r3]
		add		r3, #1
		mov		r0, #0x6F
		strb	r0, [r4,r3]
		add		r3, #1
		mov		r0, #0x66
		strb	r0, [r4,r3]
		add		r3, #1
		mov		r0, #0x20
		strb	r0, [r4,r3]
		
		mov		r0, #0
		
		TensDigitLoop_MaxPg:
		cmp		r2, #10
		blt		CheckTensDigit_MaxPg
		
			sub		r2, #10
			add		r0, #1
			b		TensDigitLoop_MaxPg
		
		CheckTensDigit_MaxPg:
		cmp		r0, #0
		ble		StoreOnesDigit_MaxPg
		
			add		r0, #0x30
			add		r3, #1
			strb	r0, [r4,r3]
		
		StoreOnesDigit_MaxPg:
		add		r2, #0x30
		add		r3, #1
		strb	r2, [r4,r3]
		
		@[X]
		add		r3, #1
		mov		r0, #0
		strb	r0, [r4,r3]
		
		mov		r0, r4
		pop		{r4}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@-------------------------------------------
@EditEarlyLeaveDeployQuestion
@-------------------------------------------


		EditEarlyLeaveDeployQuestion:
		push	{r4-r6,r14}
		add		sp, #-0x10
		mov		r4, r0
		
		ldr		r0, =EarlyLeaveMenuMoreInfo
		ldr		r0, [r0]
		ldrb	r0, [r0,#0x14]
		cmp		r0, #0xFF
		bne		EditEarlyLeaveDeployQuestion_GetName
		
			mov		r0, r4
			blh		String_GetFromIndex, r1
			b		EditEarlyLeaveDeployQuestion_End
			
			EditEarlyLeaveDeployQuestion_GetName:
			blh		GetUnit, r1
			ldr		r0, [r0]
			ldrh	r0, [r0]
			blh		String_GetFromIndex, r1
			mov		r5, r0
			mov		r6, r13
			mov		r1, #0
			
			EditEarlyLeaveDeployQuestion_CopyNameLoop:
			ldrb	r0, [r5,r1]
			strb	r0, [r6,r1]
			add		r1, #1
			cmp		r0, #0
			bne		EditEarlyLeaveDeployQuestion_CopyNameLoop
			
			EditEarlyLeaveDeployQuestion_InitText:
			mov		r0, r4
			blh		String_GetFromIndex, r1
			mov		r3, r0
			add		r3, #7
			mov		r2, #0
			EditEarlyLeaveDeployQuestion_StoreNameLoop:
			ldrb	r1, [r6,r2]
			strb	r1, [r3,r2]
			add		r2, #1
			cmp		r1, #0
			bne		EditEarlyLeaveDeployQuestion_StoreNameLoop
			
			@[X]
			strb	r1, [r3,r2]
			
			@"?"
			sub		r2, #1
			mov		r1, #0x3F
			strb	r1, [r3,r2]
		
		EditEarlyLeaveDeployQuestion_End:
		add		sp, #0x10
		pop		{r4-r6}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@-------------------------------------------
@EarlyLeaveC3Condition
@-------------------------------------------


		EarlyLeaveC3Condition:
		push	{r4-r5,r14}
		mov		r4, r0
		mov		r5, #0
		
		@Only command available to player is axe cav (No replacement isn't an option either)
		ldr		r0, =EarlyLeaveC3ConditionCharIDLink
		ldrb	r0, [r0]
		blh		GetUnitByCharId, r1
		
		@Failesafe checks
		cmp		r0, #0
		beq		EarlyLeaveC3Condition_End
		
			ldr		r1, [r0,#0x0C]
			mov		r2, #0x08
			tst		r1, r2
			beq		EarlyLeaveC3Condition_End
		
				mov		r3, #0x06
				lsl		r3, #24
				mov		r2, #0x01
				lsl		r2, #16
				orr		r2, r3
				add		r2, #0x04
				tst		r1, r2
				bne		EarlyLeaveC3Condition_End
			
					ldrb	r0, [r0,#0x0B]
					strb	r0, [r4]
					add		r4, #1
					mov		r5, #1
		
		EarlyLeaveC3Condition_End:
		mov		r0, #0
		strb	r0, [r4]
		mov		r0, r5
		pop		{r4-r5}
		pop		{r1}
		bx		r1
		
		.align
		.ltorg


@-------------------------------------------
@IsUnitOnEarlyLeaveReplacementList
@-------------------------------------------


		IsUnitOnEarlyLeaveReplacementList:
		ldrb	r0, [r0,#0x0B]
		ldr		r2, =EarlyLeaveMenuReplacementList
		ldr		r2, [r2]
		mov		r3, #0
		
		IsUnitOnEarlyLeaveReplacementList_Loop:
		ldrb	r1, [r2]
		cmp		r1, #0xFF
		beq		IsUnitOnEarlyLeaveReplacementList_End
		
			cmp		r1, #0
			beq		IsUnitOnEarlyLeaveReplacementList_End
			
				add		r2, #1
				cmp		r1, r0
				bne		IsUnitOnEarlyLeaveReplacementList_Loop
				
					mov		r3, #1
		
		IsUnitOnEarlyLeaveReplacementList_End:
		mov		r0, r3
		bx		r14
		
		.align
		.ltorg

