; ==================================================================================================================================
;                                                               2048                                                                  
;
; A game based on http://gabrielecirulli.github.io/2048/
; Design inspired by the C64 release http://csdb.dk/release/?id=129788
;
; Created by Julien Verneuil aka Grz- for the SMS Power! 17th Anniversary Competitions in 2014. 
;
; The first release of the game was made in four days (24/03/2014 to 27/03/2014),
; it was initially a demo but i only had three effects of which only one was totally completed and nothing else, 
; so i decided to make a game instead when i saw the C64 release of 2048.
;
; As for the code,
; most of the routines in this file were written from scratch in theses four days, 
; except the base skeleton code (interrupt section and handler, utils, etc) which were built earlier on,
; the original code was massively rushed and had some bugs which were fixed few days after i submitted the entry... :D
; Anyway, do anything you want with it and have fun!
;
; Stuff fixed in the third version (1.02):
;   - the border color was set to black
;   - the background color of the board is now back to grey instead of blue, this was modified by error in the 1.01 version
;
; Stuff fixed in the second version beside massive amount of code cleaning and optimizations (1.01):
;   - "4" can also appear randomly as a new tile instead of only the tile "2"
;   - new tiles location are chosen truly randomly now, in the previous version if the first randomly chosen location was not empty,
;     the tile was placed to the first empty location found on the board
;   - sometimes (but not always) the tiles would be merged continually until there was no merging possible in a single "turn"
;   - bugfix related to continuing the game after getting 2048
;   - help at the start of the game
;   - gfx are now compressed,
;     the background tilemap is not compressed because it seem the v0.40 of the bmp2tile tool does not take into account
;     the tile index when it save compressed tilemap
;
; website: http://garzul.tonsite.biz
; email: grz.zrg at gmail dot com
;
; ===================================================================================================================================

; ==== banking ====
.memorymap
defaultslot 0
slotsize $8000
slot 0 $0000
.endme

.rombankmap
bankstotal 1
banksize $8000
banks 1
.endro

.computesmschecksum

.sdsctag 1.02,"2048","Puzzle game for the SMS Power! 17th Anniversary","Grz-"
.smstag

.define addr_board                     $C000
.define addr_ghost                     $C100 ; ghost version of the board used to stop merged tiles :)))
.define addr_move                      $C020 ; this store the move request (direction)
.define addr_move_happened             $C030 ; indicate if a move has happened 
.define addr_move_still_possible       $C040 ; indicate if a move is still possible in one turn
.define addr_score                     $C050 ; 
.define addr_highscore                 $C060
.define addr_state_of_game             $C070 ; win, lose etc
.define addr_button_released           $C080
.define addr_screen                    $C090 ; either intro screen or game screen
.define addr_move_delay                $D010 ; delay between tiles moves in vbl
.define addr_vbl_counter               $D000
.define addr_hbl_counter               $D001
.define addr_sprite_color              $D002
.define addr_rng_seed                  $D005
.define addr_font_position             $2C00
.define addr_win_help_position         $3D82
.define addr_win_position              $3886
.define addr_lose_position             $3886
.define addr_score_position            $389E
.define addr_highscore_position        $3D5A
.define addr_score_digits_position     $38AA
.define addr_highscore_digits_position $3D6E

.define win_help_y_line                176

.define const_background_tiles_adress $1C00 ; starting adress of the background tiles
.define const_move_delay              $03 ; this should be a power of two minus one because vbl_counter%this
.define const_font_start              $160 ; index of the first tile
.define const_max_tile_index          13 ; 8092
.define const_2048_tile_index         11 ; 2048

.define P1_UP_BIT    0
.define P1_DOWN_BIT  1
.define P1_LEFT_BIT  2
.define P1_RIGHT_BIT 3
.define P1_SW1_BIT   4
.define P1_SW2_BIT   5

.define MOVE_UP    1
.define MOVE_DOWN  2
.define MOVE_LEFT  3
.define MOVE_RIGHT 4

.define EMPTY_TILE 0
.define FIRST_TILE 1

.define GAME_STATE 0
.define LOSE_STATE 1
.define WIN_STATE 2
.define HELP_STATE 3
.define CONTINUE_STATE 4

.define GAME_SCREEN 1

.bank 0 slot 0
.org $0000
.section "Boot&Setup" force
    di
    im 1
    ld sp, $dff8
	
    ld hl,VDP_DATA
	call SetupVDP
	
    ld hl,$4000 
    call VRAMToHL 
	call ClrVRAM
	call ClrPSG

	ld hl,$3E00
	call DisableSprites

    jp main
.ends

.org $0038
.section "VBL/HBL interrupt" force
	push af
	push bc
	push de
	push hl
	
	in a,($BF)
	bit 7,a
	jp nz,vblHandler
	
	jp hblHandler
	
	EOI: ; end of interrupt
	
	pop hl
	pop de
	pop bc
	pop af
	ei
	ret
