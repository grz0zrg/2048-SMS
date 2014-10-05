; setup & run the intro screen
intro:
	call loadFont
	call loadIntroBackground
		
	ld de,PRESS_START
	ld hl,$3C0C
	call putText
		
	ld de,BASED
	ld hl,$3d18
	call putText
		
	ld de,CREDIT
	ld hl,$3d8e
	call putText
		
	ld de,AUTHOR
	ld hl,$3b2a
	call putText
		
	xor a
	ld (addr_screen),a
	
	call displayOn
		
	ei
	halt
		
	introLoop:
		in a,($dc)
		bit P1_SW1_BIT,a
		jr nz,introLoop
	
	di
	call gameSetup
	ret

; setup the game screen
gameSetup:
	call displayOff
	
	ld hl,$4000 
	call VRAMToHL 
	call ClrVRAM

	call ClrPSG

	ld hl,$3E00
	call DisableSprites

	call loadBackground
	call loadBoardTiles
	
	call loadHelpPalettes
		
	ld a,HELP_STATE
	ld (addr_state_of_game),a
		
	call loadFont
	call setupHelp
	;call newGame
	;call setupHighScore
	
	ld a,const_move_delay
	ld (addr_move_delay),a
		
	call displayOn
	
	ei
	halt
	ret
	
setupHelp:
	; the d-pad...
	ld de,HELP_L1
	ld hl,$3924
	call putText
	
	ld de,HELP_L2
	ld hl,$395e
	call putText
	
	; start button...
	ld de,HELP_L3
	ld hl,$39e0
	call putText
	
	ld de,HELP_L4
	ld hl,$3a1c
	call putText
	
	; desc...
	ld de,HELP_L5
	ld hl,$3aa2
	call putText
	
	ld de,HELP_L6
	ld hl,$3ae0
	call putText
	
	; main goal is...
	ld de,HELP_L7
	ld hl,$3b60
	call putText
	
	ld de,HELP_L8
	ld hl,$3b9e
	call putText
	
	;ld de,HELP_L9
	;ld hl,$3be6
	;call putText
	
	ld de,HELP_L9
	ld hl,$3c22
	call putText
	
	ld de,HELP_L10
	ld hl,$3c60
	call putText
	
	ld de,HELP_L11
	ld hl,$3ca0
	call putText
	
	ld de,HELP_L12
	ld hl,$3ce0
	call putText
ret
	
; start a new game
newGame:
	xor a
	ld (addr_state_of_game),a
	ld (addr_move),a
	ld (addr_move_happened),a
	ld (addr_move_still_possible),a
		
	call clearBoard
	
	call loadBackground

	ld de,HIGHSCORE_DATA
	ld hl,addr_highscore_position
	call putText

	ld a,1 ; force
	call updateHighScore
	call setupScore
	
	call updateBoard
	
	call loadPalettes
	
	ld de,(addr_rng_seed)
	call Rand16
	
	ld a,l
	and $f
	ld b,a
	ld c,FIRST_TILE
	call setBoardTile
	
	; === debug, place two 1024 tiles
	/*ld b,0
	ld c,10
	call setBoardTile
	
	ld b,1
	ld c,10
	call setBoardTile*/
	; === end

	call placeRandomTile
	
	ld a,GAME_SCREEN
	ld (addr_screen),a
	
	xor a
	ld (addr_button_released),a
	
	xor a
	ld (addr_vbl_counter),a
	ret
	
; update the background so it display 8192 for the second part of the game
load8192:
	di
	call displayOff
	call loadBackground8192
	call updateHighScore
	call updateScore
	
	ld de,HIGHSCORE_DATA
	ld hl,addr_highscore_position
	call putText
	
	ld de,SCORE_DATA
	ld hl,addr_score_position
	call putText
	
	call updateBoard
	call displayOn
	ei
	ret