.ends

; ==== pause ====
.org $0066
.section "Pause handler" force
	call ClrPSG
	retn
.ends

; ==== main ====
.section "Main" free
	main:
		call intro
		
		ld a,$2a
		ld (addr_sprite_color),a

		gameLoop:
			ld a,(addr_move) ; don't check inputs if a "move" is still ongoing
			or a
			jr nz,gameLoop
			
			call checkInputs
			jp gameLoop
		
	checkInputs:
		ld a,(addr_state_of_game)
		or a
		jr nz, +
			checkDPAD:
			
			ld a,(addr_button_released)
			or a
			jr nz,_checkReleased

			in a,($dc)
			bit P1_UP_BIT,a
			jp z,P1_UP

			bit P1_DOWN_BIT,a
			jp z,P1_DOWN
			
			bit P1_LEFT_BIT,a
			jp z,P1_LEFT

			bit P1_RIGHT_BIT,a
			jp z,P1_RIGHT
			
			_SW1Check:
			bit P1_SW1_BIT,a
			call z,NEW_GAME
			
			xor a
			ld (addr_button_released),a
			ret
			
			; this check & release the directions buttons if needed
			_checkReleased:
				cp MOVE_UP
				jr nz,++
				
				in a,($dc)
				bit P1_UP_BIT,a
				jr nz,releaseDPAD
				ret
				
				++:
				cp MOVE_DOWN
				jr nz,++
				
				in a,($dc)
				bit P1_DOWN_BIT,a
				jr nz,releaseDPAD
				ret
				
				++:
				cp MOVE_LEFT
				jr nz,++
				
				in a,($dc)
				bit P1_LEFT_BIT,a
				jr nz,releaseDPAD
				ret
				
				++:
				cp MOVE_RIGHT
				ret nz
				
				in a,($dc)
				bit P1_RIGHT_BIT,a
				jr nz,releaseDPAD
				ret
				
				releaseDPAD:
					xor a
					ld (addr_button_released),a
			ret
			
		+: ; this handle inputs for different states of the game
			cp CONTINUE_STATE
			jr z,_gameInputs

			cp HELP_STATE
			jr z,_helpInputs
			
			cp LOSE_STATE
			jr nz,_winInputs
				
			; lose
			in a,($dc)
			bit P1_SW1_BIT,a
			jr z,P1_SW1_LOSE
			ret
			
			_winInputs: ; win
			
			in a,($dc)
			bit P1_SW1_BIT,a
			jr z,P1_SW1_WIN
			
			bit P1_SW2_BIT,a
			jr z,P1_SW2_WIN ; but still continue
			ret
			
			_gameInputs:
			
			in a,($dc)
			bit P1_SW1_BIT,a
			jr z,NEW_GAME
			
			jp checkDPAD
			
			_helpInputs:
			in a,($dc)
			bit P1_SW1_BIT,a
			jr z,NEW_GAME
		ret
		
	P1_UP:
		ld a,MOVE_UP
		ld (addr_move),a
		jp end_of_dpad_check
		
	P1_DOWN:
		ld a,MOVE_DOWN
		ld (addr_move),a
		jp end_of_dpad_check
		
	P1_LEFT:
		ld a,MOVE_LEFT
		ld (addr_move),a
		jp end_of_dpad_check
		
	P1_RIGHT:
		ld a,MOVE_RIGHT
		ld (addr_move),a
		jp end_of_dpad_check
		
	end_of_dpad_check:
		ld (addr_button_released),a
		ret ; return from checkInputs
		
	NEW_GAME:
		di
		call displayOff
		call newGame
		call displayOn
		ei
		ret
		;jp gameLoop
		
	P1_SW1_LOSE:
		di
		call displayOff
		call newGame
		call displayOn
		ei
		jp gameLoop
		
	P1_SW1_WIN:
		di
		call displayOff
		call newGame
		call displayOn
		ei
		jp gameLoop
		
	P1_SW2_WIN:
		ld a,CONTINUE_STATE
		ld (addr_state_of_game),a
		
		call load8192
		jp gameLoop
		
	;=============================================================================
	; doMove
	;
	; this check if the tile can be moved/merged and do so if it is possible,
	; core of the game, called in gameLogic
	; 
	; Input: hl = RAM adress of the source tile of the board
	;        de = RAM adress of the destination tile of the board
	;=============================================================================
	doMove:	
		ld c,(hl)
		ld a,c
		or a
		ret z ; check if the source tile is empty
				
		ld a,(de)
		or a
		jr z,_destTileEmpty ; check if the destination tile is empty

		cp c
		ret nz ; check if both tiles are equals
				
		_tilesEqual: ; both tiles equals
			; check if the destination tile is the max
			cp const_max_tile_index
			ret z
					
			push hl
			ld hl,addr_ghost
			ld l,e
			inc (hl)
			pop hl
			
			ld a,(de)
			inc a
			
			cp const_2048_tile_index ; check if the tile is 2048
			jr z,_win2048
			cp const_max_tile_index ; check if 8192
			jr z,_win8192
			jr _scoring
			_win2048:
				ex af,af'
				ld a,(addr_state_of_game)
				cp CONTINUE_STATE
				jr z,+

				exx
				ld a,WIN_STATE
				ld (addr_state_of_game),a
						
				ld de,WIN
				ld hl,addr_win_position
				call putText
				
				ld de,WIN_HELP
				ld hl,addr_win_help_position
				call putText
				exx
				jr +
				
			_win8192:
				ex af,af'
				exx
				ld a,WIN_STATE
				ld (addr_state_of_game),a
						
				ld de,WIN
				ld hl,addr_win_position
				call putText
				exx
			+:
				ex af,af'
			_scoring:
					
			exx
			ld b,a
			call addToScore
			exx
			jp _updateTile
	
		_destTileEmpty: ; destination tile empty
			; update the tile in the ghost board: not needed
			/*ld a,l
			push hl
			ld hl,addr_ghost
			ld l,a
			ld a,(hl)
			or a
			jr z,+ ; if the source tile in the ghost board was 1 then reset it and propagate his ghost value to its new pos
				ld (hl),0
				ld hl,addr_ghost
				ld l,e
				inc (hl)
			+:
			pop hl
			*/
			ld a,(hl)
		_updateTile:
			push bc
			push de
			push hl
			push de
			ex af,af'
			ld b,l
			ld c,EMPTY_TILE
			call setBoardTile
			ex af,af'
			pop de
				
			ld c,a
			ld b,e
			call setBoardTile

			pop hl
			pop de
			pop bc
			
			ld a,1
			ld (addr_move_still_possible),a
				
			ld (addr_move_happened),a
	ret
	
	;=============================================================================
	; checkMove
	;
	; only check if the tile can be moved/merged
	; 
	; Input: hl = RAM adress of the source tile of the board
	;        de = RAM adress of the destination tile of the board
	; Output: a = 1 if a move/merge is possible 0 otherwise
	;=============================================================================
	checkMove:
		ld c,(hl)
		ld a,c
		or a
		jr z,_moveNotPossible ; check if the source tile is empty
				
		ld a,(de)
		or a
		jr z,+ ; check if the destination tile is empty

		cp c
		jr nz,_moveNotPossible ; check if both tiles are equals
				
		+:
		; check if the destination tile is the max
		cp const_max_tile_index
		jr z,_moveNotPossible
		
		ld a,1
		ret
		
		_moveNotPossible:
		xor a
	ret
	
	; "turn" meaning is when at least a move happened and no moves is possible anymore
	; this just place a random tile, update the highscore etc
	endOfTurn:
		xor a
		ld (addr_move),a
				
		xor a
		ld (addr_move_happened),a
				
		call placeRandomTile
		call clearGhostBoard
		call updateScore
		call checkHighScore
		ret
		
	; ===========================================================================
	; main game logic is done here (called in the VBL handler on N VBLANK)
	; ===========================================================================
	gameLogic:
		xor a
		ld (addr_move_still_possible),a

		ld a,(addr_move)
	
		cp MOVE_UP
		jp nz,cDown
			ld de,addr_board
			ld hl,addr_board+4
			ld b,12
			-:		
				push bc
				ld bc,addr_ghost
				ld c,e
				ld a,(bc)
				or a
				pop bc
				jr nz,noUp
				
				push bc
				ld bc,addr_ghost
				ld c,l
				ld a,(bc)
				or a
				pop bc
				jr nz,noUp
		
				call doMove
			
				noUp:
				inc de
				inc hl
			djnz -
				
			ld a,(addr_move_still_possible)
			or a
			ret nz
				
			ld a,(addr_move_happened)
			or a
			call nz,endOfTurn
			
			xor a
			ld (addr_move),a
			ret
		cDown:
		
		cp MOVE_DOWN
		jp nz,cLeft
			ld de,addr_board+15
			ld hl,addr_board+11
			ld b,12
			-:
				push bc
				ld bc,addr_ghost
				ld c,e
				ld a,(bc)
				or a
				pop bc
				jr nz,noDown
				
				push bc
				ld bc,addr_ghost
				ld c,l
				ld a,(bc)
				or a
				pop bc
				jr nz,noDown
		
				call doMove
			
				noDown:
				dec de
				dec hl
			djnz -
				
			ld a,(addr_move_still_possible)
			or a
			ret nz
				
			ld a,(addr_move_happened)
			or a
			call nz,endOfTurn
			
			xor a
			ld (addr_move),a
			ret
		cLeft:
		
		cp MOVE_LEFT
		jp nz,cRight
			ld de,addr_board
			ld hl,addr_board+1
			ld b,16
			-:
				push bc
				ld bc,addr_ghost
				ld c,e
				ld a,(bc)
				or a
				pop bc
				jr nz,noLeft
				
				push bc
				ld bc,addr_ghost
				ld c,l
				ld a,(bc)
				or a
				pop bc
				jr nz,noLeft
			
				ld a,16
				sub b
				inc a
				and 3
				jr z,noLeft
		
				call doMove
			
				noLeft:
				inc de
				inc hl
			djnz -
				
			ld a,(addr_move_still_possible)
			or a
			ret nz
				
			ld a,(addr_move_happened)
			or a
			call nz,endOfTurn
			
			xor a
			ld (addr_move),a
			ret
		cRight:
		
		cp MOVE_RIGHT
		ret nz
			ld de,addr_board+15
			ld hl,addr_board+14
			ld b,16
			-:
				push bc
				ld bc,addr_ghost
				ld c,e
				ld a,(bc)
				or a
				pop bc
				jr nz,noRight
				
				push bc
				ld bc,addr_ghost
				ld c,l
				ld a,(bc)
				or a
				pop bc
				jr nz,noRight
			
				ld a,16
				sub b
				inc a
				and 3
				jr z,noRight
		
				call doMove
			
				noRight:
				dec de
				dec hl
			djnz -
				
			ld a,(addr_move_still_possible)
			or a
			ret nz
				
			ld a,(addr_move_happened)
			or a
			call nz,endOfTurn
			
			xor a
			ld (addr_move),a
			ret
		
	; ===============================================================
	; check if at least one move is possible for all dirs
	; ===============================================================
	checkIfMovePossible:
		ld de,addr_board
		ld hl,addr_board+4
		ld b,12
		-:		
			call checkMove
			or a
			ret nz

			inc de
			inc hl
		djnz -
		
		ld de,addr_board+15
		ld hl,addr_board+11
		ld b,12
		-:
			call checkMove
			or a
			ret nz

			dec de
			dec hl
		djnz -
		
		ld de,addr_board
		ld hl,addr_board+1
		ld b,16
		-:
			ld a,16
			sub b
			inc a
			and 3
			jr z,+
		
			call checkMove
			or a
			ret nz
			
			+:
			inc de
			inc hl
		djnz -
		
		ld de,addr_board+15
		ld hl,addr_board+14
		ld b,16
		-:
			ld a,16
			sub b
			inc a
			and 3
			jr z,+
		
			call checkMove
			or a
			ret nz
			
			+:
			dec de
			dec hl
		djnz -
		
		ld hl,addr_lose_position
		
		ld a,(addr_state_of_game)
		cp CONTINUE_STATE
		jr nz,+ ; this is just aesthetics fix
			dec hl
			dec hl
		+:
		ld de,LOSE
		call putText
		
		ld a,LOSE_STATE
		ld (addr_state_of_game),a
	ret