setupHighScore:
	xor a
	ld (addr_highscore),a
	ld (addr_highscore+1),a
	ld (addr_highscore+2),a
	ld (addr_highscore+3),a
	ld de,HIGHSCORE_DATA
	ld hl,addr_highscore_position
	call putText
	
	ld de,SCORE_DIGITS_DATA
	ld hl,addr_highscore_position+20
	call putText
	ret
	
setupScore:
	ld de,SCORE_DATA
	ld hl,addr_score_position
	call putText
	
	ld de,SCORE_DIGITS_DATA
	ld hl,addr_score_position+12
	call putText
	
	xor a
	ld (addr_score),a
	ld (addr_score+1),a
	ld (addr_score+2),a
	ld (addr_score+3),a
	ret
	
updateScore: ; visual update
	ld hl,(addr_score + 2)
	ld de,addr_score_digits_position ; vram adress of the score chunk to update!
		
	ld a,h
	call bcdA
	ld a,l
	call bcdA
		
	ld hl,(addr_score)
	ld de,addr_score_digits_position+8 ; vram adress of the score chunk to update!

	ld a,h
	call bcdA
	ld a,l
	call bcdA
	ret
	
checkHighScore:
	; this check if an highscore update is needed
	ld de,addr_score+3
	ld hl,addr_highscore+3
	ld b,4
	-:
		ld a,(de)
		ld c,(hl)
		cp c
		jr z,+
		jr nc,_copyScoreToHighscore
			ld a,c
			or a
			jr z,+
			ret ; highscore > score
		+:
			
		dec de
		dec hl
	djnz -
		
	_copyScoreToHighscore:
		
	ld de,addr_score
	ld hl,addr_highscore
	ld b,4
	-:
		ld a,(de)
		ld (hl),a
		inc de
		inc hl
	djnz -
	
	call updateHighScore
	ret
		
updateHighScore: ; visual update
	ld hl,(addr_highscore + 2)
	ld de,addr_highscore_digits_position ; vram adress of the score chunk to update!

	ld a,h
	call bcdA
	ld a,l
	call bcdA
		
	ld hl,(addr_highscore)
	ld de,addr_highscore_digits_position+8 ; vram adress of the score chunk to update!

	ld a,h
	call bcdA
	ld a,l
	call bcdA
	ret
	
; update the display of the entire board
updateBoard:
	ld hl,addr_board+15
	ld b,$10
	_updateTile:
		ld a,(hl)
		dec hl

		push hl
		push bc
		
		dec b
		ld c,a
		call setBoardTile
		pop bc
		pop hl
	djnz _updateTile
	ret
		
clearBoard:
	call loadBackgroundTileMap
	
	ld hl,addr_board
	xor a
	ld b,$10
	_clearTile:
		ld (hl),a
		inc hl
			
		push hl
		push bc
		dec b

		ld c,EMPTY_TILE
		call setBoardTile
		pop bc
		pop hl
	djnz _clearTile
	ret
		
clearGhostBoard:
	ld hl,addr_ghost
	ld b,16
	_clearGhostTile:
		xor a
		ld (hl),a
		inc hl
	djnz _clearGhostTile
	ret
	
;======================================================================
; addToScore
;
; this will calculate the value to add based on the tile type index
; then convert it to bcd, add it to the score and store it
; 
; Input: b = tile type index
;======================================================================
addToScore:
	ld hl,2
	dec b

	_shift_hl: ; get the value of a tile
		add hl,hl
		djnz _shift_hl
					
	ex af,af'
	ld a,h
	call bin2bcd
	ld b,a
	ld a,l
	call bin2bcd
	ld c,a
	
	; this bcd chunk was ripped from cpcwiki.eu :p
	ld hl,(addr_score)
	ld a,l
	add a,c
	daa
	ld l,a
	ld a,h
	adc a,b
	daa
	ld h,a
	ld (addr_score),hl
	jr nc,+
	ld hl,(addr_score + 2)
	ld a,l
	add a,1
	daa
	ld l,a
	ld a,h
	adc a,0
	daa
	ld h,a
	ld (addr_score + 2),hl
	; end
	
	+:
	ex af,af'
	ret
	