.ends

.section "Vbl handler" free
	vblHandler:
		xor a
		ld (addr_hbl_counter),a
		
		; increase vbl counter
		ld hl,addr_vbl_counter
		inc (hl)
	
		; if in intro screen, no need to update
		ld a,(addr_screen)
		or a
		jp z,EOI
		
		; same for lose state
		ld a,(addr_state_of_game)
		cp LOSE_STATE
		jp z,EOI

		; check the move delay
		ld hl,addr_move_delay
		ld b,(hl)
		ld a,(addr_vbl_counter)
		and b
		jp nz,EOI
		
		; === display off
		ld a,(VDP_DATA+1)
		out ($bf),a
		inc hl
		dec hl
		ld a,$81
		out ($bf),a
		
		call gameLogic
		
		call checkIfMovePossible
		
		; === display on
		ld a,(VDP_DATA+1)
		or $40
		out ($bf),a
		ld a,$81
		out ($bf),a	
		
		ld hl,addr_sprite_color
		inc (hl)
		
		ld a,(hl)
		ld (addr_sprite_color+1),a
		
		jp EOI
.ends

.section "Hbl handler" free
	hblHandler:
		ld hl,(addr_rng_seed)
		inc hl
		ld (addr_rng_seed),hl
		jp EOI
.ends

.section "Utils" free
	.include "game_utils.inc"
	.include "utils.inc"
.ends

; ==== data section ====
.section "Game Data" free		
	; VDP data
	VDP_DATA:
	.db $16,$a0,$ff,$ff,$ff,$fc,$fb,$f2,$00,$00,$00
	
	; graphics data and routines to load them
	.include "gfx/data.inc"
.ends

.include "Phantasy Star decompressors.inc"