;======================================================================
; bcdA - source: cpcwiki.eu
;
; this just display a 8 bits bcd at a VRAM location pointed by de
; 
; Input: a = 8 bits bcd chunk
;        de = VRAM location
;======================================================================
bcdA:
	ld h,a
	rra
	rra
	rra
	rra
	and $f
	add a,$30
	call putCharacter
	ld a,h
	and $f
	cp $a ; fix a bug related to the score (it sucks but it work!)
	add a,$30
	call putCharacter
	ret

;======================================================================
; setBoardTile
;
; Input: b = tile index of the board (0 to $F)
;        c = tile index which will be placed on the board (0 to $B)
;======================================================================
setBoardTile:
	; put in memory
	ld hl,addr_board
	ld l,b
	ld (hl),c
	
	; determine starting nametable adress of the board tile to modify
	ld hl,BoardTilesPosition
	ld a,l
	add a,b
	add a,b
	ld l,a
	ld e,(hl)
	inc hl
	ld d,(hl)
	ex de,hl
	
	; calculate tile number based on the index
	ld b,c
	xor a
	-:
		add a,$10
	djnz -
	ld c,a
	
	; set board tile in the nametable, this is unrolled
	ld de,$0040 ; nametable jump
	
	; select adress
	ld a,l
	out ($bf),a
	ld a,h
	or $40
	out ($bf),a
	
	; copy
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	add hl,de
	
	; select adress
	ld a,l
	out ($bf),a
	ld a,h
	or $40
	out ($bf),a
	
	; copy
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	add hl,de
	
	; select adress
	ld a,l
	out ($bf),a
	ld a,h
	or $40
	out ($bf),a
	
	; copy
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	add hl,de
	
	; select adress
	ld a,l
	out ($bf),a
	ld a,h
	or $40
	out ($bf),a
	
	; copy
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	
	ld a,c
	inc c
	out ($be),a
	xor a
	out ($be),a
	ret
	
; nametable position in VRAM of each tiles of the board ordered by their index
BoardTilesPosition:
.dw $391C $3924 $392C $3934 $3A1C $3A24 $3A2C $3A34 $3B1C $3B24 $3B2C $3B34 $3C1C $3C24 $3C2C $3C34

;======================================================================
; placeRandomTile
;
; place a random tile on the board either with number "2" or "4"
;======================================================================
placeRandomTile:
	ld de,(addr_rng_seed)
	call Rand16

	; this will choose either 2 or 4
	ld a,h
	and $f
	ld b,a
		
	ld a,l
	and $1
	jr nz,_choose4
		ld c,FIRST_TILE
		jp +
	_choose4:
		
	ld c,FIRST_TILE+1
	
	+:
		
	; check if already exist
	ld de,addr_board
	ld e,b
	ld a,(DE)
	or a
	jr nz,_placeItElsewhere
		call setBoardTile
		ret
	_placeItElsewhere:
	
		; count empty tiles
		ld b,15
		ld h,0
		-:
			ld de,addr_board
			ld e,b
			ld a,(DE)
			or a
			jr nz,_nonEmpty
				inc h
			_nonEmpty:
		djnz -
		
		;ld a,h
		
		; mod, D / E -> D (remainder in A), basically select a random empty tile
		ld a,l
		and $f
		ld d,a
		ld e,h
		
		push bc
		xor a
		ld b,8
	-:  sla d
		rla
		cp e
		jr c,+
		sub e
		inc d
	+:  djnz -
		pop bc
	
		ld h,a
			
		; put the tile on the board, this chunk could be better i guess...
		ld l,0
		ld b,15
		-:
			ld de,addr_board
			ld e,b
			ld a,(DE)
			or a
			jr nz,++
				ld a,h
				cp l
				jr nz,+
					call setBoardTile
					ret
			+:
			inc l
			++:
		djnz -	
	ret
